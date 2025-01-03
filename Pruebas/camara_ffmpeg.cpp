extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
    #include <libavdevice/avdevice.h> // Incluye esta cabecera para dispositivos
    #include <libswscale/swscale.h>
}

#include <iostream>

int main() {
    // Inicializar la biblioteca de dispositivos (esto reemplaza avdevice_register_all)
    avdevice_register_all();

    // Configuración del formato de entrada para capturar desde V4L2
    AVFormatContext* formatContext = avformat_alloc_context();
    AVInputFormat* inputFormat = av_find_input_format("v4l2");
    if (!inputFormat) {
        std::cerr << "Error: No se encontró el formato de entrada v4l2." << std::endl;
        return -1;
    }

    // Abrir el dispositivo de video (cámara web)
    if (avformat_open_input(&formatContext, "/dev/video0", inputFormat, nullptr) != 0) {
        std::cerr << "Error: No se pudo abrir la cámara." << std::endl;
        return -1;
    }

    std::cout << "Cámara abierta correctamente." << std::endl;

    // Leer los frames capturados
    AVPacket packet;
    while (av_read_frame(formatContext, &packet) >= 0) {
        std::cout << "Frame capturado, tamaño: " << packet.size << " bytes" << std::endl;
        av_packet_unref(&packet); // Liberar memoria del paquete
    }

    // Liberar recursos
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);

    return 0;
}

