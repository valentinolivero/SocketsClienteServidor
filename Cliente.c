#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_LEN 50

void menu();

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[MAX_LEN] = {0};
    char response[MAX_LEN] = {0};
    int option, length;


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
        printf("Direccion invalida / Direccion no soportada\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }


    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Conexion fallida: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (1) {

        menu();
        if (scanf("%d", &option) != 1) {
            printf("Entrada invalida. Intentelo de nuevo.\n");
            while (getchar() != '\n');
            continue;
        }

        if (option == 1) {
            printf("Ingrese la longitud del nombre de usuario (5-15): ");
            if (scanf("%d", &length) != 1) {
                printf("Entrada invalida. Intentelo de nuevo.\n");
                while (getchar() != '\n');
                continue;
            }
            snprintf(buffer, MAX_LEN, "U%d", length);
        } else if (option == 2) {
            printf("Ingrese la longitud de la contrasenia (8-50): ");
            if (scanf("%d", &length) != 1) {
                printf("Entrada invalida. Intentelo de nuevo.\n");
                while (getchar() != '\n');
                continue;
            }
            snprintf(buffer, MAX_LEN, "P%d", length);
        } else {
            printf("Opcion invalida. Intentelo de nuevo.\n");
            continue;
        }


        send(sock, buffer, strlen(buffer), 0);


        int valread = recv(sock, response, MAX_LEN, 0);
        if (valread > 0) {
            response[valread] = '\0';
            printf("Respuesta del servidor: %s\n", response);
        } else {
            printf("Error al recibir la respuesta del servidor\n");
        }


        char another;
        printf("¿Desea hacer otra solicitud? (s/n): ");
        if (scanf(" %c", &another) != 1) {
            printf("Entrada invalida. Intentelo de nuevo.\n");
            continue;
        }
        if (another != 's' && another != 'S') {
            break;
        }
    }


    closesocket(sock);
    WSACleanup();
    return 0;
}

void menu() {
    printf("Menu:\n");
    printf("1. Generar nombre de usuario\n");
    printf("2. Generar clave\n");
    printf("Ingrese una opcion: ");
}
