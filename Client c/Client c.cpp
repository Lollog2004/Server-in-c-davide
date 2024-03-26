#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h> // Includi questa libreria per inet_pton

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    const  char *message = "Ciao, server!";

    // Inizializza Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Errore durante l'inizializzazione di Winsock\n");
        return 1;
    }

    // Crea il socket del client
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Errore durante la creazione del socket\n");
        return 1;
    }

    // Imposta gli indirizzi del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Converti l'indirizzo IP del server da stringa a formato binario
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printf("Indirizzo IP non valido\n");
        return 1;
    }

    // Connessione al server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connessione al server fallita\n");
        return 1;
    }

    // Invia un messaggio al server
    send(client_socket, message, strlen(message), 0);
    printf("Messaggio inviato al server: %s\n", message);

    // Ricevi la risposta dal server
    char buffer[1024] = { 0 };
    recv(client_socket, buffer, 1024, 0);
    printf("Risposta dal server: %s\n", buffer);

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
