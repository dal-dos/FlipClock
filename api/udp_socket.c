// udp_socket.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>	
#include <arpa/inet.h>

#include "udp_socket.h"
#include "request_handler.h"
#include "utils.h"

#define MSG_MAX_LEN 1500
#define PORT        12345

static pthread_t listening_thread;
static bool running = false;

struct sockaddr_in sin1;
int socket_descriptor;

struct sockaddr_in default_remote_addr; // web client

// thread helper, waits for messages
static void* start_listening(void* arg) {
    while (running) {
		struct sockaddr_in sin_remote;
		unsigned int sin_len = sizeof(sin_remote);
		char message_rx[MSG_MAX_LEN];

		int bytes_rx = recvfrom(socket_descriptor,
			message_rx, MSG_MAX_LEN - 1, 0,
			(struct sockaddr *) &sin_remote, &sin_len);

		message_rx[bytes_rx] = 0;
        printf("[Received]: %s\n", message_rx);

        char message_tx[MSG_MAX_LEN];
        Handler_make_request(message_rx, message_tx);
        UDP_send(message_tx, &sin_remote);

        if (strncmp(message_rx, "stop", strlen("stop")) == 0) { break; }
    }
    return NULL;
}

void UDP_start_listening(int port) {
    if (!running) {
        memset(&sin1, 0, sizeof(sin1));
        sin1.sin_family = AF_INET;
        sin1.sin_addr.s_addr = htonl(INADDR_ANY);
        sin1.sin_port = htons(port);

        int remote_addr_port = 12312;
        char remote_addr_address[] = "0.0.0.0";
        default_remote_addr.sin_family = AF_INET;
        default_remote_addr.sin_addr.s_addr = inet_addr(remote_addr_address);
        default_remote_addr.sin_port = htons(remote_addr_port);

        // Create the socket for UDP
        socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);

        // Bind the socket to the port (PORT) that we specify
        bind(socket_descriptor, (struct sockaddr*) &sin1, sizeof(sin1));
        running = true;
        pthread_create(&listening_thread, NULL, start_listening, NULL);
    }
}

void UDP_stop_listening() {
    running = false;
    pthread_join(listening_thread, NULL);
}

void UDP_send(char* http_message, struct sockaddr_in *sin_remote) {
    sendto(socket_descriptor,
        http_message, strlen(http_message),
        0, (struct sockaddr *)sin_remote, sizeof(*sin_remote));
}

void UDP_send_default(char* http_message) {
    sendto(socket_descriptor,
        http_message, strlen(http_message),
        0, (struct sockaddr *)&default_remote_addr, sizeof(default_remote_addr));
}

int UDP_get_msg_max_len(void) {
    return MSG_MAX_LEN;
}
