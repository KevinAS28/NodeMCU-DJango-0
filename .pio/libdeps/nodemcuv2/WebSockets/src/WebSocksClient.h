#ifndef WEBSOCKSCLIENT_H_
#define WEBSOCKSCLIENT_H_

#include "WebSockets.h"
#include "WebSocketsClient.h"
#include <ArduinoJson.h>
#include <string>
// #include "WebSocksClientHandler.h"

struct HostDest {
    std::string address;
    std::string path;
    int port;
};


// class EventHandlerPack{
//   public:
//     virtual void onConnected(){}
//     virtual void onDisconnected(){}
//     virtual void onGetTextPayload(uint8_t * payload){}
//     virtual void onGetString(std::string the_string){}
//     virtual void onGetJson(DynamicJsonDocument json){}
//     virtual void onGetBin(uint8_t * payload, size_t length){}
//     virtual void onError(){}
// };


class WebSockClient {
  public:
    bool keepLooping = false;
    bool connected = false;
    // bool autoConnect = true;
    // EventHandlerPack eventHandlerPack;

    WebSockClient(std::string _address, std::string _path, int _port, WebSocketClientEventHandler _eventHandler, bool autoConnect = true)
        : address(_address),
          path(_path),
          port(_port) {
        this->hostDest     = { .address = _address, .path = _path, .port = _port };
        this->eventHandler = _eventHandler;
        this->initializeWebSocket();
        this->initialized = true;
        if(autoConnect) {
            this->connectServer();
        }
    }

    WebSockClient(HostDest _dest, WebSocketClientEventHandler _eventHandler, bool autoConnect = true) {
        this->hostDest     = _dest;
        this->address      = hostDest.address;
        this->path         = hostDest.path;
        this->port         = hostDest.port;
        this->eventHandler = _eventHandler;
        this->initializeWebSocket();
        this->initialized = true;
        if(autoConnect) {
            this->connectServer();
            // this->autoConnect = autoConnect;
        }
    }

    WebSockClient(){this->initialized=false;}

    // WebSockClient(WebSockClient & _wsc)
    // {
    //     this->copyOther(_wsc);
    // }

    // void copyOther(WebSockClient & _wsc){
    //     this->keepLooping = _wsc.keepLooping;
    //     this->connected = _wsc.connected;
    //     this->initialized = _wsc.initialized;
    //     this->webSocket = _wsc.webSocket;
    //     this->hostDest = _wsc.hostDest;
    //     this->address = _wsc.address;
    //     this->path = _wsc.path;
    //     this->port = _wsc.port;
    //     this->eventHandler = _wsc.eventHandler;
    // }

    // void copyOther(const WebSockClient & _wsc){
    //     this->keepLooping = _wsc.keepLooping;
    //     this->connected = _wsc.connected;
    //     this->initialized = _wsc.initialized;
    //     this->webSocket = _wsc.webSocket;
    //     this->hostDest = _wsc.hostDest;
    //     this->address = _wsc.address;
    //     this->path = _wsc.path;
    //     this->port = _wsc.port;
    //     this->eventHandler = _wsc.eventHandler;
    // }

    // WebSockClient & operator=(const WebSockClient& other){
    //     if (this!=&other){
    //         this->copyOther(other);
    //     }
    //     return *this;
    // }

    void setConnected(bool _connected){
      this->connected = _connected;
      Serial.printf("Connected set to %d\n", this->connected);
    }

    void initializeServerConnection(){
            if (!(this->connected)){this->initializeWebSocket();}
            webSocket.begin(hostDest.address.c_str(), hostDest.port, hostDest.path.c_str());
            webSocket.onEvent(this->eventHandler);   
    }

    void connectServer(int timeout = 10000) {
        if (!this->initialized){
          Serial.println("WSC Not initialized yet, cannot connectServer()");
          return;
        }
        std::string msg = ("Connecting to webserver " + hostDest.address + " " + hostDest.path + " with port: " + std::to_string(hostDest.port));
        Serial.println(msg.c_str());

        int timing = 0;
        while(!(this->connected)) {
            if (!(this->connected)){this->initializeWebSocket();}
            webSocket.begin(hostDest.address.c_str(), hostDest.port, hostDest.path.c_str());
            webSocket.onEvent(this->eventHandler);
            while((!this->connected) && (timing <= timeout)) {
                webSocket.loop();
                delay(100);
                timing+=100;
            }
            if(!this->connected) {
                Serial.println("Connecting timeout! Try again");
            } else {
                break;
            }

            timing = 0;
        }
        Serial.println("Connected to server successful!");
        // webSocket.begin(hostDest.address.c_str(), hostDest.port, hostDest.path.c_str());
        // webSocket.onEvent(this->eventHandler);
        // while (!(this->connected)){
        //   this->loop();
        // }
        

        
    }

    bool isConnected() {
        return this->connected;
    }

    bool isInitialized() {
        return this->initialized;
    }

    void loop() {
        if (!this->initialized){
            Serial.println("Not initialized yet! Cannot loop");
            return;
        }
        (this->webSocket).loop();
    }

    void loopForever() {
        this->keepLooping = true;
        while(this->keepLooping) {
            this->loop();
        }
    }

    operator bool() const {
        return this->initialized;
    }
  
  // protected:
    bool initialized = false;
    WebSocketsClient webSocket;
    std::string address;
    std::string path;
    int port;
    HostDest hostDest;
    WebSocketClientEventHandler eventHandler;

    void initializeWebSocket() {
        this->webSocket = WebSocketsClient(this);
    }
};



#endif