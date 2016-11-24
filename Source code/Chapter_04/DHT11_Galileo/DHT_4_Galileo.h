#ifndef DHT_4_GALILEO
#define DHT_4_GALILEO
#include <Arduino.h> 
#include <stdint.h>

class DHT_4_Galileo
{
  
public:
   
   // to report ACK status
   static const int DHT_ACK_OK = 0;
   static const int DHT_ACK_ERROR = -1;
   
   // to report reading status
   static const int DHT_READ_SUCCESS = 0;
   static const int DHT_READ_TIMEOUT_ERROR = -1;
   static const int DHT_READ_CHECKSUM_ERROR = -2;

   int humidity;
   int temperature;

    /* 
     * The class constructor must receive the pins 
     * used as gate/command and the pin to read
     * the data from sensor
     */
    DHT_4_Galileo(uint8_t p_gate, uint8_t p_read);
  
    /*
     *  sendCommand():
     *  sends the commands and wait for the acknolodge from the sensor
     *
     */ 
    int sendCommand();  // end of send_command()
  
  
    /*
     *  read():
     *  Reads the 40 bits of data, parse the data in 
     *  temperature and humidity
     */
     int read ();
  
private:
   int pin_gate;
   int pin_read;
   
   /*
    * getFastIOMacro(int pin)
    * only an utility function to return
    * the right macro according the pin
    */
   static int getFastIOMacro(int pin);

}; // end of class
#endif 