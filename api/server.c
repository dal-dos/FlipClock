// server.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

const unsigned int MSG_MAX_LEN = 1500;

bool running = false;

struct sockaddr_in sin;
int socket_descriptor;

// thread helper, waits for messages
static void* start_listening(void* arg) {
    while (running) {
		struct sockaddr_in sin_remote;
		unsigned int sin_len = sizeof(sin_remote);
		char message_rx[MSG_MAX_LEN];

		// Pass buffer size - 1 for max # bytes so room for the null (string data)
		int bytes_rx = recvfrom(socket_descriptor,
			message_rx, MSG_MAX_LEN - 1, 0,
			(struct sockaddr *) &sin_remote, &sin_len);

		message_rx[bytes_rx] = 0;

        // do something
    }
}

void Server_start_server(int port) {
    if (!running) {
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;                   // Connection may be from network
        sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
        sin.sin_port = htons(port);                 // Host to Network short

        // Create the socket for UDP
        int socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);

        // Bind the socket to the port (PORT) that we specify
        bind(socket_descriptor, (struct sockaddr*) &sin, sizeof(sin));
    }
}

void Server_stop_server() {}

void Server_send(char* http_message, struct sockaddr *sin_remote) {
    sendto(socket_descriptor,
        http_message, strlen(http_message),
        0, sin_remote, sizeof(*sin_remote));
}
