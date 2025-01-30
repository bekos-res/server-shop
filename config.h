#ifndef CONFIG_H
#define CONFIG_H

// Constants
#define NUM_ORDERS      10	// Orders per customer
#define PRODUCT_COUNT   20	// Total products in catalog
#define NUM_CUSTOMERS    5	// Total customers
#define SERVER_PORT  8080	// Port for socket communication

// Product structure (used by server)
typedef struct {
	char description[50];	// Product name
	float price;		// Price per unit
	int item_count;		// Available stock
	int attempt_count;	// Total order attempts
	int sold_count;		// Successful sales
	int failed_customers[NUM_CUSTOMERS]; // Failed orders per customer
} Product;

// Global catalog (defined in server.c)
extern Product catalog[PRODUCT_COUNT];

// Server functions
void initialise_catalog();	// Initialize product data

#endif
