#include <signal.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/time.h> 

// the led
int led = 2;  // the built-in LED

// emails counters
int current_emails_counter = -1;
int last_emails_counter = -1;


// add here your credentials
String gmailUserName="";  // email or password
String gmailPassword="";  // your password

String script_name = "python /home/root/emailCounter.py ";

// this function calls a python script to read the number of emails not read
int processEmailCounter()
{
  
   char cmd_rsp[8];  // This buffer will containg the script response
   
   FILE *fpipe;
   
    
   String command = script_name;
   
   command += gmailUserName;
   command += " ";
  
   command += gmailPassword;
  
    // buffer to be used with popen
   char cmd_char[300];  
   
   // clear message buffer
   memset((void *)cmd_char, sizeof(cmd_char), 0);
   
   // convert the message to char array
   command.toCharArray(cmd_char, sizeof(cmd_char), 0);
   
   if ( !(fpipe = (FILE*)popen((char *)cmd_char,"r")) ) 
      {  // If fpipe is NULL
      Serial.println("Problems with pipe");      
   }
   else
   {
   
     while ( fgets( cmd_rsp, sizeof(cmd_rsp), fpipe)) {}
   
     pclose(fpipe);
   
     // let's print the serial result
     Serial.println(cmd_rsp);
     
     return atoi(cmd_rsp);
   }
   
   return -1;
}


// this is my time handler...
void timerHandler (int signum) 
{  
    current_emails_counter = processEmailCounter();
    if (last_emails_counter == -1) 
    {
        last_emails_counter = current_emails_counter;
        Serial.println("I am ready to check now... ");
    }
    
    if (current_emails_counter != last_emails_counter)
    {
        // turn on the LED
        digitalWrite(led, HIGH);
    }
} 
 
int setAlarm () 
{
  struct sigaction sa; 
  struct itimerval timer; 
 
  /* Install timer_handler as the signal handler for SIGVTALRM.  */ 
  memset (&sa, 0, sizeof (sa)); 
  sa.sa_handler = &timerHandler; 
  sigaction (SIGVTALRM, &sa, NULL); 
 
  // Configure the timer to expire after 1 seconds 
  timer.it_value.tv_sec = 1; 
  timer.it_value.tv_usec = 0; 
  // ... and every 10 seconds after that 
  timer.it_interval.tv_sec = 10; 
  timer.it_interval.tv_usec = 0; 
  
  // Start a virtual timer. Counter while sketch runs
  setitimer (ITIMER_VIRTUAL, &timer, NULL); 
 
} 


void setup() {
   // only a small delay to allow you press CTRL+SHIT+M
   // and see the Serial Monitor
   delay(3000);
  
   // set the alarm
   setAlarm();
   pinMode(led, OUTPUT);
   digitalWrite(led, LOW);
}

void loop() {
  // put your main code here, to run repeatedly: 
 
  
}
