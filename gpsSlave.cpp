#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 9, TXPin = 10;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

int DSTbegin[] = { //DST 2013 - 2025 in Canada and US
  310, 309, 308, 313, 312, 311, 310, 308, 314, 313, 312, 310, 309};
int DSTend[] = { //DST 2013 - 2025 in Canada and US
  1103, 1102, 1101, 1106, 1105, 1104, 1103, 1101, 1107, 1106, 1105, 1103, 1102};

int DaysAMonth[] = { //number of days a month
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


int gpsYear, gpsMonth, gpsDay, gpsHour, gpsMin, gpsSec;
//byte gpsMin, byte gpsSec;

const int TimeZone = -8;

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Wire.begin(4);                // join i2c bus with address #4


    Wire.onRequest(requestEvent);
//  Wire.onReceive(receiveEvent); // register event
 
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
// This sketch displays information every time a new sentence is correctly encoded.
//  while (ss.available() > 0)
//    if (gps.encode(ss.read()))
//      displayInfo();

    while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {      
        timeAdjust();
        smartdelay(1000);        
    }

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        while(true);
    }
    
    
//    smartdelay(1000);
    
}

void displayInfo()
{

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
//    if (gps.time.centisecond() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
  
    smartdelay(1000);
}


static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  }
  while (millis() - start < ms);
}

void timeAdjust()
{
      if (gps.time.isValid() && gps.date.isValid() )    
      {      
      
        gpsYear = gps.date.year();
        gpsMonth = gps.date.month();
        gpsDay = gps.date.day();
        gpsHour = gps.time.hour();
        gpsMin = gps.time.minute();
        gpsSec = gps.time.second();
      
        //Timezopne Adjustment
        gpsHour = gps.time.hour();
        gpsHour += TimeZone;
                        
        //DST adjustment
        if (gpsMonth*100+gpsDay >= DSTbegin[gpsYear-13] && 
            gpsMonth*100+gpsDay < DSTend[gpsYear-13]) gpsHour += 1;

          if (gpsHour < 0)
          {
            gpsHour += 24;
            gpsDay -= 1;
            if (gpsDay < 1)
            {
              if (gpsMonth == 1)
              {
                gpsMonth = 12;
                gpsYear -= 1;
              }
              else
              {
                gpsMonth -= 1;
              }
              gpsDay = DaysAMonth[gpsMonth-1];
            } 
          }
          if (gpsHour >= 24)
          {
            gpsHour -= 24;
            gpsDay += 1;
            if (gpsDay > DaysAMonth[gpsMonth-1])
            {
              gpsDay = 1;
              gpsMonth += 1;
              if (gpsMonth > 12) gpsYear += 1;
            }
          }      
      
          Serial.print(gpsDay);
          Serial.print("/");
          Serial.print(gpsMonth);
          Serial.print("/");
          Serial.print(gpsYear);
          Serial.print(" ");
          Serial.print(gpsHour);
          Serial.print(":");
          Serial.print(gpsMin);
          Serial.print(":");
          Serial.println(gpsSec);             

       }//if

}


byte* timeReturn()
{

       //time array
       byte* timeArr = new byte[7];

      if (gps.time.isValid() && gps.date.isValid() )    
      {      
      
        gpsYear = gps.date.year();
        gpsMonth = gps.date.month();
        gpsDay = gps.date.day();
        gpsHour = gps.time.hour();
        gpsMin = gps.time.minute();
        gpsSec = gps.time.second();
      
        //Timezopne Adjustment
        gpsHour = gps.time.hour();
        gpsHour += TimeZone;
                        
        //DST adjustment
        if (gpsMonth*100+gpsDay >= DSTbegin[gpsYear-13] && 
            gpsMonth*100+gpsDay < DSTend[gpsYear-13]) gpsHour += 1;

          if (gpsHour < 0)
          {
            gpsHour += 24;
            gpsDay -= 1;
            if (gpsDay < 1)
            {
              if (gpsMonth == 1)
              {
                gpsMonth = 12;
                gpsYear -= 1;
              }
              else
              {
                gpsMonth -= 1;
              }
              gpsDay = DaysAMonth[gpsMonth-1];
            } 
          }
          if (gpsHour >= 24)
          {
            gpsHour -= 24;
            gpsDay += 1;
            if (gpsDay > DaysAMonth[gpsMonth-1])
            {
              gpsDay = 1;
              gpsMonth += 1;
              if (gpsMonth > 12) gpsYear += 1;
            }
          }      
          timeArr[0] = 1;
          timeArr[1] = gpsDay;
          timeArr[2] = gpsMonth;  
          timeArr[3] = gpsYear - 2000;  
          timeArr[4] = gpsHour;  
          timeArr[5] = gpsMin;  
          timeArr[6] = gpsSec;  

          return timeArr;

       }//if
       else
       {
            for(int i = 0; i < 7; i++)
                timeArr[i] = 0;
                
            return timeArr;
       }

}


//void receiveEvent(int howMany)
//{  
//    Wire.beginTransmission();  
//    Wire.write( timeReturn(), 6 );
//    Wire.endTransmission();
//  
//}

void requestEvent()
{
 
      Wire.write(timeReturn(), 7);
   
}

