
#include "config.h"
#include "U8glib.h"
#include "SPI.h"

#define E  3 //SCK
#define RW 4 //MOSI
#define RS 5 //CS

#define ON true
#define OFF false

#define sCS 53

int temp_val, potin_val, pressure_val, current_val;
int sw1_state, sw2_state, sw3_state;
bool k3_state, sw3;

char temp_buf[50], potin_buf[50], press_buf[50], curr_buf[50];

bool led_b, led_g;

U8GLIB_ST7920_128X64 LCD(E, RW, RS, U8G_PIN_NONE);

int psi, tmp, potin, current;

int samples[NUMSAMPLES];



void check_pins_state(void) {
  sw1_state = digitalRead(SW1);
  if (sw1_state==HIGH) {
    digitalWrite(K1, HIGH);
    delay(8);
    digitalWrite(K5, HIGH);
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
    sw2_state = digitalRead(SW2);
    if (sw2_state==HIGH) {
      digitalWrite(K3, LOW);
      digitalWrite(LED1_B, LOW);
      if (sw3 == false) {k3_state=false;}
      digitalWrite(K2, HIGH);
      digitalWrite(LED2_G, HIGH);
    } else {
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
      k3_state = false;
      sw3 = false;
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

int current_result(int val) {
  int temp;
  temp = val*0.244;
  return temp;
}

void digitalPotWrite(int adr, int val) {
  digitalWrite(sCS, LOW);
  SPI.transfer(adr);
  SPI.transfer(val);
  digitalWrite(sCS, HIGH);
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
  k3_state=false;
  sw3 = false;
  led_g=false;
  led_b=false;
  
  pinMode(sCS, OUTPUT);
  SPI.begin(); 
  

}

void loop() {
  uint8_t i;
  float average;

  potin_val = analogRead(POTIN);
  delay(100);
  pressure_val = analogRead(PRESSURE);
  delay(100);
  current_val = analogRead(CURRENT);
  delay(100);
  
  psi = psi_result(pressure_val);
  potin = potin_result(potin_val);
  current = current_result(current_val);
  analogReference(INTERNAL2V56);
  delay(200);
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  analogReference(DEFAULT);
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  // convert the value to resistance
  average = 2.56 * 1023 / average - 1;
  average = SERIESRESISTOR / average;
 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C  
  
  sprintf(temp_buf, "%d", steinhart); //*C
  sprintf(potin_buf, "%d", potin); //A
  sprintf(press_buf, "%d", psi); // psi
  sprintf(curr_buf, "%d", current); //A
  
  check_pins_state();
  if ((potin_val/4)<60) {digitalPotWrite(0, (potin_val/4));}
  if ((potin_val/4)>60) { 
   sw2_state = digitalRead(SW2);
   if (sw2_state == HIGH) {
     if ((potin_val/4)>60) {digitalPotWrite(0, potin_val/5.58);} else {digitalPotWrite(0, potin_val/4);}
   } else { digitalPotWrite(0, 60);}
   }
    
  
  LCD.firstPage();  
  do {
    LCD.setFont(Bebasfont10x64);
    if (potin<10) {LCD.drawStr(25, 64, potin_buf);} else {LCD.drawStr(0, 64, potin_buf);}
    LCD.drawStr(50, 64, "A");
    
    //LCD.setFont(u8g_font_fur17);
    LCD.setFont(Steelfish4x29);
    LCD.drawStr(92, 19, temp_buf);
    //LCD.setFont(u8g_font_helvR08);
    LCD.drawStr(109, 19, "C");
    
    LCD.setFont(Steelfish4x29);
    LCD.drawStr(84, 40, press_buf);
    //LCD.setFont(u8g_font_helvR08);
    LCD.drawStr(109, 40, "PSI");
    
    LCD.setFont(Steelfish4x29);
    LCD.drawStr(84, 62, curr_buf);
    //LCD.setFont(u8g_font_helvR08);
    LCD.drawStr(109, 62, "A");
    
    LCD.setColorIndex(1);
    LCD.drawCircle(107, 3, 1);
    LCD.drawLine(77, 0, 77, 63);
    LCD.drawLine(77, 21, 127, 21);
    LCD.drawLine(77, 42, 127, 42);
  } while( LCD.nextPage() );
}

