#include "Wire.h" 

int PIN = 9; 

void setup() 
{ 
 
   // Set divider to get 122.533Hz freq. 
   Wire.beginTransmission(0x20); 
   Wire.write(0x2C); 
   Wire.write(0x03); 
   Wire.endTransmission(); 
 
   // Select PWM source clock 
   Wire.beginTransmission(0x20); 
   Wire.write(0x29); 
   Wire.write(0x04); 
   Wire.endTransmission(); 
 
   // Set period register 
   Wire.beginTransmission(0x20); 
   Wire.write(0x2a); 
   Wire.write(0xff); 
   Wire.endTransmission(); 
 
   // Duty cycle of 32us @ 122.533hz 
   Wire.beginTransmission(0x20); 
   Wire.write(0x2b); 
   Wire.write(0x01);  // 1 is the minimum granularity
   Wire.endTransmission(); 
 
} 

void loop() 
{   
} 

