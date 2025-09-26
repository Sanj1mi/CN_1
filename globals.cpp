#include "globals.h"

int port_ = 1280;

const char* ip_address_ = "127.0.0.1";

std::atomic<bool> is_server_run_ = true;
std::atomic<bool> is_client_active_ = true;
