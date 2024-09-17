#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main () {
	int server_fd;
	int new_socket;

	struct sockaddr_in address;

	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	// HTTP response with headers and HTML content
	const char *http_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 46\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html><body><h1>Hello from server!</h1></body></html>";

	// Creating socket file descriptor
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {
		std::cerr << "Socket Failed!" << std::endl;
		return (-1);
	}

	// Attaching socket to the port 8080
	if (!setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		std::cerr << "Setsockopt failed!" << std::endl;
		close(server_fd);
		return (-1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Binding socket to the address
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cerr << "Bind failed!" << std::endl;
		close(server_fd);
		return (-1);
	}

	// Listening for incoming connections
	if (listen(server_fd, 3) < 0) {
		std::cerr << "Listen failed!" << std::endl;
		close(server_fd);
		return -1;
	}

	// Accepting an incoming connection
	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_socket < 0){
		std::cerr << "Accept failed!" << std::endl;
		close(server_fd);
		return -1;
	}

	// Reading from the client
	read(new_socket, buffer, 1024);
	std::cout << "Message received: " << buffer << std::endl;

	// Sending a response to the client
	send(new_socket, http_response, strlen(http_response), 0);
	std::cout << "HTTP response sent"<< std::endl;

	// Closing the socket
	close(new_socket);
	close(server_fd);
	return (0);
}