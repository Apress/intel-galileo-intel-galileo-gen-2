#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

char cmd_rsp[256];

/* Add here your consumer and OAuth string codes */
String consumer_key="";
String consumer_secret="";
String oauth_token="";
String oauth_secret="";

/* this is the name of script in the target 
   you need to push this script to the target before run this software */
String script_name = ". /home/root/twitter.sh ";
  
/* prototype */  
boolean post(String message);  
  
void setup() {
  
  Serial.begin(115200);
  
  delay(3000);  // just a delay to allow you to see the serial monitor messages
                // you can remove this delay
  
  String message="TWEETING!!!";
  post(message);
  
}

void loop() {
  delay(100);
}

boolean post(String message)
{
  
  boolean result = true;
  
  String command = script_name;
  
  command += consumer_key;
  command += " ";
  
  command += consumer_secret;
  command += " ";

  command += oauth_token;
  command += " ";

  command += oauth_secret;  
  command += " ";
  
  command += "'";
  command += message;
  command += "'";

   // buffer to be used with popen
   char cmd_char[1024];  
   
   // clear message buffer
   memset((void *)cmd_char, sizeof(cmd_char), 0);
   
   // convert the message to char array
   command.toCharArray(cmd_char, sizeof(cmd_char), 0);
   
   Serial.println(cmd_char);
   
   FILE *fpipe;
   
   if ( !(fpipe = (FILE*)popen((char *)cmd_char,"r")) ) 
      {  // If fpipe is NULL
      Serial.println("Problems with pipe");
      result = false;
      
   }
   else
   {
   
     while ( fgets( cmd_rsp, sizeof(cmd_rsp), fpipe)) {}
   
     pclose(fpipe);
   
     // checking for errors
     if ((strstr(cmd_rsp, "{\"errors\":[{\"message\"") != NULL) ||
         (strstr(cmd_rsp, "ERROR: No internet connection") != NULL))
     {
        Serial.println("OPS!!! ERROR!!");
        Serial.println(cmd_rsp);
        result = false;
     }
     else
     {
        Serial.println("POSTED WITH SUCCESS!");
        Serial.println(cmd_rsp);
     }  
   }
   
   return result;
}

