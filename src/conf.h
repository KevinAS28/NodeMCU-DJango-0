#ifndef CUSTOM_CONF0
#define CUSTOM_CONF0

#include "tools.h"
#include <map>
#include "WebSocksClient.h"

const char* ssid = "kevin-hotspot"; 
const char* password = "HelloWorld321";
const uint16_t port = 8000;
const char* DEVICE_ID = "NODMC0";

std::map<std::string, HostDest> hostMap = {
  {"send_nodemcu", {.address="10.42.0.1", .path="/ws/main/send_nodemcu/", .port=8000}},
  {"send_server", {.address="10.42.0.1", .path="/ws/main/send_server/", .port=8000}}
  };

#endif