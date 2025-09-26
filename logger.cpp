#include "logger.h"
#include <windows.h>
#include <iostream>

/**
* enable ANSI sequences 
*/
void enable_ansi() {
    /* get handle of standart input(console) */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    /* current mode of console */
    DWORD dwMode = 0;

    /* get current console settings */
    GetConsoleMode(hOut, &dwMode);

    /* add ASCI secunces flag */
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    
    /* set new settings */
    SetConsoleMode(hOut, dwMode);
}

void log_server(const std::string& message_, const std::string& details_) {
    std::cout << '[' << crgb::GREEN << "Server" << crgb::RESET << "] "
        << crgb::GRAY << std::setw(25) << std::left << message_;
    if (!details_.empty()) {
        std::cout << ": " << crgb::GREEN << details_;
    }
    std::cout << crgb::RESET << std::endl;
}

void log_client(const std::string& message_, const std::string& details_) {
    std::cout << '[' << crgb::GREEN << "Client" << crgb::RESET << "] "
        << crgb::GRAY << std::setw(25) << std::left << message_;
    if (!details_.empty()) {
        std::cout << ": " << crgb::GREEN << details_;
    }
    std::cout << crgb::RESET << std::endl;
}

void log_error(const std::string& context_, const std::string& message_, int error_code_) {
    std::cerr << '[' << crgb::RED << "Error"  << crgb::RESET << "] " << crgb::GRAY << std::setw(26) << std::left << context_ << ": " << crgb::RED << message_ << crgb::RESET;
    if (error_code_ != 0) {
        std::cerr << " (code: " << error_code_ << ")";
    }
    std::cerr << crgb::RESET << std::endl;
}

void log_utils(const std::string& message_, const std::string& details_) {
    std::cout << '[' << crgb::CYAN << "Utils" << crgb::RESET << "] "
        << crgb::GRAY << std::setw(26) << std::left << message_;
    if (!details_.empty()) {
        std::cout << ": " << crgb::GREEN << details_;
    }
    std::cout << crgb::RESET << std::endl;
}