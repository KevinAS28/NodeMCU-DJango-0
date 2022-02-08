// #ifndef JSON_WSC_H0
// #define JSON_WSC_H0

// #include "WebSocksClient.h"
// #include <ArduinoJson.h>
// #include "WebSocksClientHandler.h"

// void jsonWSCWrapperEventHandler(WebSockClient *_wsc, WStype_t type, uint8_t * payload, size_t length);

// class JsonWSC : public WebSockClient{
//     public:

//         // void generalJsonWSCInitialize(){

//         // }

//         void jsonWSCGeneralInitialize(){
//             this->obj = this;
//         }

//         JsonWSC(std::string _address, std::string _path, int _port, bool autoConnect = true):
//         WebSockClient(_address, _path, _port, jsonWSCWrapperEventHandler, autoConnect)
//         {
//             // this->eventHandlerPack = _eventHandlerPack;
//             jsonWSCGeneralInitialize();
//         }

//         JsonWSC(HostDest _dest, bool autoConnect = true):
//         WebSockClient(_dest, jsonWSCWrapperEventHandler, autoConnect)
//         {
//             // this->eventHandlerPack = _eventHandlerPack;
//             jsonWSCGeneralInitialize();
//         }

//         JsonWSC(WebSockClient& wsc){
//             this->hostDest     = wsc.hostDest;
//             this->address      = wsc.hostDest.address;
//             this->path         = wsc.hostDest.path;
//             this->port         = wsc.hostDest.port;
//             this->eventHandler = wsc.eventHandler;
//             this->eventHandlerPack = wsc.eventHandlerPack;
//             this->webSocket    = wsc.webSocket;
//             this->initialized  = wsc.initialized;
//             this->autoConnect  = wsc.autoConnect;
//             this->keepLooping  = wsc.keepLooping;
//             this->connected    = wsc.connected;
//         }

//         JsonWSC(){}

//         virtual void onConnected(){}
//         virtual void onDisconnected(){}
//         virtual void onGetTextPayload(uint8_t * payload){}
//         virtual void onGetBin(uint8_t * payload, size_t length){}
//         virtual void onError(){}

//         virtual void onGetJson(DynamicJsonDocument json){
//             // Serial.println("GET JSON");
//             Serial.println("JSON received!");
//         }

//         virtual void onGetString(std::string the_string){
//             Serial.println(the_string.c_str());
//         }
// };

// void jsonWSCWrapperEventHandler(WebSockClient *_wsc, WStype_t type, uint8_t * payload, size_t length){
//     Serial.println("JSON WSC WRAPPER");
//     JsonWSC  wsc = JsonWSC(*_wsc);
//     Serial.println("CASTED!");
//     delay(2000);
//     switch(type) {
//         case WStype_DISCONNECTED:
//             // Serial.printf("[WSc] Disconnected!\n");
//             Serial.println("JSON WSC DISCONNECT");
//             delay(2000);
//             (*_wsc).connected = false;
//             (wsc).onDisconnected();
//             break;
//         case WStype_CONNECTED: {
//             Serial.println("JSON WSC CONNECT");
//             delay(2000);
//             // Serial.printf("[WSc] Connected to url: %s\n", payload);
//             (*_wsc).connected = true;
//             (wsc).onConnected();
//             break;
//         }
//         case WStype_TEXT:{
//             Serial.println("JSON WSC TEXT");
//             delay(2000);
//             // Serial.printf("[WSc] RESPONSE: %s\n", payload);
//             (wsc).onGetTextPayload(payload);

//             auto the_string = (std::string((char *)payload));
//             DynamicJsonDocument json(1024);
//             auto err = deserializeJson(json, the_string);
//             if (err) {
//             Serial.println("JSON WSC String");
//             delay(2000);
//                 (wsc).onGetString(the_string);
//             }
//             else{
//             Serial.println("JSON WSC json");
//             delay(2000);
//                 (wsc).onGetJson(json);
//             }
//             break;
//         }
//         case WStype_BIN:
//             Serial.println("JSON WSC bin");
//             delay(2000);
//             (wsc).onGetBin(payload, length);
//             break;
//         case WStype_PING:
//                 // pong will be send automatically
//                 Serial.printf("[WSc] get ping\n");
//                 break;
//         case WStype_PONG:
//                 // answer to a ping we send
//                 Serial.printf("[WSc] get pong\n");
//                 break;
//         case WStype_ERROR:
//                     Serial.println("JSON WSC ERROR");
//             delay(2000);
//             (wsc).onError();
//     }

// }
// #endif