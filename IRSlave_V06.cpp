#include <IRremote.h>
#include <Wire.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
#define POWER 0x10EFD827 
#define A 0x10EFF807 
#define B 0x10EF7887
#define C 0x10EF58A7
#define UP 0x10EFA05F
#define DOWN 0x10EF00FF
#define LEFT 0x10EF10EF
#define RIGHT 0x10EF807F
#define SELECT 0x10EF20DF

void setup()
{
  Serial.begin(115200);
  Wire.begin(5);
  irrecv.enableIRIn(); // Start the receiver
  Wire.onRequest(requestEvent);
 
  Serial.println(F("IR remote."));
  Serial.println();
  
}

byte button = 0;

void loop() 
{
  if (irrecv.decode(&results)) 
  {
    if (results.value == POWER) 
    {
      button = 1;  
    }
     if (results.value == A) 
    {
      button = 2;

    }
     if (results.value == B) 
    {
      button = 3;

    }
     if (results.value == C) 
    {     
      button = 4;

    }
     if (results.value == UP) 
    {     
      button = 5;

    }
     if (results.value == DOWN) 
    {     
      button = 9;

    }
     if (results.value == LEFT) 
    {     
      button = 6;
    }
     if (results.value == RIGHT) 
    {     
      button = 8;
    }
     if (results.value == SELECT) 
    {     
      button = 7;
    }
    Serial.println(button);
    
    sendCode();
    irrecv.resume();

  }
}


void sendCode()
{
  if(button != 0)  
      Wire.write(button);     
}


void requestEvent()
{
    sendCode();
    button = 0;
}
