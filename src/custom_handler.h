#ifndef CUSTOM_HANDLER0
#define CUSTOM_HANDLER0

#include "WebSocksClient.h"
#include "ArduinoJson.h"
#include <map>
#include <string>
#include "led.h"

void multiSwitchLed(JsonObject arguments){
    Serial.println("multiswitch!");
    JsonObject led_switches = arguments["LED_switchs"];
    for (auto const& led_sw:led_switches){
        std::string led = led_sw.key().c_str();
        bool status = led_sw.value();
        uint8_t led_index = std::stoi(led);
        bool current_status = led_map[led_index].LED_status;
        led_map[led_index].LED_status = (status==1) ? !current_status : current_status;
        Serial.print("Key: "); Serial.printf("%d\n", led_index);
        Serial.print(" Value: "); Serial.printf("%d\n", status);
    }
}

void runJsonCommand(JsonArray commandArguments){
    Serial.println("Run Json Command");
    for (JsonObject jsonCommandArgument : commandArguments){
        std::string command = jsonCommandArgument["command"];
        JsonObject arguments = jsonCommandArgument["arguments"];
        Serial.print("Run Command:"); Serial.println(command.c_str());
        if (command==(std::string)"multi_switch_led"){
            multiSwitchLed(arguments);
        }
    }
}


void sendNodemcuHandler(WebSockClient *_wsc, WStype_t type, uint8_t * payload, size_t length){
    Serial.println("sendNodemcuHandler() called");
    switch(type) {
        case WStype_DISCONNECTED: {
            Serial.printf("[Custom WSc] Disconnected!\n");
            Serial.printf("WSC initialized: %d\n", _wsc->initialized);
            Serial.printf("WSC path: %s\n", _wsc->path.c_str());
            _wsc->setConnected(false);
            Serial.println("[Custom WSc] End of disconneced");
            break;
        }
        case WStype_CONNECTED: {
            Serial.printf("[Custom WSc] Connected to url: %s\n", payload);
            Serial.printf("WSC initialized: %d\n", _wsc->initialized);
            Serial.printf("WSC path: %s\n", _wsc->path.c_str());
            _wsc->setConnected(true);
            Serial.println("[Custom WSc] End of conneced");
            break;
        }
        case WStype_TEXT:{
            Serial.printf("[WSc] RESPONSE: %s\n", payload);
            std::string the_string = std::string((char *) payload);
            DynamicJsonDocument json(1024);
            auto jsonError = deserializeJson(json, the_string);
            if (jsonError){
                Serial.printf("Json Error: %s\n", jsonError.c_str());
            }else{
                Serial.println("Json accepted");
                std::string type = json["data"]["type"];
                bool check = type==(std::string)"json_command";
                Serial.println(check);
                if (check){
                    runJsonCommand(json["data"]["command_arguments"]);
                }
            }
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
        case WStype_ERROR:{
            Serial.println("[WSc] ERROR");
            break;
        }
    }
}

// class SendNodeMcuHandler: public EventHandlerPack{
//     public:
//         SendNodeMcuHandler(){}
//         void onGetJson(DynamicJsonDocument json) override{
//             // Serial.println("GET JSON");
//             serializeJsonPretty(json, Serial);
//             Serial.println("JSON received!");
//         }

//         void onGetString(std::string the_string) override{
//             Serial.println(the_string.c_str());
//         }

//         void onConnected(){}
//         void onDisconnected(){}
//         void onGetTextPayload(uint8_t * payload){}
//         void onGetBin(uint8_t * payload, size_t length){}
//         void onError(){}
// };

void sendServerHandler(WebSockClient *_wsc, WStype_t type, uint8_t * payload, size_t length){
}

#endif