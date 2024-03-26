#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(struct sockaddr_in);
    char buffer[1024] = {0};
    const  char* message = "Messaggio ricevuto dal server";

    // Inizializza Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Errore durante l'inizializzazione di Winsock\n");
        return 1;
    }

    // Crea il socket del server
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Errore durante la creazione del socket\n");
        return 1;
    }

    // Imposta gli indirizzi del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Associa il socket del server all'indirizzo e alla porta specificati
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind fallito\n");
        return 1;
    }

    // Mette in ascolto il socket per le connessioni in arrivo
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen fallito\n");
        return 1;
    }

    printf("Server in ascolto sulla porta %d...\n", PORT);

    // Accetta una connessione in arrivo
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) == INVALID_SOCKET) {
        printf("Accettazione della connessione fallita\n");
        return 1;
    }

    // Legge il messaggio inviato dal client
    recv(client_socket, buffer, 1024, 0);
    printf("Messaggio ricevuto dal client: %s\n", buffer);

    // Invia un messaggio di conferma al client
    send(client_socket, message, strlen(message), 0);
    printf("Messaggio inviato al client: %s\n", message);

    closesocket(server_socket);
    closesocket(client_socket);
    WSACleanup();
    return 0;
}
