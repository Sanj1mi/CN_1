#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#include"globals.h"
#include"exception.h"
#include"logger.h"

std::string get_valid_input() {						// input validation(for empty, too long,...)
	std::string user_input_;						// user inupt string

	bool is_input_exit = false;						// running until coorect, or command

	while (!is_input_exit) {
		std::cout << crgb::GRAY << "input: " << crgb::RESET;

		if (!std::getline(std::cin, user_input_)) { // validate getline
			throw InvalidInputException("input stream erro");
		}

		if (user_input_.empty()) {					// throw when empty
			log_error("input validation","empty input not allowed");
		}
		else if (user_input_.length() > 512) {		// too long (max: 512 chars)
			log_error("input validation", "input too long(max: 512)");
		}
		else {										// if OK -> return
			is_input_exit = true;
		}
	}
	return user_input_;								// return string
}

void clear_input_buffer() {							// same like server(clear input buffer)
	std::cin.clear();
	do {
		cin.ignore();
	} while (cin.get() != '\n');
}


void initialize_winsock() {							// func: init winsock [necessarily before work][!!!]
	WSADATA wsa_data_{};							// socket struct
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data_) != 0) { // init version - 2.2
		throw WinsockException("WSAStartup failed", WSAGetLastError());
	}
	log_client("winsock initialized", "successfully (^^)");
}

SOCKET create_client_socket() {						// func: creating client socket
	SOCKET client_socket_ = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
	if (client_socket_ == INVALID_SOCKET) {			// validation
		throw SocketCreationException("socket() failed(_-_)", WSAGetLastError());
	}
	log_client("socket initialized", "successfully (^^)");
	return client_socket_;							// descriptor of new socket
}

void connect_to_server(SOCKET client_socket_) {		// func: connect to current server(by IP, PORT)
	sockaddr_in server_address_{};					// address
	server_address_.sin_family = AF_INET;			// IP
	server_address_.sin_port = htons(port_);		// port
	server_address_.sin_addr.s_addr = inet_addr(ip_address_); // IP address

	/* try connection */
	if (connect(client_socket_, reinterpret_cast<sockaddr*>(&server_address_), sizeof(server_address_)) == SOCKET_ERROR) {
		throw ConnectionFailedException("connect() failed (-_-)", WSAGetLastError());
	}
	log_client("connected to server",ip_address_ + ' ' + ':' + std::to_string(port_));
}

void send_message(SOCKET client_socket_, const std::string& message_) { // func: send message to server
	int result_ = send(client_socket_, message_.c_str(), // send data by socket
		static_cast<int>(message_.size()), 0);
	if (result_ == SOCKET_ERROR) {					// validation
		throw SocketException("send() failed", WSAGetLastError());
	}
}

std::string receive_message(SOCKET client_socket_) {// get server answer
	char response_buffer_[512]{};					// buffer, than get
	int bytes_received_ = recv(client_socket_, response_buffer_, sizeof(response_buffer_) - 1, 0);
	if (bytes_received_ <= 0) {						// error, or brake of connection
		throw ConnectionLostException("receive error", WSAGetLastError());
	}

	response_buffer_[bytes_received_] = '\0';		// protection (&-&)
	return std::string(response_buffer_);			// -> to_string 
}

void client_loop(SOCKET client_socket_) {			// func: main cycle to interract with server
	bool is_exit_ = false;							// main flag

	while (!is_exit_) {
		try {
			std::string user_input_ = get_valid_input(); // validate input
			if (user_input_ == "clear") {			// commands
				system("cls");
				log_client("console statement", "cleared");
			} else if (user_input_ == "exit") {
				send_message(client_socket_, user_input_);
				log_client("exiting", "");
				is_exit_ = true;
			} 
			else {
				send_message(client_socket_, user_input_);
				std::string response = receive_message(client_socket_);
				log_client("server reply", response);
			}
		}											// catch exceptions
		catch (const InvalidInputException& exception_) {	
			log_error("input error", exception_.what());
			clear_input_buffer();
		}
		catch (const ConnectionLostException& exception_) {
			log_error("connection", "server disconnected");
			is_exit_ = true;
		}
		catch (const std::exception& exception_) {
				log_error("client loop", exception_.what());
				is_exit_ = true;
		}
		
	}
}

int main() {
	enable_ansi();									// eanble ansi
	SOCKET client_socket_ = INVALID_SOCKET;			// init client socket

	try {											
		initialize_winsock();						// try init winsock
		client_socket_ = create_client_socket();	// client socket
		connect_to_server(client_socket_);			// connection
		client_loop(client_socket_);				// running
	}												// exceptions
	catch (const NetworkException& exception_) {
		log_error("network error", exception_.what(), exception_.get_error_code());
	}
	catch (const std::exception& exception_) {
		log_error("unexpected error", exception_.what());
	}

	if (client_socket_ != INVALID_SOCKET) {
		closesocket(client_socket_);
		log_client("socket closed", "cleanup completed (^-_)");
	}
	WSACleanup();									// clean [necessarily][!!!]
	log_client("application", "shutdown completed (^^)");

	return 0;
}