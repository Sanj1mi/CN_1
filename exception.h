#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

/**
* custom exceptions
*/

/* input exception */
class UnknownCommandException : public std::runtime_error {
public:
    explicit UnknownCommandException(const std::string& command)
        : std::runtime_error("Unknown command: " + command) {}
};


class InvalidInputException : public std::runtime_error {
public:
    explicit InvalidInputException(const std::string& message)
        : std::runtime_error("Invalid input: " + message) {}
};


/* Basic exceptions */
class NetworkException : public std::runtime_error {
public:
    explicit NetworkException(const std::string& message, int error_code = 0)
        : std::runtime_error(message), error_code_(error_code) {}

    int get_error_code() const { return error_code_; }

private:
    int error_code_;
};

/* Scoket exceptions */
class SocketException : public NetworkException {
public:
    explicit SocketException(const std::string& message, int error_code = 0)
        : NetworkException(message, error_code) {
    }
};

class SocketCreationException : public SocketException {
public:
    explicit SocketCreationException(const std::string& message, int error_code = 0)
        : SocketException("Socket creation failed: " + message, error_code) {
    }
};

class SocketBindException : public SocketException {
public:
    explicit SocketBindException(const std::string& message, int error_code = 0)
        : SocketException("Socket bind failed: " + message, error_code) {
    }
};

class SocketListenException : public SocketException {
public:
    explicit SocketListenException(const std::string& message, int error_code = 0)
        : SocketException("Socket listen failed: " + message, error_code) {
    }
};

/* Linking exceptions */
class ConnectionException : public NetworkException {
public:
    explicit ConnectionException(const std::string& message, int error_code = 0)
        : NetworkException(message, error_code) {
    }
};

class ConnectionFailedException : public ConnectionException {
public:
    explicit ConnectionFailedException(const std::string& message, int error_code = 0)
        : ConnectionException("Connection failed: " + message, error_code) {
    }
};

class ConnectionLostException : public ConnectionException {
public:
    explicit ConnectionLostException(const std::string& message, int error_code = 0)
        : ConnectionException("Connection lost: " + message, error_code) {
    }
};

/* Server exceptions */
class ServerException : public NetworkException {
public:
    explicit ServerException(const std::string& message, int error_code = 0)
        : NetworkException(message, error_code) {
    }
};

class ServerInitializationException : public ServerException {
public:
    explicit ServerInitializationException(const std::string& message, int error_code = 0)
        : ServerException("Server initialization failed: " + message, error_code) {
    }
};

/* Winsock exceptions */
class WinsockException : public NetworkException {
public:
    explicit WinsockException(const std::string& message, int error_code = 0)
        : NetworkException("Winsock error: " + message, error_code) {
    }
};

#endif // EXCEPTIONS_H