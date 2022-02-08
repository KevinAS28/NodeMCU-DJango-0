#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string>
#include <ctime>
#include <stdarg.h>  
#include <memory>    
#include <stdexcept>
#include <map>
#include <vector>
#include "WebSocksClient.h"
// #include "JsonWSC.h"
#include "tools.h"
#include "conf.h"
#include "custom_handler.h"

void connectWifi(){
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
}

WebSockClient all_connections[25] = {
    {hostMap["send_nodemcu"], sendNodemcuHandler, false}
};

uint8_t leds[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10};

// WebSockClient nodemcu_send0 = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler, false);

void setup()
{
    Serial.begin(115200);
    Serial.println("Setup running!");

    // for (uint8_t i = 0; i < 10; i++)
    // {
    //     pinMode(leds[i], OUTPUT);
    // }

    connectWifi();

    // connections_map.push_back(WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler, false));
   // WebSockClient sendNodeMcuCon = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler);
    // connections_map["send_nodemcu"] = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler);
    // connections_map.insert(std::pair<std::string, WebSockClient>("send_nodemcu", WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler)));
    // sendNodemcu = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler);
    // connections_map["send_server"] = WebSockClient(hostMap["send_server"], sendServerHandler);
    setupLED();
    // for (uint8_t i = 0; i < 10; i++)
    // {
    //     pinMode(leds[i], OUTPUT);
    // }

    // nodemcu_send0.initializeServerConnection();
    // nodemcu_send0.connectServer();
    // connections_map[0] = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler);
    // connections_map.push_back(WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler));
    // auto sendNodeMcuConnection = WebSockClient(hostMap["send_nodemcu"], sendNodemcuHandler, false);
    // all_connections.push_back(sendNodeMcuConnection);

    // for (WebSockClient wsc : all_connections){
    //   if (wsc.initialized){
    //       Serial.println("Try to connecting 1 object");
    //       wsc.connectServer();
    //   }
    // }
    for (size_t i = 0; i < (sizeof(all_connections)/sizeof(all_connections[0])); i++)
    {
        WebSockClient & wsc = all_connections[i];
        if (wsc.isInitialized()){
            wsc.connectServer();
        }
        
    }

    Serial.println("Setup ended!");
    delay(2000);    
}

bool loop_msg = false;

void loop()
{
    if (!loop_msg){
      loop_msg = true;
      Serial.println("Loop running");
    }
    // Led
    powerLED();
    // for (uint8_t i = 0; i < 10; i++)
    // {
    //     // pinMode(leds[i], OUTPUT);
    //     digitalWrite(leds[i], HIGH);
    // }   
    // delay(1000);
    // for (uint8_t i = 0; i < 10; i++)
    // {
    //     // pinMode(leds[i], OUTPUT);
    //     digitalWrite(leds[i], LOW);
    // }   
    // delay(1000);

    // Handling connection event changes
    // for (auto connection : connections_map)
    // {
    //     connection.second.loop();
    // }
    // for (WebSockClient wsc : connections_map){
    //     wsc.loop();
    // }
  // connections_map[0].loop();
  // nodemcu_send0.loop();

    for (size_t i = 0; i < (sizeof(all_connections)/sizeof(all_connections[0])); i++)
    {
        WebSockClient & wsc = all_connections[i];
        
        if (wsc.isInitialized()){
            wsc.loop();
        }
        
    }
    // for (WebSockClient wsc : all_connections){
    //   if (wsc.initialized){
    //       // Serial.println("Try to connecting 1 object");
    //       wsc.loop();
    //   }
    // }

}
