#include <opencv2/opencv.hpp>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>

#define PORT 8080

void sendFrame(int client_socket, const std::vector<uchar>& buffer) {
    std::ostringstream header;
    header << "--frame\r\n"
           << "Content-Type: image/jpeg\r\n"
           << "Content-Length: " << buffer.size() << "\r\n\r\n";
    std::string header_str = header.str();

    send(client_socket, header_str.c_str(), header_str.size(), 0);
    send(client_socket, buffer.data(), buffer.size(), 0);
    send(client_socket, "\r\n", 2, 0);
}

int main() {
    // Inicializar la captura de video
    cv::VideoCapture cap(2);
    if (!cap.isOpened()) {
        std::cerr << "Error al abrir la cámara" << std::endl;
        return -1;
    }

    // Configurar el socket del servidor
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar el socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Error al enlazar el socket");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(server_fd, 3) < 0) {
        perror("Error al escuchar");
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor MJPEG corriendo en http://localhost:" << PORT << "/ ..." << std::endl;

    // Aceptar una conexión
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Error al aceptar la conexión");
        exit(EXIT_FAILURE);
    }

    // Enviar la cabecera HTTP inicial
    std::string http_header = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    send(client_socket, http_header.c_str(), http_header.size(), 0);

    // Loop para capturar y enviar frames
    cv::Mat frame;
    std::vector<uchar> buffer;
    while (true) {
        cap >> frame; // Capturar frame
        if (frame.empty()) {
            std::cerr << "Error al capturar el frame" << std::endl;
            break;
        }

        // Codificar el frame en JPEG
        cv::imencode(".jpg", frame, buffer);

        // Enviar el frame al cliente
        sendFrame(client_socket, buffer);

        // Mostrar el frame en el servidor
        cv::imshow("Servidor", frame);
        if (cv::waitKey(1) == 'q') break;
    }

    // Liberar recursos
    cap.release();
    close(client_socket);
    close(server_fd);
    cv::destroyAllWindows();

    return 0;
}

