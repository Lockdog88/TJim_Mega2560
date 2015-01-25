#include "config.h"
#include "U8glib.h"

#define E  3 //SCK
#define RW 4 //MOSI
#define RS 5 //CS

int temp_val, potin_val, pressure_val;
int sw1_state, sw2_state, sw3_state;

void setup() {
  //init LCD
  U8GLIB_ST7920_128X64 u8g(E, RW, RS, U8G_PIN_NONE);
  
  pinMode(SW1, INPUT); 
  pinMode(SW3, INPUT); 
  pinMode(SW2, INPUT); 
  
  pinMode(K1, OUTPUT); 
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);  
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);  
}

void loop() {
  temp_val = analogRead(TEMP);
  potin_val = analogRead(POTIN);
  pressure_val = analogRead(PRESSURE);
  
  sw1_state = digitalRead(SW1);
  sw2_state = digitalRead(SW2);
  if ((sw1_state==LOW)&&(sw2_state==LOW)) {digitalWrite(K3, HIGH); digitalWrite(LED1, HIGH);} else {digitalWrite(K3, LOW); digitalWrite(LED1, LOW);}
  if (sw2_state == LOW) {digitalWrite(K2, HIGH); digitalWrite(LED2, HIGH);} else {digitalWrite(K2, LOW); digitalWrite(LED2, LOW);}
  


}

