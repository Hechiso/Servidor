#include <boost/asio.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <filesystem> // Para manejar rutas

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

constexpr int Puerto = 8080;
const std::string directorio_static = "static"; // Carpeta para archivos estáticos

// Función auxiliar para verificar sufijo
bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.size() < suffix.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

// Función para leer un archivo y devolver su contenido como cadena
std::string leerArchivo(const std::string& ruta_archivo) {
    std::ifstream archivo(ruta_archivo, std::ios::binary);
    if (!archivo.is_open()) {
        return ""; // Retorna vacío si no se puede abrir el archivo
    }

    std::ostringstream buffer;
    buffer << archivo.rdbuf(); // Carga todo el contenido del archivo
    return buffer.str();
}

// Función para manejar solicitudes HTTP
void manejarSolicitud(tcp::socket socket) {
    try {
        // Leer la solicitud del cliente
        char buffer[1024];
        size_t bytes_leidos = socket.read_some(boost::asio::buffer(buffer));
        std::string solicitud(buffer, bytes_leidos);

        // Obtener la ruta del recurso solicitado
        std::istringstream stream_solicitud(solicitud);
        std::string metodo, ruta, version_http;
        stream_solicitud >> metodo >> ruta >> version_http;

        // Quitar el primer '/' de la ruta
        if (!ruta.empty() && ruta.front() == '/') {
            ruta.erase(0, 1);
        }
        if (ruta.empty()) {
            ruta = "index.html"; // Cargar index.html por defecto
        }

        // Construir la ruta absoluta del archivo solicitado
        std::string ruta_completa = directorio_static + "/" + ruta;

        // Verificar si el archivo existe
        if (fs::exists(ruta_completa) && fs::is_regular_file(ruta_completa)) {
            // Leer el archivo y enviar la respuesta
            std::string contenido = leerArchivo(ruta_completa);
            std::string tipo_contenido = "text/plain";

            if (endsWith(ruta, ".html")) tipo_contenido = "text/html";
            else if (endsWith(ruta, ".css")) tipo_contenido = "text/css";
            else if (endsWith(ruta, ".js")) tipo_contenido = "application/javascript";
            else if (endsWith(ruta, ".jpg") || endsWith(ruta, ".jpeg")) tipo_contenido = "image/jpeg";
            else if (endsWith(ruta, ".png")) tipo_contenido = "image/png";

            std::ostringstream respuesta;
            respuesta << "HTTP/1.1 200 OK\r\n"
                      << "Content-Type: " << tipo_contenido << "\r\n"
                      << "Content-Length: " << contenido.size() << "\r\n\r\n"
                      << contenido;

            boost::asio::write(socket, boost::asio::buffer(respuesta.str()));
        } else {
            // Responder con 404 si el archivo no existe
            std::string respuesta =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 49\r\n\r\n"
                "<html><body><h1>404 Not Found</h1></body></html>";
            boost::asio::write(socket, boost::asio::buffer(respuesta));
        }
    } catch (std::exception& e) {
        std::cerr << "Error al manejar solicitud: " << e.what() << std::endl;
    }
}


int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), Puerto));

        std::cout << "Servidor iniciado en http://127.0.0.1:" << Puerto << std::endl;

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            // Crear un nuevo hilo para manejar la solicitud
            std::thread hilo(manejarSolicitud, std::move(socket));
            hilo.detach(); // Desvincular el hilo para evitar bloqueos
        }
    } catch (std::exception& e) {
        std::cerr << "Error en el servidor: " << e.what() << std::endl;
    }

    return 0;
}
