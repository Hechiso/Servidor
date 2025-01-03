#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

constexpr int Puerto = 8080;

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
        // Enviar encabezado inicial HTTP
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

