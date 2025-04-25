#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 8080

void run_tcp_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Crear socket TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear el socket TCP");
        exit(EXIT_FAILURE);
    }

    // Configurar opciones del socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Error en setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(DEFAULT_PORT);

    // Vincular socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor TCP esperando conexiones en el puerto " << DEFAULT_PORT << "..." << std::endl;

    // Aceptar conexión entrante
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Error en accept");
        exit(EXIT_FAILURE);
    }

    std::cout << "Cliente conectado desde " << inet_ntoa(address.sin_addr) << std::endl;

    // Leer datos del cliente
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    std::cout << "Mensaje recibido: " << buffer << std::endl;

    // Responder al cliente
    const char *response = "Mensaje recibido por el servidor TCP";
    send(new_socket, response, strlen(response), 0);
    std::cout << "Respuesta enviada al cliente." << std::endl;

    close(new_socket);
    close(server_fd);
}

void run_udp_server() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];

    // Crear socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket UDP");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(DEFAULT_PORT);

    // Vincular socket al puerto
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error en bind UDP");
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor UDP esperando datagramas en el puerto " << DEFAULT_PORT << "..." << std::endl;

    socklen_t len = sizeof(cliaddr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    std::cout << "Mensaje recibido: " << buffer << std::endl;

    const char *response = "Mensaje recibido por el servidor UDP";
    sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&cliaddr, len);
    std::cout << "Respuesta enviada al cliente." << std::endl;

    close(sockfd);
}

void run_tcp_client(const char *server_ip) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket TCP");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);

    // Convertir dirección IP a formato binario
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("Dirección IP inválida");
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error en connect");
        exit(EXIT_FAILURE);
    }

    const char *message = "Hola, servidor TCP!";
    
    send(sock, message, strlen(message), 0);
    std::cout << "Mensaje enviado al servidor TCP." << std::endl;

    int valread = read(sock, buffer, BUFFER_SIZE);
    std::cout << "Respuesta del servidor: " << buffer << std::endl;

    close(sock);
}

void run_udp_client(const char *server_ip) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket UDP");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(DEFAULT_PORT);

    if (inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0) {
        perror("Dirección IP inválida");
        exit(EXIT_FAILURE);
    }

    const char *message = "Hola, servidor UDP!";
    sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    std::cout << "Mensaje enviado al servidor UDP." << std::endl;

    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    std::cout << "Respuesta del servidor: " << buffer << std::endl;

    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <modo> <protocolo> [<ip_servidor>]" << std::endl;
        std::cerr << "  modo: server | client" << std::endl;
        std::cerr << "  protocolo: tcp | udp" << std::endl;
        std::cerr << "  ip_servidor: Obligatorio si modo=client" << std::endl;
        return 1;
    }

    std::string modo = argv[1];
    std::string protocolo = argv[2];

    if (modo == "server") {
        if (protocolo == "tcp") {
            run_tcp_server();
        } else if (protocolo == "udp") {
            run_udp_server();
        } else {
            std::cerr << "Protocolo no válido. Use 'tcp' o 'udp'." << std::endl;
            return 1;
        }
    } else if (modo == "client") {
        if (argc < 4) {
            std::cerr << "Debe proporcionar la IP del servidor." << std::endl;
            return 1;
        }
        const char *server_ip = argv[3];
        if (protocolo == "tcp") {
            run_tcp_client(server_ip);
        } else if (protocolo == "udp") {
            run_udp_client(server_ip);
        } else {
            std::cerr << "Protocolo no válido. Use 'tcp' o 'udp'." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Modo no válido. Use 'server' o 'client'." << std::endl;
        return 1;
    }

    return 0;
}
