#ifndef GLOBALS_H
#define GLOBALS_H

/* special for threads */
#include <atomic>	

/* port */
extern int port_;	

/* IP address */
extern const char* ip_address_;

/* flags */
extern std::atomic<bool> is_server_run_;	
extern std::atomic<bool> is_client_active_;

#endif // !GLOBALS_H

/*
	Compile scripts:
	server: cl /EHsc /Fe:server.exe server.cpp globals.cpp symbol_filter.cpp logger.cpp exception.cpp ws2_32.lib
	client: cl /EHsc /Fe:client.exe client.cpp globals.cpp symbol_filter.cpp logger.cpp exception.cpp ws2_32.lib
*/
