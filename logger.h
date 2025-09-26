#ifndef LOGGER_H
#define LOGGER_H

#include "colors.h"
#include<iomanip>
#include <string>

/**
* File for logging the process
* + ASCI support
*/

/* logger's */
void log_server(const std::string& message_, const std::string& details_);
void log_client(const std::string& message_, const std::string& details_);
void log_error(const std::string& context_, const std::string& message_, int error_code_ = 0);
void log_utils(const std::string& message_, const std::string& details_);

/* enable ANSI escape sequences */
void enable_ansi();

#endif // LOGGER_H