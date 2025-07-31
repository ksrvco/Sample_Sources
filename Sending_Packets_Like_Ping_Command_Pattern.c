// Sending Packets to any host and any port number using ping command pattern.
// Like:  ping google.com   >>>> This command sending number of 30 packets to port 443 of google.com.
// Number of 30 packets and port 443 and payload message can be modified in source code.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#define PORT 443
#define NUM_PACKETS 30

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *hostname = argv[1];
    struct hostent *host;
    struct sockaddr_in server_addr;
    int sock;
    char *message = "Hello, this is a TCP packet";

    // Resolve hostname to IP address
    host = gethostbyname(hostname);
    if (host == NULL) {
        fprintf(stderr, "Error: Could not resolve hostname.\n");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr, host->h_addr, host->h_length);

    for (int i = 0; i < NUM_PACKETS; i++) {
        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }

        // Connect to the server
        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection failed");
            close(sock);
            continue;
        }

        // Send the message
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Send failed");
        } else {
            printf("Packet %d sent to %s:%d\n", i + 1, hostname, PORT);
        }

        // Close the socket after each packet
        close(sock);
    }

    return 0;
}
