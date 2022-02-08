#ifndef JSON_WSC_1
#define JSON_WSC_1

#include "WebSocksClient.h"
#include <ArduinoJson.h>

class EventHandlerPack{
    virtual void onConnected(){Serial.println("Default onConnected()");}
    virtual void onDisconnected(){Serial.println("Default onDisconnected()");}
    virtual void onGetPayload(uint8_t * payload){Serial.println("Default onGetPayload()");}
    virtual void onGetString(std::string the_string){Serial.println("Default onGetString()");}
    virtual void onGetJson(DynamicJsonDocument json){Serial.println("Default onGetJson()");}
    virtual void onGetBin(uint8_t * payload, size_t length){Serial.println("Default onGetBin()");}
    virtual void onError(){Serial.println("Default onError()");}
};

void jsonWrapperEventHandler(WebSockClient *_wsc, WStype_t type, uint8_t * payload, size_t length){
    // Serial.println("sendNodemcuHandler() called");
    switch(type) {
        case WStype_DISCONNECTED: {
            Serial.printf("[WSc] Disconnected!\n");
            (*_wsc).connected = false;
            break;
        }
        case WStype_CONNECTED: {
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            (*_wsc).connected = true;
            break;
        }
        case WStype_TEXT:{
            Serial.printf("[WSc] RESPONSE: %s\n", payload);
            break;
        }
        case WStype_BIN:{
            Serial.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            break;
        }
        case WStype_PING:
                // pong will be send automatically
                Serial.printf("[WSc] get ping\n");
                break;
        case WStype_PONG:
                // answer to a ping we send
                Serial.printf("[WSc] get pong\n");
                break;
        case WStype_ERROR:
            Serial.println("[WSc] ERROR");
            break;
    }
}

#endif