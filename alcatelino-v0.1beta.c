//SPIDRIVER ALCATEL - ALCATELINO v0.1 beta - 30.04.2018
//MODIFED BY SQ2MTG & SP2WK

#include <Wire.h>
#include <SPI.h>

//ZDEFINIOWANIE WYJSCIA SPI
#define DATA  5   /* Serial Data */
#define CLK 6   /* Serial Clock */
#define LE  7   /* Synthesizer Latch Enable */
//

unsigned long int Frq=431400000; // FREQ WE


unsigned long int FI=21400000; // POSREDNIA FREQ ( first mixer )




void setup() {
Serial.begin(9600); // UART 9600 z ALCATELINO

 pinMode (LE,     OUTPUT);
 drive_bus (0);
 digitalWrite (LE, LOW);
 digitalWrite (CLK, LOW);
}

void set_pll (unsigned long int hz)
{
 unsigned int n;    /* n to 10-bitowy licznik dzielony przez 127 */
 byte a;      /* a to 7-bitowy licznik dodatkowy */
//hz*=1000;
  //hz += (0);//-(!Tx*Shift*-7600000);      /* first mixer: 21.4 MHz */
  hz /= 5000;     // Divide by 5KHz reference
  n = hz / 127;  // 127 for UHF
  a = hz - (n * 127);  // 127 for UHF
  drive_bus (1);

  /* Send "(n << 8)|(a << 1)", 24 bits, MSB first, LSB always zero */
  emit_byte ((n >> 8) & 0xFF);  /* N high byte */
  emit_byte (n & 0xFF);   /* N low byte */
  emit_byte (a << 1);   /* A and LSB 0 */
  pulse_le();     /* Latch it */

 
  emit_byte (0x16);   // 0x0901 is R=1152<<1 with LSB flag set but i wanted 5K step so change new settings
  emit_byte (0x81);
  pulse_le();

  drive_bus (0);
}
volatile unsigned long int time = 0;
void drive_bus (byte enable)
{
#if TRACES_CUT
enable = 1; 
#endif

  if (enable) {
    pinMode (DATA, OUTPUT);
    pinMode (CLK, OUTPUT);
    digitalWrite (DATA, HIGH);
  } else {
    pinMode (DATA, INPUT);
    pinMode (CLK, INPUT);
    digitalWrite (DATA, LOW);
  }
}


void emit_byte (byte c)
{
  byte bit;

  for (bit = 0; bit < 8; bit++)
    {
      digitalWrite (DATA, (c & 0x80) ? HIGH : LOW);
      time++;
      time++; 
      digitalWrite (CLK, HIGH); /* rising edge latches data */
      c <<= 1;      
      time++;
      time++;
      time++;
      digitalWrite (CLK, LOW);
    }


  digitalWrite (DATA, HIGH);
}
void pulse_le (void)
{
  digitalWrite (LE, HIGH);
  time++;     
  time++;
  digitalWrite (LE, LOW);

}
void loop() {
set_pll(Frq+FI);
  
  {   



      }  
    
 }
