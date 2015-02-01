#define THERMISTORNOMINAL 50000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 20   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3000
// the value of the 'other' resistor
#define SERIESRESISTOR 50000    

#define POTIN    A0
#define THERMISTORPIN     A7
#define PRESSURE A2
#define CURRENT  A4

#define LED1_B     10 // BLUE
#define LED2_G      9 // GREEN

#define SW1      11
#define SW2      12
#define SW3      49

#define K1        7
#define K2        6
#define K3        8
#define K5        31

