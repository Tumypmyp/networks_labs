#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int create_socket() {
	int sock;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket creation error\n");
		exit(2);
	}
	printf("Socket created...\n");
	return sock;
}

struct sockaddr_in make_socket_address(int port, uint32_t s_addr) {
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = s_addr;
	return server;
}

void bind_socket(int sock, struct sockaddr_in addr) {
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("Binding error\n");
		exit(3);
	}
	printf("Binded...\n");
}

struct hostent* convert_host_name(char* name) {
	struct hostent* hostnm = gethostbyname(name);
	if (hostnm == (struct hostent *) 0) {
		fprintf(stderr, "Gethostbyname failed\n");
		exit(2);
	}
	return hostnm;
}


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s recieve_port [forward_server forward_port]\n", argv[0]);
		exit(1);
	}

	int sock = create_socket();


	int port = atoi(argv[1]);

	struct sockaddr_in server_address = make_socket_address(port, INADDR_ANY);

	bind_socket(sock, server_address);

	// Listen for connections
	if (listen(sock, 1) != 0) {
		printf("Listening error\n");
		exit(4);
	}
	printf("Listening...\n");

	// Accept a connection. 
	struct sockaddr_in client; 
	int nsock;
	int namelen = sizeof(client);
	if ((nsock = accept(sock, (struct sockaddr *)&client, &namelen)) == -1) {
		printf("Accepting connection error\n");
		exit(5);
	}
	printf("Connection acceped...\n");
	
	// Receive the message
	char buf[12] = {};

	if (recv(nsock, buf, sizeof(buf), 0) == -1) {
		printf("Receiving error\n");
		exit(6);
	}
	printf("Message recieved: %s\n", buf);
	

	// Closing sockets
	if (close(nsock) < 0) {
		printf("Client socket can not be closed\n");
		exit(8);
	}
	printf("Client socket closed...\n");

	if (close(sock) < 0) {
		printf("Server socket can not be closed\n");
		exit(9);
	}
	printf("Server socket closed...\n");

	printf("Server executed successfully.\n\n");
	if (argc >= 4) {
		printf("Now sending to the next server...\n");
		char* args[] = {"./client.o", argv[2], argv[3], buf, NULL};
		execvp(args[0], args);
	}
	return 0;
}

