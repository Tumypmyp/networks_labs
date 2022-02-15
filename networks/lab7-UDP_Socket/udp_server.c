#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<time.h>

#define CHAT_SIZE 100
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



// Store the formatted string of time in the output
void format_time(time_t rawtime, char *output){
	// time_t rawtime;
	struct tm * timeinfo;
	
	// time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	sprintf(output, "[%d %d %d:%d:%d]", timeinfo->tm_mday, timeinfo->tm_mon + 1,
			timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

struct Message {
	struct sockaddr_in addr_in;
	char* text;
	time_t time;
};


struct Message chat[CHAT_SIZE];

struct Message message(int n, char *buf, struct sockaddr_in addr) {
	struct Message m;
	
	m.text = strndup(buf, n);
	m.addr_in = addr;
	time(&m.time);

	return m;
}

void message_to_string(struct Message m, char* text) {
	char *ip = inet_ntoa(m.addr_in.sin_addr);
	int port = htons(m.addr_in.sin_port);
	char t[30];
	format_time(m.time, t);

	sprintf(text, "%s %s:%d %s\n", t, ip, port, m.text);
}

void get_all_messages(char* buf) {
	char* t = buf;
	for (int i = 0; i < CHAT_SIZE && chat[i].text != NULL; i++) {
		char text[BUFFER_SIZE];
		message_to_string(chat[i], text);
		int len = strlen(text);

		strcpy(t, text);
		t += len;
	}
}



int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s port\n", argv[0]);
		exit(1);
	}

	int sock = create_socket();

	int port = atoi(argv[1]);

	struct sockaddr_in server_address = make_socket_address(port, INADDR_ANY);

	bind_socket(sock, server_address);

	printf("server started\n");

	char buf[BUFFER_SIZE] = {};
	int i = 0;
	while (1) {
		struct sockaddr_in client;
		int namelen = sizeof(client), n;
		memset(buf, '\0', BUFFER_SIZE);
		if ((n = recvfrom(sock, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr*) &client, &namelen)) < 0) {
			printf("Receiving error\n");
			exit(6);
		}
		printf("\nMessage recieved:\n%s\n", buf);

		chat[i] = message(n, buf, client);
		i++;

		memset(buf, '\0', BUFFER_SIZE);
		get_all_messages(buf);
		if (sendto(sock, buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *) &client, namelen) < 0) {
			printf("Sending error\n");
			exit(7);
		}
		printf("Chat updated:\n%s\n", buf);

	}
	return 0;      
}