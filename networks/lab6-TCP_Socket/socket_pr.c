#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


int create_socket(){
	// IPv4, TCP connection, only a single protocol supported
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Can not create socket\n");
		exit(-1);
	}
	printf("Socket created...\n");	
	return sock;
}


 void make_address(char* IP, struct sockaddr* addr) {

	struct sockaddr_in addr = {AF_INET, htons(8080)};

	if (strcmp(IP, "")) {
		addr.sin_addr.s_addr = INADDR_ANY;
	} else {
		// IPv4, src, dst
		int s = inet_pton(AF_INET, IP, &addr.sin_addr);
		if (s <= 0) {
			printf("error convering IP address\n");
			exit(-2);
		}
	}


	// remote.sin_family = AF_INET;
	// remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
	// remote.sin_port = htons(ServerPort);


	// remote.sin_family = AF_INET;  Internet address family 
	// remote.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	// remote.sin_port = htons(ClientPort); /* Local port */

	printf("Address converted...\n");

}
const char* client() {
	int sock = create_socket();
	
	string IP = "127.0.0.1";

	// scanf IP

	struct sockaddr* addr; 
	make_address(IP, addr);

	int c = connect(sock, addr, sizeof(addr));
	if (c < 0) {
		printf("connection failed\n");
		exit(-3);
	}
	printf("Connection established...\n");

	char* message = "Hellooo";
	// socket, buffer, length, flags
	send(sock, message, strlen(message), 0);

	printf("message sent\n");
	char* buf = calloc(1024, sizeof(char));
	int valread = read(sock, buf, 1024);
	
	printf("%i %s", valread, buf);
	printf("Message received...\n");

	// return buf;	
}

void server() {
	int sock = create_socket("");


	 // Forcefully attaching socket to the port 8080
	// int opt = 1;
 //    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
 //                                                  &opt, sizeof(opt)) != 0)
 //    {
 //        perror("setsockopt");
 //        exit(EXIT_FAILURE);
 //    }

	struct sockaddr* addr; 
	make_address("", addr);
	
	int b = bind(sock, addr, sizeof(addr));
	if (b < 0) {
		printf("bind failed\n");
		exit(-2);
	}


	int l = listen(sock, 3);
	if (l < 0) {
		printf("listen failed\n");
		exit(-3);
	}
	printf("listening...\n");

	while (true) {
		printf("Waiting for incoming connections...\n");
		clientLen = sizeof(struct sockaddr_in);

		//accept connection from an incoming client
		int a = accept(socket_desc, sock, (socklen_t*)&clientLen);
		if (a < 0){
			perror("accept failed");
			return 1;
		}
		printf("Connection accepted\n");
	}
}




int main(int argc, char *argv[]) {
	if (1 < argc && strcmp(argv[1], "server") == 0) {
		server();
	} else if (1 < argc && strcmp(argv[1], "client") == 0) {
		client();
	} else {
        fprintf(stderr, "Usage: %s server|client\n", argv[0]);
        exit(-1);
	}

}
