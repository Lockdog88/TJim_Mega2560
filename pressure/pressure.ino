#include "config.h"
#include "U8glib.h"
#include "Thermistor.h"

#define E  3 //SCK
#define RW 4 //MOSI
#define RS 5 //CS

int temp_val, potin_val, pressure_val, current_val;
int sw1_state, sw2_state, sw3_state;
int k3_state;

char temp_buf[50], potin_buf[50], press_buf[50], curr_buf[50];

bool led_b, led_g;

Thermistor ntc(TEMP);
U8GLIB_ST7920_128X64 LCD(E, RW, RS, U8G_PIN_NONE);

int psi, tmp, potin, current;

void check_pins_state(void) {
  sw1_state = digitalRead(SW1);
  if (sw1_state==LOW) {
    digitalWrite(K1, HIGH);
    delay(8);
    digitalWrite(K5, HIGH);
    digitalWrite(K3, HIGH);
    digitalWrite(LED1_B, HIGH);
    sw2_state = digitalRead(SW2);
    if (sw2_state==HIGH) {
      digitalWrite(K3, LOW);
      digitalWrite(LED1_B, LOW);
      digitalWrite(K2, HIGH);
      digitalWrite(LED2_G, HIGH);
    } else {
      digitalWrite(K2, LOW);
      digitalWrite(LED2_G, LOW);
      sw3_state = digitalRead(SW3);
      if (sw3_state==LOW) {
        digitalWrite(K3, HIGH);
        digitalWrite(LED1_B, HIGH);
        } else {
        digitalWrite(K1, LOW);
        digitalWrite(K2, LOW);
        digitalWrite(LED2_G, LOW);
        digitalWrite(K3, LOW);
        digitalWrite(LED1_B, LOW);
        digitalWrite(K5, LOW);
        }
      }
  } else {
      digitalWrite(K1, LOW);
      digitalWrite(K2, LOW);
      digitalWrite(LED2_G, LOW);
      digitalWrite(K3, LOW);
      digitalWrite(LED1_B, LOW);
      digitalWrite(K5, LOW);
  }
}

int psi_result(int val) {
  int temp;
  temp = (val*0.00122)*145.0377;
  return temp;
}

int potin_result(int val) {
  int temp;
  temp = val*0.044;
  return temp;
}

int temp_result(void) {
  int temp;
  temp = ntc.getTemp();
  return temp;
}

int current_result(int val) {
  int temp;
  temp = val*0.244;
  return temp;
}

void setup() {
  //init LCD

  
  pinMode(SW1, INPUT); 
  pinMode(SW3, INPUT); 
  pinMode(SW2, INPUT); 
  
  pinMode(K1, OUTPUT); 
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);  
  pinMode(K5, OUTPUT); 
  
  digitalWrite(K1, LOW);
  digitalWrite(K2, LOW);
  digitalWrite(K3, LOW);
  digitalWrite(K5, LOW);
  
  pinMode(LED1_B, OUTPUT);
  pinMode(LED2_G, OUTPUT); 
 
  digitalWrite(LED1_B, LOW);
  digitalWrite(LED2_G, LOW);
  
  led_g=false;
  led_b=false;
  //LCD.setColorIndex(1);
  //LCD.drawLine(85, 0, 85, 63);
  //LCD.drawLine(85, 21, 127, 21);
  //LCD.drawLine(85, 42, 127, 42);
}

  
void loop() {
  temp_val = analogRead(TEMP);
  potin_val = analogRead(POTIN);
  pressure_val = analogRead(PRESSURE);
  current_val = analogRead(CURRENT);
  
  psi = psi_result(pressure_val);
  potin = potin_result(potin_val);
  tmp = temp_result();
  current = current_result(current_val);
  
  sprintf(temp_buf, "Temp(A1): %d *C", tmp);
  sprintf(potin_buf, "Pot.(A0): %d A", potin);
  sprintf(press_buf, "Pressure(A2): %d PSI", psi);
  sprintf(curr_buf, "Current(A3): %d A", current);
  check_pins_state();
  
  LCD.firstPage();  
  do {
    LCD.setFont(u8g_font_7x14);
    LCD.drawStr(0, 0, temp_buf);
    LCD.drawStr(0, 12, potin_buf);
    LCD.drawStr(0, 24, press_buf);
    LCD.drawStr(0, 38, curr_buf);
  } while( LCD.nextPage() );
}

