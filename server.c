#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "config.h"

#define PORT 8080

Product catalog[PRODUCT_COUNT];	// Define the catalog array

void initialise_catalog() {
	for (int i = 0; i < PRODUCT_COUNT; i++) {
		snprintf(catalog[i].description, sizeof(catalog[i].description), "Product %d", i + 1);
		catalog[i].price = 1.0f + ((rand() % 91) / 10.0);	// Random price between 1 and 10
		catalog[i].item_count = 2;	// Default: 2 items in stock
		catalog[i].attempt_count = 0;	// No attempts initially
		catalog[i].sold_count = 0;	// No sails initially
		for (int j = 0; j < NUM_CUSTOMERS; j++) {
			catalog[i].failed_customers[j] = 0;	// No customer has failed an order on the item initially
		}
	}
}

int main() {
	initialise_catalog();
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Create socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Set socket options
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Bind
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Listen
	if (listen(server_fd, 5) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < NUM_CUSTOMERS; i++) {
		printf("Waiting for customer %d...\n", i+1);
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		int customer_id;
		for (int j = 0; j < NUM_ORDERS; j++) {
			int product_index;
			read(new_socket, &product_index, sizeof(int));
			read(new_socket, &customer_id, sizeof(int));

			// Process order
			float price;
			char description[50];
			if (product_index >= 0 && product_index < PRODUCT_COUNT) {
				catalog[product_index].attempt_count++;
				if (catalog[product_index].item_count > 0) {
					catalog[product_index].item_count--;
					catalog[product_index].sold_count++;
					price = catalog[product_index].price;
				} else {
					catalog[product_index].failed_customers[customer_id]++;
					price = -1;
				}
				strcpy(description, catalog[product_index].description);
			} else {
				price = -1;
				strcpy(description, "Invalid product");
			}

			write(new_socket, &price, sizeof(float));
			write(new_socket, description, sizeof(description));
			usleep(500000); // 0.5-second processing time
		}
		close(new_socket);
	}

	// Generate report (from original main.c)
	printf("\n\nFinal Report:\n");
	int total_attempts = 0, total_success = 0;
	float total_revenue = 0.0f;
	for (int i = 0; i < PRODUCT_COUNT; i++) {
		printf("\nProduct: %s\n", catalog[i].description);
		printf("  Attempts: %d\n", catalog[i].attempt_count);
		printf("  Sold: %d\n", catalog[i].sold_count);
		for (int j = 0; j < NUM_CUSTOMERS; j++) {
			if (catalog[i].failed_customers[j] > 0) {
				printf("  Customer %d failed %d times\n", j+1, catalog[i].failed_customers[j]);
			}
		}
		total_attempts += catalog[i].attempt_count;
		total_success += catalog[i].sold_count;
		total_revenue += catalog[i].sold_count * catalog[i].price;
	}
	printf("\nTotal Orders: %d\nSuccessful: %d\nRevenue: %.2f EUR\n", total_attempts, total_success, total_revenue);

	close(server_fd);
	return 0;
}
