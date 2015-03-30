//#include <DallasTemperature.h>
#include <OneWire.h>
//#include "config.h"
#include "U8glib.h"


#define E  3 //SCK
#define RW 4 //MOSI
#define RS 5 //CS


#define POTIN    A0
#define THERMISTORPIN     A7
#define PRESSURE A5
#define CURRENT  A6//A4

#define LED1_B     10 // BLUE
#define LED2_G      9 // GREEN

#define SW1      11
//#define SW2      12
#define SW3      49

#define K1        7
#define K2        6
#define K3        8
#define K5        31
#define K6        52

#define FANB 12
#define FANF 13

#define ON true
#define OFF false

int temp_val, potin_val, pressure_val, current_val;
int sw1_state, sw2_state, sw3_state;
bool k3_state, sw3;
int Temp;

char temp_buf[50], potin_buf[50], press_buf[50], curr_buf[50];

bool led_b, led_g;

U8GLIB_ST7920_128X64 LCD(E, RW, RS, U8G_PIN_NONE);

int psi, tmp, potin, current;

//int samples[NUMSAMPLES];

OneWire ds(48);
byte data[2];


void check_pins_state(void) {
  potin_val = analogRead(POTIN);
  delay(50);
  pressure_val = analogRead(PRESSURE);
  delay(50);
  current_val = analogRead(CURRENT);
  psi = psi_result(pressure_val);
  potin = potin_result(potin_val);
  current = current_result(current_val);
  sw1_state = digitalRead(SW1);
  if (sw1_state==HIGH) {
    digitalWrite(K1, HIGH);
    delay(8);
    digitalWrite(K5, HIGH);
    //digitalWrite(K6, HIGH);
    sw3_state = digitalRead(SW3);
    if (sw3_state == LOW) {sw3 = false;}
    if ((k3_state==false)&&(sw3_state==HIGH)){
      digitalWrite(K3, HIGH);
      digitalWrite(LED1_B, HIGH);
      k3_state=true;
      sw3 = true;
    } else {
      if (k3_state==false){
      digitalWrite(K3, HIGH);
      digitalWrite(LED1_B, HIGH);
      sw3=false;
      k3_state = true;
      }
    }
    if (current>5) sw2_state=HIGH; else sw2_state=LOW;
    if (sw2_state==HIGH) {
      digitalWrite(K3, LOW);
      digitalWrite(LED1_B, LOW);
      if (sw3 == false) {k3_state=false;}
      digitalWrite(K2, HIGH);
      digitalWrite(LED2_G, HIGH);
      digitalWrite(K6, LOW);
    } else {
      if (potin_val>240) {digitalWrite(K6, HIGH);} else { digitalWrite(K6, LOW);}
      digitalWrite(K2, LOW);
      digitalWrite(LED2_G, LOW);
      if (k3_state==false){
      digitalWrite(K3, HIGH);
      digitalWrite(LED1_B, HIGH);}
      sw1_state = digitalRead(SW1); 
      if ((k3_state==false)&&(sw3==true)){      
        digitalWrite(K3, HIGH);
        digitalWrite(LED1_B, HIGH); }       
        }
          if(sw1_state==LOW) {
        digitalWrite(K1, LOW);
        digitalWrite(K2, LOW);
        digitalWrite(LED2_G, LOW);
        digitalWrite(K3, LOW);
        digitalWrite(LED1_B, LOW);
        digitalWrite(K5, LOW);
        k3_state = false;
        sw3 = false;
          }
  } else {
      digitalWrite(K1, LOW);
      digitalWrite(K2, LOW);
      digitalWrite(LED2_G, LOW);
      digitalWrite(K3, LOW);
      digitalWrite(LED1_B, LOW);
      digitalWrite(K5, LOW);
      digitalWrite(K6, LOW);
      k3_state = false;
      sw3 = false;
  }
  sprintf(temp_buf, "%d", Temp); //*C
  sprintf(potin_buf, "%d", potin); //A
  sprintf(press_buf, "%d", psi); // psi
  sprintf(curr_buf, "%d", current); //A
  LCD.firstPage();  
  do {
    //check_pins_state();
    LCD.setFont(Bebasfont10x64);
    if (potin<10) {LCD.drawStr(0/*25*/, 64, potin_buf);} else {LCD.drawStr(0, 64, potin_buf);}
    LCD.drawStr(50, 64, "A");
    
    //LCD.setFont(u8g_font_fur17);
    LCD.setFont(Steelfish4x29);
    LCD.drawStr(92, 19, temp_buf);
    //LCD.setFont(u8g_font_helvR08);
    LCD.drawStr(112, 19, "C");
    
    LCD.setFont(Steelfish4x29);
    LCD.drawStr(84, 40, press_buf);
    //LCD.setFont(u8g_font_helvR08);
    LCD.drawStr(109, 40, "PSI");
    
    LCD.setFont(Steelfish4x29);
    LCD.drawStr(84, 62, curr_buf);
    //LCD.setFont(u8g_font_helvR08);
    LCD.drawStr(109, 62, "A");
    
    LCD.setColorIndex(1);
    LCD.drawCircle(110, 3, 1);
    LCD.drawLine(77, 0, 77, 63);
    LCD.drawLine(77, 21, 127, 21);
    LCD.drawLine(77, 42, 127, 42);
    //check_pins_state();
  } while( LCD.nextPage() );
}

