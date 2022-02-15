#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int create_socket() {
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Socket creation error\n");
		exit(2);
	}
	printf("Socket created...\n");
	return sock;
}

struct hostent* convert_host_name(char* name) {
	struct hostent* hostnm = gethostbyname(name);
	if (hostnm == (struct hostent *) 0) {
		fprintf(stderr, "Gethostbyname failed\n");
		exit(2);
	}
	return hostnm;
}

struct sockaddr_in make_socket_address(int port, uint32_t s_addr) {
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = s_addr;
	return server;
}


int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s hostname port\n", argv[0]);
		exit(1);
	}

	int sock = create_socket();


	struct hostent *host_name = convert_host_name(argv[1]);  
	
	int port = atoi(argv[2]);

	struct sockaddr_in server = make_socket_address(port, *((unsigned long *)host_name->h_addr));



	char buf[BUFFER_SIZE] = {};
	scanf("%s", buf);

	int namelen = sizeof(server);
	if (sendto(sock, buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *) &server, namelen) < 0) {
		printf("Sending error\n");
		exit(3);
	}
	// printf("Message sent:\n%s\n", buf);



	// struct sockaddr_in client;
	int n;
	if ((n = recvfrom(sock, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr*) &server, &namelen)) < 0) {
		printf("Receiving error\n");
		exit(6);
	}
	printf("\nChat:\n%s\n", buf);
	// buffer[n] = '\0';

	return 0;      
}