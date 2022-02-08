// #include <ArduinoJson.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <string>
// #include <ctime>
// #include <stdarg.h>  
// #include <memory>    
// #include <stdexcept>
// #include <map>
// #include "WebSocksClient.h"
// // #include "JsonWSC.h"
// #include "tools.h"
// #include "conf.h"
// #include "custom_handler.h"

// void connectWifi(){
//   Serial.println("Connecting to ");
//   Serial.println(ssid);
  
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
  
//   Serial.println("");
//   Serial.println("WiFi connected..!");
//   Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
// }



// std::map<std::string, WebSockClient> connections_map;

// WebSockClient sendNodemcu;

// void setup()
// {
//     Serial.begin(115200);
//     Serial.println("Setup running!");

//     connectWifi();

//     // WebSockClient sendNodeMcuCon = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler);
//     connections_map["send_nodemcu"] = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler);
//     // sendNodemcu = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler);
//     // connections_map["send_server"] = WebSockClient(hostMap["send_server"], sendServerHandler);
// }

// void loop()
// {
//     // Handling connection event changes
//     for (auto connection : connections_map)
//     {
//         connection.second.loop();
//     }

//     // Led
//     powerLED();
    
// }
