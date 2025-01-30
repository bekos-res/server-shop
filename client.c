#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config.h"

#define PORT 8080

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <customer_id>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int customer_id = atoi(argv[1]);
	int sock = 0;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket creation error");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	srand(time(NULL) ^ (getpid() << 16));
	float total_spent = 0.0;
	int success_counter = 0;

	for (int i = 0; i < NUM_ORDERS; i++) {
		sleep(1);
		int product_index = rand() % PRODUCT_COUNT;
		write(sock, &product_index, sizeof(int));
		write(sock, &customer_id, sizeof(int));

		float price;
		char description[50];
		read(sock, &price, sizeof(float));
		read(sock, description, sizeof(description));

		if (price == -1) {
			printf("Customer #%d: %s is out of stock.\n", customer_id, description);
		} else {
			printf("Customer #%d: Bought %s for %.2f EUR.\n", customer_id, description, price);
			total_spent += price;
			success_counter++;
		}
	}

	printf("Customer #%d: Total spent: %.2f EUR (%d/%d successful)\n", 
		customer_id + 1, total_spent, success_counter, NUM_ORDERS);
	close(sock);
	sleep(1);
	return 0;
}
