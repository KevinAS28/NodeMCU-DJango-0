

// #ifndef WEBSOCKSCLIENTHANDLER_0
// #define WEBSOCKSCLIENTHANDLER_0
// #include "WebSocksClient.h"


// void webSocketEventHandler(WebSockClient *_wsc, WStype_t type, uint8_t * payload, size_t length){
//  {
//           WebSockClient wsc = (*_wsc);
//           switch(type) {
//               case WStype_DISCONNECTED:
//                   Serial.printf("[WSc] Disconnected!\n");
//                   wsc.connected = false;
//                   break;
//               case WStype_CONNECTED: {
//                   Serial.printf("[WSc] Connected to url: %s\n", payload);
//                   wsc.connected = false;
//               }
//                   break;
//               case WStype_TEXT:
//                   Serial.printf("[WSc] RESPONSE: %s\n", payload);
//                   break;
//               case WStype_BIN:
//                   Serial.printf("[WSc] get binary length: %u\n", length);
//                   hexdump(payload, length);
//                   break;
//                       case WStype_PING:
//                               // pong will be send automatically
//                               Serial.printf("[WSc] get ping\n");
//                               break;
//                       case WStype_PONG:
//                               // answer to a ping we send
//                               Serial.printf("[WSc] get pong\n");
//                               break;
//               case WStype_ERROR:
//                   Serial.println("[WSc] ERROR");
//                   break;
//           }
//       }
//     }

// #endif