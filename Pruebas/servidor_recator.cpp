#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <poll.h>
#include <fstream>
#include <sys/types.h>
#include <sstream>


// Configuración del servidor
constexpr int PORT = 8080; // Puerto del servidor
constexpr int BACKLOG = 10; // Número máximo de conexiones pendientes
constexpr int BUFFER_SIZE = 1024; // Tamaño del buffer

// Función para configurar el socket como no bloqueante
void setNonBlocking(int socket_fd) {
    int flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

// Función para leer el contenido de un archivo
std::string readFile(const std::string& filePath) {

    std::ifstream file(filePath); // Abre el archivo
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Lee el contenido completo
    file.close();
    return buffer.str(); // Devuelve el contenido como una cadena
}




// Lee el archivo index.html
std::string htmlContent = readFile("index.html");


// Respuesta simple para los clientes
const std::string HTTP_RESPONSE = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n"
            "\r\n" +
            htmlContent;
int main() {
    // 1. Crear un socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // 2. Configurar el socket para reutilizar direcciones y no bloquear
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setNonBlocking(server_fd);

    // 3. Enlazar el socket a una dirección y puerto
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Escucha en todas las interfaces locales
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al enlazar el socket");
        close(server_fd);
        return 1;
    }

    // 4. Poner el socket en modo de escucha
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Error al escuchar");
        close(server_fd);
        return 1;
    }

    std::cout << "Servidor iniciado en http://127.0.0.1:" << PORT << "\n";

    // 5. Configurar el bucle de eventos con poll
    std::vector<pollfd> poll_fds;
    poll_fds.push_back({server_fd, POLLIN, 0}); // Añadir el socket del servidor
   
    while (true) {
        // 6. Llamar a poll para monitorizar eventos
        int event_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (event_count < 0) {
            perror("Error en poll");
            break;
        }

        // 7. Iterar sobre los sockets monitorizados
        for (size_t i = 0; i < poll_fds.size(); ++i) {
            // Si hay una nueva conexión entrante
            if (poll_fds[i].fd == server_fd && (poll_fds[i].revents & POLLIN)) {
                sockaddr_in client_addr{};
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
                if (client_fd >= 0) {
                    setNonBlocking(client_fd);
                    poll_fds.push_back({client_fd, POLLIN, 0}); // Añadir el cliente a poll
                }
            }
            // Si hay datos de un cliente existente
            else if (poll_fds[i].revents & POLLIN) {
                char buffer[BUFFER_SIZE];
                int bytes_read = read(poll_fds[i].fd, buffer, sizeof(buffer));
                if (bytes_read > 0) {
                    // Responder con HTTP
                    write(poll_fds[i].fd, HTTP_RESPONSE.c_str(), HTTP_RESPONSE.size());
                }
                // Cerrar conexión si el cliente terminó
                else {
                    close(poll_fds[i].fd);
                    poll_fds.erase(poll_fds.begin() + i);
                    --i; // Ajustar índice después de eliminar
                 }
            }
        }
    }

    // 8. Limpiar recursos al terminar
    close(server_fd);
    return 0;
}

