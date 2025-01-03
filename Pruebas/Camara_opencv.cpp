#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Abrir el dispositivo de captura (por defecto la cámara 0)
    cv::VideoCapture cap(2);
    if (!cap.isOpened()) {
        std::cerr << "Error al abrir la cámara" << std::endl;
        return -1;
    }

    // Configurar el formato de captura (opcional)
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // Crear un objeto VideoWriter para guardar el video
    cv::VideoWriter writer("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(640, 480));

    if (!writer.isOpened()) {
        std::cerr << "Error al abrir el archivo de salida" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        // Capturar un frame de la cámara
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error al capturar el frame" << std::endl;
            break;
        }

        // Mostrar el frame en una ventana
        cv::imshow("Frame", frame);

        // Guardar el frame en el archivo
        writer.write(frame);

        // Esperar por una tecla, si presionas 'q', salimos del loop
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Liberar los recursos
    cap.release();
    writer.release();
    cv::destroyAllWindows();

    return 0;
}

