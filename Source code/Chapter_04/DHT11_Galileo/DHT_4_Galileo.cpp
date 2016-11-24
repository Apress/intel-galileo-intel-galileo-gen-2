#include "DHT_4_Galileo.h"
#define DEBUG 1  // change to 0 if you do not want debug messages

    /* 
     * The class constructor must receive the pins 
     * used as gate/command and the pin to read
     * the data from sensor
     */
    DHT_4_Galileo::DHT_4_Galileo(uint8_t p_gate, uint8_t p_read)
    {
         pin_gate = p_gate;
         pin_read = p_read;
         
         pinMode(pin_gate, OUTPUT_FAST);
         pinMode(pin_read, INPUT_FAST);
    };
  
    /*
     *  sendCommand():
     *  sends the commands and wait for the acknolodge from the sensor
     *
     */ 
    int DHT_4_Galileo::sendCommand()
    {
      
       // pull down during 18 microseconds.. this is our command!
       fastGpioDigitalWrite(getFastIOMacro(pin_gate), LOW);
       delay(18);
       fastGpioDigitalWrite(getFastIOMacro(pin_gate), HIGH);  // High impedance
       delayMicroseconds(40);
    
    
       // now let's check if some ACK was received
       unsigned long t0,ti;

       int state = fastGpioDigitalRead(getFastIOMacro(pin_read));
       int new_state = state;

       boolean ack = false;
       t0 = micros(); // number microseconds since booted 
       while (micros()-t0<80)
       {
            new_state = fastGpioDigitalRead(getFastIOMacro(pin_read));
           if (new_state==0) 
           {
               // cool!!! first ACK received during 80 microseconds
               ack = true;
               break;
           }
       }

       if (!ack) 
       {
#if (DEBUG)           
          Serial.println("problem in FIRST PART OF ACK");
#endif
          return DHT_ACK_ERROR;
       }
  
       ack=false;
       t0 = micros(); // number microseconds since booted 
       while (micros()-t0 < 80)
       {
           // waiting for HIGH
           new_state = fastGpioDigitalRead(getFastIOMacro(pin_read));
           if (new_state!=0) 
           {  
              // the second ACK received!!! let's wait for the data!!!
              ack = true;
              break;
           }
       }
  
  
       if (!ack) 
       { 
#if (DEBUG)         
           Serial.println("problem in SECOND PART ACK"); 
#endif           
           return DHT_ACK_ERROR;
       }
  
       return DHT_ACK_OK;
    }  // end of send_command()
  
  
    /*
     *  read():
     *  Reads the 40 bits of data, parse the data in 
     *  temperature and humidity
     */
     int DHT_4_Galileo::read () {
  
        unsigned long t0;
   
            // BUFFER TO RECEIVE
        uint8_t bits[5];
        uint8_t cnt = 7;
        uint8_t idx = 0;

        
        int start_bit[40];
        int reading_bit[40];
        
        
        // cleaning arrays
        for (int i=0; i<40; i++)
        {
          start_bit[i] = 0;
          reading_bit[i] = 0;
        }
        
        for (int i=0; i<5; i++)
        {
          bits[i] = 0;
        }
        
        
         // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
        for (int i=0; i<40; i++)
        {
          
                //start bit
                // will stay low for 50us
                
                t0 = micros(); // number microseconds since booted
                while(fastGpioDigitalRead(getFastIOMacro(pin_read)) == 0)
                {
				    // using 70 instead 50 us due to the remaining
					// state of last ack
                    if ((micros() - t0) > 70) {
                        return DHT_READ_TIMEOUT_ERROR;
                    }
                }

                start_bit[i] = micros() -t0;

                t0 = micros(); // number microseconds since booted


                //reading bit
                // 26 to 28us  -> 0
                // up tp 70 us -> 1
                while( fastGpioDigitalRead(getFastIOMacro(pin_read)) != 0) {
                        if ((micros() - t0) > 77) {
                             return DHT_READ_TIMEOUT_ERROR;
                    }
                };

                unsigned long delta_time =  micros() - t0;
                reading_bit[i] = delta_time;

                if (delta_time > 50) bits[idx] |= (1 << cnt);
                if (cnt == 0)   // next byte?
                {
                        cnt = 7;    // restart at MSB
                        idx++;      // next byte!
                }
                else cnt--;
        }
        
        // dump
#if (DEBUG)         
        Serial.println();
        for (int i=0; i<40; i++)
        {
          Serial.print(i);
          Serial.print("   ");
          Serial.print(start_bit[i]);
          Serial.print("   ");
          Serial.print(reading_bit[i]);
          Serial.print("   ");
          if (reading_bit[i] > 40)
            Serial.println("1");
          else
            Serial.println("0");  
        }

        Serial.println();
        Serial.println("BYTES PARSED:");
        Serial.println("------------");
        
        for (int i=0; i<5; i++)
        {
          Serial.print(i);
          Serial.print(": ");
          Serial.println(bits[i]);
        }
		Serial.println();
#endif            
        // parsing the bits
        humidity    = bits[0]; 
        temperature = bits[2]; 
        uint8_t sum = (bits[0] +  bits[1] + bits[2] +  bits[3]) & 0xff;  

        if (bits[4] != sum) 
        {
          return DHT_READ_CHECKSUM_ERROR;
        } else {
		  return DHT_READ_SUCCESS;
		}
    

}

  
   /*
    * getFastIOMacro(int pin)
    * only an utility function to return
    * the right macro according the pin
    */
   int DHT_4_Galileo::getFastIOMacro(int pin)
   {
        int macro;
        switch (pin)
        {
#if PLATFORM_ID != 0x06
             // this is Galileo Gen 2
            case 0: macro = GPIO_FAST_IO0;
              break;              
            case 1: macro = GPIO_FAST_IO1;
               break;
#endif               
            case 2: macro = GPIO_FAST_IO2;
               break;
            case 3: macro = GPIO_FAST_IO3;
               break;
#if PLATFORM_ID != 0x06               
            // this is Galileo Gen 2 - no fast I/O for pins 7 and 8
            case 4: macro = GPIO_FAST_IO4;
               break;
            case 5: macro = GPIO_FAST_IO5;
               break;
            case 6: macro = GPIO_FAST_IO6;
               break;
            case 9: macro = GPIO_FAST_IO9;
               break;
            case 10: macro = GPIO_FAST_IO10;
               break;
            case 11: macro = GPIO_FAST_IO11;
               break;
            case 12: macro = GPIO_FAST_IO12;
               break;
            case 13: macro = GPIO_FAST_IO13;
               break;
#endif               
            default:
                     macro = 0;
               break;   
           
        }
     
        return macro;
   } // end of getFastIOMacro()
