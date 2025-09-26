#define _WINSOCK_DEPRECATED_NO_WARNINGS				// unable old(irreevant) functions
#include <winsock2.h>								// windows socket API (win dependenicy, not recommended for cross-platform)
#include <iostream>									// basic iostream(_-_)
#include <thread>									// threads lib							
#pragma comment(lib, "ws2_32.lib")					// auto linking with Winsock

#include"symbol_filter.h"							// symbol filter
#include"globals.h"									// global varible's
#include"exception.h"								// custom exception's
#include"logger.h"									// logging + ANSI secuences

void clear_input_buffer() {							// func: clear input buffer
	std::cin.clear();								// clear input stream
	do {									
		cin.ignore();								// ignore symbol's
	} while (cin.get() != '\n');					// until new row [Ret]
}

void process_server_command(const std::string& command_) {	// commands for terminal
	if (command_ == "help" || command_ == "?") {			// if command -> help, show all command list
		std::cout << crgb::GRAY << "[" << crgb::GREEN << "Server commands" << crgb::GRAY << ']' << crgb::RESET << endl;
		log_utils("help, ?", "show this message (^^)");
		log_utils("status", "server status");
		log_utils("clients", "connected clients");
		log_utils("port", "current port");
		log_utils("clear", "clear console");
		log_utils("stop", "stop server gracefully");
		log_utils("exit", "emergency stop");
	}
	else if (command_ == "status") {				 
		log_server("status", "Server is " + std::string(is_server_run_ ? "RUNNING" : "STOPPED"));
		log_server("port", std::to_string(port_));
		log_server("clients", "1 connected");		// TODO: add client counter
	}
	else if (command_ == "clients") {
		log_server("connected clients", "1 active");// TODO: dynamic counter
	}
	else if (command_ == "port") {
		log_server("current port", std::to_string(port_));
	}
	else if (command_ == "clear") {
		system("cls");
		log_server("console", "cleared");
	}
	else if (command_ == "stop") {
		log_server("command", "graceful shutdown initiated...");
		is_server_run_ = false;
	}
	else if (command_ == "exit") {
		log_server("command", "EMERGENCY STOP");
		is_server_run_ = false;
	}
	else {
		throw UnknownCommandException(command_);
	}
}

void validate_server_input(const std::string& input) {	// validate command input(or throw exception)
	if (input.empty()) {							// if input - empty
		throw InvalidInputException("empty command");	// throw
	}
}

void handle_client(SOCKET client_socket_) {			// func: client connection processing
	log_server("client handler started", "new connection");	// log: client processing

	while (is_client_active_ && is_server_run_) {	// main cycle
		try {										
			char input_buffer_[512]{};				// buffer to get data from client
			int bytes_received_ = recv(client_socket_, input_buffer_, sizeof(input_buffer_) - 1, 0);
													// get client request
			if (bytes_received_ <= 0) {				// if something went wrong
				is_client_active_ = false;			// turb of connection
			}
			else {									
				input_buffer_[bytes_received_] = '\0';	// protection move (^^)
				std::string recived_message_ = input_buffer_; // -> to_string 

				if (recived_message_ == "exit") {	// command validation
					is_client_active_ = false;		// exit
				}
				else {								// filter string
					std::string response_ = replace_foutrh_symbol_(input_buffer_, 4, '%');
					send(client_socket_, response_.c_str(), static_cast<int>(response_.size()), 0);
													// send response to client
					log_server("received", recived_message_); // log: receive 
					log_server("sent ", response_);	// log: response
				}
			}
		} catch (const std::exception& error_) {	// catch exceptions
			log_error("client handler", error_.what());
			is_client_active_ = false;
		}
	}
	closesocket(client_socket_);					// close client socket
	log_server("client handler", "finished");		// log: finishing
}


void server_loop() {								// func: main server thread
	try {
		WSADATA wsa_;								// wincosk
		if (WSAStartup(MAKEWORD(2, 2), &wsa_) != 0) {// try to initialize
			throw WinsockException("WSAStartup failed (_-_)", WSAGetLastError());
		}
		log_server("Winsock initialized", "successfully (^^)");

		SOCKET listening_socket_ = socket(AF_INET, SOCK_STREAM, 0); // listening socket(TCP, IP)
		if (listening_socket_ == INVALID_SOCKET) {
			throw SocketCreationException("socket() fialed", WSAGetLastError());
		}
		log_server("listening socket created", "successfully (^^)");

		sockaddr_in server_addrss_{};				// set address settings
		server_addrss_.sin_family = AF_INET;		// IP
		server_addrss_.sin_port = htons(port_);		// port
		server_addrss_.sin_addr.s_addr = INADDR_ANY;// accept connection for all IP

		if (bind(listening_socket_, (sockaddr*)&server_addrss_,// connect socket with address
			sizeof(server_addrss_)) == SOCKET_ERROR) {
			throw SocketBindException("bind() failed", WSAGetLastError());
		}

		if (listen(listening_socket_, 5) == SOCKET_ERROR) {// listening port(max queue -> 5)
			throw SocketListenException("listen() failed", WSAGetLastError());
		}

		log_server("server started", "waiting on port [" + std::to_string(port_) + ']');

		while (is_server_run_) {					// accept connection to client
			SOCKET client_socket_ = accept(listening_socket_, nullptr, nullptr);
			if (client_socket_ == INVALID_SOCKET) {
				if (is_server_run_) {
					log_error("Server Accept", "accept() failed", WSAGetLastError());
					is_server_run_ = false;			// stop when error
				}
			} else {
				log_server("new client connected", "starting handler");
				std::thread client_thread_(handle_client, client_socket_); // run separated thread for client 
				client_thread_.detach();			// will be auto-close
			}
		}
		closesocket(listening_socket_);				// close socket
		log_server("server loop", "finished");		// log:

	}
	catch (const std::exception& exception_) {
		log_error("server loop", exception_.what());
	}
	WSACleanup();									// wincosk clean [necessarily][!!!]
	log_server("server", "shutdwon complited");
}

int main() {
	enable_ansi();									// enable ANSI
	log_server("server application", "run");		
	std::thread server_thread_(server_loop);		// separated thread for server

	std::string command_;
	while (is_server_run_) {						// main run
		try {
			if (!std::getline(cin,command_)) {		// if something wrong with input
				clear_input_buffer();
				log_error("input validation","unknown command");
			}
			validate_server_input(command_);		// check input correctness
			process_server_command(command_);		// server command's
		} catch (const UnknownCommandException& e) {
			log_error("Server Command", e.what());
			log_utils("commands to help","help, ?");
		} catch (const std::exception& e) {
			log_error("Server Console", e.what());
		}
	}

	if (server_thread_.joinable()) {				// wait finish
		server_thread_.join();
	}

	log_server("server application", "finished with code 1");

	return 0;
}