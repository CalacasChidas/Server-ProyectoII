// Servidor.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
int suma(int a, int b) {
    return a + b;
}
int main() {
    // Crear un socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket del servidor" << std::endl;
        return 1;
    }

    // Configurar la dirección del servidor
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Puerto de escucha
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Vincular el socket a la dirección
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error al vincular el socket a la dirección" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error al escuchar conexiones entrantes" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Servidor esperando conexiones entrantes..." << std::endl;

    while (true) {
        // Aceptar una conexión entrante
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error al aceptar la conexión" << std::endl;
            continue; // Continuar esperando conexiones
        }

        std::cout << "Cliente conectado" << std::endl;
//ADS

        while (true) {
            // Recibir la palabra clave del cliente
            char word[5] = {0};
            int bytesRead = recv(clientSocket, word, sizeof(word), 0);
            if (bytesRead != 4 || strcmp(word, "suma") != 0) {
                std::cerr << "Palabra clave incorrecta o error al recibir" << std::endl;
                break;
            }

// Recibir el primer número del cliente
            char num1[1024] = {0};
            bytesRead = recv(clientSocket, num1, sizeof(num1), 0);
            if (bytesRead <= 0) {
                std::cerr << "Error al recibir el primer número del cliente" << std::endl;
                break;
            }

// Recibir el segundo número del cliente
            char num2[1024] = {0};
            bytesRead = recv(clientSocket, num2, sizeof(num2), 0);
            if (bytesRead <= 0) {
                std::cerr << "Error al recibir el segundo número del cliente" << std::endl;
                break;
            }

// Convierte los números a enteros
            int a = std::stoi(num1);
            int b = std::stoi(num2);

// Calcular la suma
            int result = suma(a, b);

// Enviar el resultado al cliente
            send(clientSocket, &result, sizeof(result), 0);
        }

        // Cerrar el socket del cliente
        close(clientSocket);
    }


}


