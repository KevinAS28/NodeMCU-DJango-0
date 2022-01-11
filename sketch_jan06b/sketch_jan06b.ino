#include <map>

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


/*Put your SSID & Password*/
const char* ssid = "kevin-hotspot";  // Enter SSID here
const char* password = "HelloWorld321";  //Enter Password here
const char* DEVICE_ID = "NODMC0";

ESP8266WebServer server(80);

uint8_t LEDpin = D2;
bool LEDstatus = LOW;

typedef struct LedPinStatus{
  uint8_t LED_pin = 255;
  bool LED_status = false;
};

//std::map<int, LedPinStatus> led_map = {
//    { 0, (struct LedPinStatus){.LED_pin=D2, .LED_status=LOW} },
//    { 1, (struct LedPinStatus){.LED_pin=10, .LED_status=LOW} },
//};

LedPinStatus led_map[] = {
  (struct LedPinStatus){.LED_pin=D2, .LED_status=LOW}, 
  (struct LedPinStatus){.LED_pin=D1, .LED_status=LOW}
  };



void connectWIFI() {
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
}

void startServer() {

  server.on("/switchled", handle_switchLED);
  server.on("/led", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.on("/ledoff", handle_ledoff);
  server.on("/post", handleRoot);
  server.on("/postplain", handlePlain);
  server.on("/postform", handleForm);
  server.on("/postjson", handleJSON);
  server.on("/switchledmulti", handle_switchLED_multi);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Setup running!");

  for (int i=0; i<=sizeof(led_map)/sizeof(led_map[0]); i++){
    pinMode(led_map[i].LED_pin, OUTPUT);
  }
  
  connectWIFI();
  startServer();
}

void loop() {
  server.handleClient();

  for (int i=0; i<=sizeof(led_map)/sizeof(led_map[0]); i++){
    struct LedPinStatus led = led_map[i];
    
    if (led.LED_status){
      digitalWrite(led.LED_pin, HIGH);
    }
    else {
      digitalWrite(led.LED_pin, LOW);
    }
    
  }
}

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_switchLED_multi(){
//  digitalWrite(LEDpin, 1);
  
  StaticJsonDocument<1000> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, server.arg("plain"));

  StaticJsonDocument<100> jsonResponse;  

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    
    jsonResponse["success"] = false;
    char jsonResponseString[100];
    serializeJson(jsonResponse, jsonResponseString);

    
    server.send(200, "application/json", jsonResponseString);  
  }
  else{
    int led = jsonDoc["led"];
    struct LedPinStatus led_pin_status = led_map[led];
    char msg[100];
    sprintf(msg ,"LED %i  %i: %i", led_pin_status.LED_pin, led, led_pin_status.LED_status);
    Serial.println(msg);    
    
    if (!led_pin_status.LED_status){
      led_pin_status.LED_status = !led_pin_status.LED_status;
      led_map[led] = led_pin_status;
    }
    else {
      led_pin_status.LED_status = !led_pin_status.LED_status;
      led_map[led] = led_pin_status;
    }

    
    jsonResponse["success"] = true;
    char jsonResponseString[100];
    serializeJson(jsonResponse, jsonResponseString);
    server.send(200, "application/json", jsonResponseString);  
    
  }

//  delay(500);
//  digitalWrite(LEDpin, 0);
}

void handle_switchLED() {
  LEDstatus = !LEDstatus;
  server.send(200, "text/html", SendHTML((LEDstatus == LOW) ? false : true));
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true));
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_NotFound() {
  char msg[200] = "Not found ";
  strcat(msg, server.uri().c_str());
  server.send(404, "text/plain", msg);
}


const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>POST plain text to /postplain/</h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
      <input type=\"text\" name=\'{\"hello\": \"world\", \"trash\": \"\' value=\'\"}\'><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>POST form data to /postform/</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <input type=\"text\" name=\"hello\" value=\"world\"><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

void handleRoot() {
  digitalWrite(LEDpin, 1);
  server.send(200, "text/html", postForms);
  digitalWrite(LEDpin, 0);
}

void handleJSON(){
  digitalWrite(LEDpin, 1);
  
  StaticJsonDocument<1000> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, server.arg("plain"));

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    server.send(200, "application/json", "{\"success\": false}");  
  }
  else{
    char sum[200] = "";
    strcat(sum, jsonDoc["angka1"]);
    strcat(sum, jsonDoc["angka2"]);
    char response[250];
    sprintf(response, "{\"success\": true, \"sum\": %s}", sum);
    server.send(200, "application/json", response);  
    
  }

  delay(500);
  digitalWrite(LEDpin, 0);
  
}

void handlePlain() {
  if (server.method() != HTTP_POST) {
    digitalWrite(LEDpin, 1);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(LEDpin, 0);
  } else {
    digitalWrite(LEDpin, 1);
    server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
    digitalWrite(LEDpin, 0);
  }
}

void handleForm() {
  if (server.method() != HTTP_POST) {
    digitalWrite(LEDpin, 1);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(LEDpin, 0);
  } else {
    digitalWrite(LEDpin, 1);
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(200, "text/plain", message);
    digitalWrite(LEDpin, 0);
  }
}

String SendHTML(uint8_t led) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>LED</h1>\n";
  ptr += "<p>Click to switch LED on and off.</p>\n";
  ptr += "<form method=\"get\">\n";
  if (led)
    ptr += "<input type=\"button\" value=\"LED OFF\" onclick=\"window.location.href='/ledoff'\">\n";
  else
    ptr += "<input type=\"button\" value=\"LED ON\" onclick=\"window.location.href='/ledon'\">\n";
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
