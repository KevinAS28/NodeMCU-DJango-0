#ifndef CUSTOM_LED
#define CUSTOM_LED

#include <stdint.h>
#include "Arduino.h"

struct LedPinStatus{
  uint8_t LED_pin = 255;
  bool LED_status = false;
};

LedPinStatus led_map[] = {
  (struct LedPinStatus){.LED_pin=D1, .LED_status=HIGH},
  (struct LedPinStatus){.LED_pin=D2, .LED_status=HIGH}, 

};

void setupLED(){
    for (unsigned int i=0; i<=sizeof(led_map)/sizeof(led_map[0]); i++){
      pinMode(led_map[i].LED_pin, OUTPUT);
    }
}

void powerLED(){
    for (unsigned int i=0; i<=sizeof(led_map)/sizeof(led_map[0]); i++){
       digitalWrite(led_map[i].LED_pin, led_map[i].LED_status);
    }    
}

#endif