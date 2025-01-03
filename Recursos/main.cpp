#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
using boost::asio::ip::tcp;

constexpr int Puerto = 8080;
const std::string directorio_estatico = "./www"; // Carpeta donde están los archivos estáticos

// Función para leer un archivo desde el sistema de archivos
std::string leerArchivo(const std::string& ruta) {
    std::ifstream archivo(ruta, std::ios::binary);
    if (!archivo) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
    }

    std::ostringstream contenido;
    contenido << archivo.rdbuf();
    return contenido.str();
}

// Función para obtener el tipo de contenido basado en la extensión del archivo
std::string obtenerTipoContenido(const std::string& extension) {
    if (extension == ".html" || extension == ".htm") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".js") return "application/javascript";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    if (extension == ".png") return "image/png";
    if (extension == ".gif") return "image/gif";
    if (extension == ".ico") return "image/x-icon";
    return "application/octet-stream";
}

// Función para enviar un archivo estático
void servirArchivo(tcp::socket& socket, const std::string& ruta) {
    try {
        std::string ruta_completa = directorio_estatico + ruta;

        // Si la ruta es una carpeta, servir index.html
        if (fs::is_directory(ruta_completa)) {
            ruta_completa += "/index.html";
        }

        // Verificar que el archivo existe
        if (!fs::exists(ruta_completa) || !fs::is_regular_file(ruta_completa)) {
            throw std::runtime_error("Archivo no encontrado.");
        }

        // Leer el archivo y determinar el tipo de contenido
        std::string contenido = leerArchivo(ruta_completa);
        std::string extension = fs::path(ruta_completa).extension().string();
        std::string tipo_contenido = obtenerTipoContenido(extension);

        // Construir y enviar la respuesta HTTP
        std::ostringstream respuesta;
        respuesta << "HTTP/1.1 200 OK\r\n"
                  << "Content-Type: " << tipo_contenido << "\r\n"
                  << "Content-Length: " << contenido.size() << "\r\n"
                  << "Connection: close\r\n\r\n"
                  << contenido;

        boost::asio::write(socket, boost::asio::buffer(respuesta.str()));
    } catch (const std::exception& e) {
        std::cerr << "Error al servir el archivo: " << e.what() << std::endl;

        // Responder con 404 Not Found
        std::string respuesta = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        boost::asio::write(socket, boost::asio::buffer(respuesta));
    }
}

// Función para enviar un frame a un cliente
void enviarFrame(tcp::socket& socket, const cv::Mat& frame) {
    std::vector<uchar> bufferJPEG;
    std::vector<int> parametros = {cv::IMWRITE_JPEG_QUALITY, 80};

    // Codificar el frame en formato JPEG
    if (!cv::imencode(".jpg", frame, bufferJPEG, parametros)) {
        std::cerr << "Error al codificar el frame." << std::endl;
        return;
    }

    // Construir encabezado HTTP y el contenido
    std::ostringstream stream;
    stream << "--frame\r\n"
           << "Content-Type: image/jpeg\r\n"
           << "Content-Length: " << bufferJPEG.size() << "\r\n\r\n";

    std::string encabezado = stream.str();
    try {
        boost::asio::write(socket, boost::asio::buffer(encabezado));
        boost::asio::write(socket, boost::asio::buffer(bufferJPEG));
        boost::asio::write(socket, boost::asio::buffer("\r\n"));
    } catch (std::exception& e) {
        std::cerr << "Error al enviar el frame: " << e.what() << std::endl;
    }
}

// Hilo para manejar un cliente
void manejarCliente(tcp::socket socket, cv::VideoCapture& captura) {
    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\r\n\r\n");

        std::istream stream(&buffer);
        std::string metodo, ruta, version_http;
        stream >> metodo >> ruta >> version_http;

        std::cout << "Solicitud: " << metodo << " " << ruta << std::endl;

        if (ruta == "/stream") {
            // Enviar encabezado inicial HTTP para el stream
            std::string http_header = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
            boost::asio::write(socket, boost::asio::buffer(http_header));

            // Enviar frames continuamente
            cv::Mat frame;
            while (true) {
                captura >> frame;
                if (frame.empty()) {
                    std::cerr << "No se pudo capturar el frame." << std::endl;
                    break;
                }

                enviarFrame(socket, frame);
                std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
            }
        } else {
            // Servir archivo estático
            servirArchivo(socket, ruta);
        }
    } catch (std::exception& e) {
        std::cerr << "Cliente desconectado: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), Puerto));
        cv::VideoCapture captura(2);

        if (!captura.isOpened()) {
            std::cerr << "Error al abrir la cámara." << std::endl;
            return 1;
        }

        std::cout << "Servidor iniciado en http://127.0.0.1:" << Puerto << std::endl;

        while (true) {
            tcp::socket socket(io_context);

            // Aceptar nuevas conexiones
            acceptor.accept(socket);
            std::cout << "Cliente conectado desde: " << socket.remote_endpoint() << std::endl;

            // Crear un nuevo hilo para manejar al cliente
            std::thread(manejarCliente, std::move(socket), std::ref(captura)).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Error en el servidor: " << e.what() << std::endl;
    }

    return 0;
}
