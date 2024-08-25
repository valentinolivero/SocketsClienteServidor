#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_LEN 50

void generate_username(char *username, int length);
void generate_password(char *password, int length);

int main() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_LEN] = {0};
    char response[MAX_LEN] = {0};


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }


    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == SOCKET_ERROR) {
        printf("setsockopt failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }


    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    srand(time(0));

    while (1) {
        printf("Esperando conexiones...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
            printf("Accept failed: %d\n", WSAGetLastError());
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        int bytes_received = recv(new_socket, buffer, MAX_LEN, 0);
        if (bytes_received == SOCKET_ERROR) {
            printf("Error al recibir datos: %d\n", WSAGetLastError());
            strcpy(response, "Error: No se recibieron datos.");
            send(new_socket, response, strlen(response), 0);
            closesocket(new_socket);
            continue;
        }

        buffer[bytes_received] = '\0';

        int length = atoi(buffer + 1);

        if (buffer[0] == 'U') {
            if (length < 5 || length > 15) {
                strcpy(response, "Error: Longitud de nombre de usuario invalida.");
            } else {
                generate_username(response, length);
            }
        } else if (buffer[0] == 'P') {
            if (length < 8 || length > 50) {
                strcpy(response, "Error: Longitud de contrasenia invalida.");
            } else {
                generate_password(response, length);
            }
        } else {
            strcpy(response, "Error: Opcion invalida.");
        }

        send(new_socket, response, strlen(response), 0);
        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}

void generate_username(char *username, int length) {
    const char *vowels = "aeiou";
    const char *consonants = "bcdfghjklmnpqrstvwxyz";
    int is_vowel = rand() % 2;

    for (int i = 0; i < length; i++) {
        if (is_vowel) {
            username[i] = vowels[rand() % 5];
        } else {
            username[i] = consonants[rand() % 21];
        }
        is_vowel = !is_vowel;
    }
    username[length] = '\0';
}

void generate_password(char *password, int length) {
    const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length; i++) {
        password[i] = chars[rand() % 62];
    }
    password[length] = '\0';
}
