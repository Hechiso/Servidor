#include <iostream>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <ctime>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <sstream>
#include <vector>

//------------------------- librerias camara ----------------------------------
#include <opencv2/opencv.hpp>
//------------------------------------------------------------------------


#include <map>

// Función para procesar una solicitud HTTP y extraer los encabezados
void procesarHttp(const std::string& httpRequest) {
    std::istringstream stream(httpRequest);
    std::string line;

    // Almacenar encabezados en un mapa
    std::map<std::string, std::string> headers;

    // Leer la primera línea (línea de solicitud)
    std::getline(stream, line);
    std::cout << "Request Line: " << line << std::endl;

    // Leer los encabezados
    while (std::getline(stream, line) && line != "\r") {
        size_t delimiter = line.find(":");
        if (delimiter != std::string::npos) {
            std::string key = line.substr(0, delimiter);
            std::string value = line.substr(delimiter + 1);
            // Eliminar espacios en blanco iniciales
            while (!value.empty() && value[0] == ' ') value.erase(0, 1);
            headers[key] = value;
        }
    }


// Mostrar encabezados
    for (const auto& header : headers) {
        std::cout << header.first << ": " << header.second << std::endl;
    }


    // Mostrar el encabezado User-Agent
    auto userAgentIt = headers.find("User-Agent");
    if (userAgentIt != headers.end()) {
	std::cout << std::endl << std::endl;   
        std::cout << "User-Agent: " << userAgentIt->second << std::endl;
    } else {
        std::cout << "User-Agent header not found" << std::endl;
    }
}

//------------------------------------------------------------------------





// Configurar el servidor
constexpr int Puerto = 8080;
constexpr int NumeroMaximoClientes = 10;
constexpr int Tama_Buffer = 1024;

void NobloquearServ(int socket){ //configurar el servidor como no bloquente
     int banderas = fcntl(socket,F_GETFL,0);
     fcntl(socket,F_SETFL,banderas|O_NONBLOCK);
}

std::string LeerArchivo(const std::string& archivoRuta){// funcion para leer el contenido de un archivo
     std::ifstream archivo(archivoRuta);
     if(!archivo.is_open()){
        throw std::runtime_error("No se puede abrir el archivo: "+archivoRuta);
     }
     std::stringstream buffer;
     buffer << archivo.rdbuf();
     archivo.close();
     return buffer.str();
}

std::string archivoHtml = LeerArchivo("www/index.html");

const std::string Respuesta_HTML =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "Content-Length: "+std::to_string(archivoHtml.size())+"\r\n"
      "\r\n"+
      archivoHtml;


int main(){
//crear socket
int servidor = socket(AF_INET,SOCK_STREAM,0);
   if(servidor<0){
     perror("Error al crear socket.");
     return 1;
   }else{
     std::cout<<"Se Creo Correctamente el servidor."<<std::endl;
   }

//configurar el socket para utilizar direcciones y no bloquear
   int opt = 1;
   setsockopt(servidor,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
   NobloquearServ(servidor);

//Enlazar el servidor con su direccion y puerto
   sockaddr_in server_addr{};
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY; // Escucha en todas la interfaces locales, 127.0.0.1
   server_addr.sin_port = htons(Puerto);

   if(bind(servidor,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
      perror("Error al enlazar el socket.");
      close(servidor);
      return 1;
   } 

   if(listen(servidor,NumeroMaximoClientes)<0){
      perror("El servidor tuvo un error al ponerse a escuchar.");
      close(servidor);
      return 1;
   }

 


    std::cout<<"El servidor esta iniciando en http://127.0.0.1:"<<Puerto<<std::endl;
    
//Configura el bucle de eventos con poll
    std::vector<pollfd> poll_fds;
    poll_fds.push_back({servidor,POLLIN,0});// Añade el socket del servidor
 

	    while(true){
	         //llama a poll para monitorizar eventos
		 int contador_eventos = poll(poll_fds.data(),poll_fds.size(),-1);
		    if(contador_eventos<0){
		       perror("Error en poll.");
		       break;
		    }
	            
		    //Itera sobre los socket monitprizados
		    for(size_t i=0;i<poll_fds.size();++i){
                       //Si hay una nueva conexion
		       if(poll_fds[i].fd == servidor && (poll_fds[i].revents & POLLIN)){
			   sockaddr_in client_addr{};
		           socklen_t client_len = sizeof(client_addr);
		           int client_fd = accept(servidor,(struct sockaddr*)&client_addr,&client_len);
		           if(client_fd>=0){
		              NobloquearServ(client_fd);
		              poll_fds.push_back({client_fd,POLLIN,0});//añade el cliente a poll	      
			   }
		       }else if(poll_fds[i].revents & POLLIN){ //si hay datos del un clientes existente
		           char buffer[Tama_Buffer];
			   int bytes_leidos = read(poll_fds[i].fd,buffer,sizeof(buffer));
			   if(bytes_leidos > 0){
		             procesarHttp(buffer);//funcion para procesar los datos de navegador de los clientes   		   
			     write(poll_fds[i].fd,Respuesta_HTML.c_str(),Respuesta_HTML.size());
			   }else{//Cerrar conexion si el cliente termino
			     close(poll_fds[i].fd);
			     poll_fds.erase(poll_fds.begin()+i);  
			     --i;//ajusta indice despues de terminar
			   }
                             
		       
		       }


			    
		    }//for
	    
	           
	    }//while
          
     close(servidor);

  return 0;

}

