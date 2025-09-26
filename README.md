First lab TCP/IP connection
COMPILE:
  server: cl /EHsc /Fe:server.exe server.cpp globals.cpp symbol_filter.cpp logger.cpp exception.cpp ws2_32.lib
  client: cl /EHsc /Fe:client.exe client.cpp globals.cpp symbol_filter.cpp logger.cpp exception.cpp ws2_32.lib

- globals.h have 2 this scripts, for [client] and [server]
- run in current directory: cd /<project_folder>
- then run scripts

enjoy (^^)
