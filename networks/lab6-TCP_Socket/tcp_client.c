#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	if (argc < 3) {
		printf("Usage: %s hostname port [message]\n", argv[0]);
		exit(1);
	}

	int sock = create_socket();
	

	struct hostent *host_name = convert_host_name(argv[1]);  

	int port = atoi(argv[2]);

	struct sockaddr_in server = make_socket_address(port, *((unsigned long *)host_name->h_addr));
	
	// Connect to the server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Connect error\n");
		exit(4);
	}
	printf("Connected...\n");


	// Make message
	char buf[12];
	if (3 < argc) {
		strcpy(buf, argv[3]);
	} else {
		strcpy(buf, "abcd efgh");
	}

	// Send message
	if (send(sock, buf, sizeof(buf), 0) < 0) {
		printf("Sending error\n");
		exit(5);
	}
	printf("Message sent: %s\n", buf);


	// // Receive message
	// if (recv(sock, buf, sizeof(buf), 0) < 0) {
	// 	printf("Receiving error\n");
	// 	exit(6);
	// }
	// printf("Server reply: %s\n", buf);

	
	// Close socket
	if (close(sock) < 0) {
		printf("socket can not be closed\n");
		exit(7);
	}
	printf("Socket closed...\n");
	
	printf("Client executed successfully...\n");
	return 0;
}

