
# README - E-Shop with Inter-Process Communication

## Project Overview
This project is an implementation of an electronic shop (e-shop) using Internet sockets and Inter-Process Communication (IPC). The e-shop allows clients to place orders for products, and the server processes these orders while managing product availability.

## Features
- **Client-Server Architecture**: The e-shop follows a client-server model using sockets.
- **Multi-client Support**: The server can handle multiple clients making orders.
- **Product Management**: The server maintains a catalog of 20 products with limited stock.
- **Order Processing**: Orders are processed sequentially with a delay of 1 second.
- **Communication via Sockets**: Clients and the server communicate using `read()` and `write()` system calls.
- **Fork-based Client Execution**: Multiple clients are spawned using the `fork()` system call.
- **Randomized Orders**: Clients randomly select products for ordering.
- **Final Report Generation**: The server generates a summary report detailing:
  - Number of purchase requests per product
  - Number of successful sales
  - Users who failed to get their requested products
  - Total number of orders
  - Total number of successful and failed orders
  - Total revenue

## File Structure
- `server.c` - Implements the server logic, manages product catalog, handles client requests.
- `client.c` - Implements client functionality, connects to the server, sends orders.
- `config.h` - Header file containing function declarations and struct definitions.
- `Makefile` - Build script to compile the server and client programs.

## Compilation & Execution
### Compilation
To compile the project, run:
```sh
make
```
This will generate `server` and `client` executables.

### Running the Server
To start the server, execute:
```sh
./server
```
The server initializes the product catalog and waits for client connections.

### Running Clients
To run a client, specify a client number:
```sh
./client <client_number>
```
Each client will place 10 random orders, waiting 1 second between each.

## Dependencies
- GCC compiler
- Linux-based system (for socket programming and `fork()` usage)