int psi_result(int val) {
  int temp;
  //temp = (val*0.00122)*145.0377;
  temp = (((val*0.00488)*0.2)*145.0377)-16;
  return temp;
}

int potin_result(int val) {
  int temp;
  temp = val*0.044;
  return temp;
}

int current_result(int val) {
  int temp;
  //temp = val*0.244;
  temp = val*0.0609;//((val/205)*12.5);
  return temp;
}

void delay_ds18() {
  int i;
  for(i=0;i<6;i++){
    check_pins_state();
    delay(20);
  }
}


void setup() {
  //init LCD
  
  pinMode(SW1, INPUT); 
  pinMode(SW3, INPUT); 
  //pinMode(SW2, INPUT); 
  
  pinMode(K1, OUTPUT); 
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);  
  pinMode(K5, OUTPUT); 
  pinMode(K6, OUTPUT); 
  
  digitalWrite(K1, LOW);
  digitalWrite(K2, LOW);
  digitalWrite(K3, LOW);
  digitalWrite(K5, LOW);
  digitalWrite(K6, LOW);
  
  pinMode(LED1_B, OUTPUT);
  pinMode(LED2_G, OUTPUT); 
 
  pinMode(FANB, OUTPUT);
  pinMode(FANF, OUTPUT); 
  analogWrite(FANB, 0);
  analogWrite(FANF, 0);
   
  digitalWrite(LED1_B, LOW);
  digitalWrite(LED2_G, LOW);
  k3_state=false;
  sw3 = false;
  led_g=false;
  led_b=false;
  
    LCD.firstPage();  
  do {
    LCD.setFont(LaBabaca15x77);
    LCD.drawStr(0, 52, "Orestherm");
  } while( LCD.nextPage() );
  delay(3000);
  
  Temp = 0;
}

void loop() { 
  uint8_t i;
  float average;
  

  check_pins_state();
  ds.reset(); 
  ds.write(0xCC);
  ds.write(0x44);
  delay_ds18();
  //check_pins_state();  
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);
  data[0] = ds.read(); 
  data[1] = ds.read();
  Temp = (data[1]<< 8)+data[0];
  Temp = Temp>>4;
  if (Temp<30)
  {
    analogWrite(FANB, (int)(255*0.30));
    analogWrite(FANF, (int)(255*0.30)); 
  } 
  if ((Temp>30)&&(Temp<75)) analogWrite(FANB, (int)(Temp*3.4)); 
  if ((Temp>30)&&(Temp<70)) analogWrite(FANF, (int)(Temp*3.64));
  if (Temp>75) analogWrite(FANB, 255);
  if (Temp>70) analogWrite(FANF, 255);
  
  check_pins_state();
  

  //check_pins_state(); 

    
 // Serial.println(temp_buf);

  //check_pins_state();
}

