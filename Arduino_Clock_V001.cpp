#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h> 

//MP3 Shield Library
#include <SFEMP3Shield.h>

//RealTime clock
#include <DS1307RTC.h>

//name the library object
SFEMP3Shield MP3player;
SdFat sd;
SdFile file;

byte temp;
byte result;

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

const int ledPin = 13;

int n = 1;
byte button = 0;
bool flag = false;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

bool alarmSet = false;
int myVal = 0;
int timeArr[6];

//alarm time
int alarm_hour = 0, alarm_minute = 0, alarm_second = 0, alarm_year = 0, alarm_month = 0, alarm_day = 0;

void setup()
{
    Wire.begin(); 
    Serial.begin(9600);

  lcd.begin (20,4);
  
// Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();                   // go home
  
  //Time Module
//  Serial.begin(9600);
//      setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms 
//  Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
//  Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day 
//  Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday 

    if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) 
        sd.initErrorHalt(); // newly required in 1.01.00 and higher

    MP3player.begin();

    setTime(RTC.get());                 
                    
}
   
//        increaseVolume();

void loop()
{
    getButton();   
    if(flag)
    {
        lcd.setCursor(0,3);
        lcd.print(button); 
        delay(500);                  
        flag = false;
    }
    
    //sync time once for a day
    if( second() % 55 == 0 && minute() % 55 == 0 && hour() % 23 == 0)
    // if( minute() % 10 == 0)  
        gpsSync();

//---------------
    // Backlight on/off every 3 seconds
    //  lcd.setCursor (14,3);        // go col 14 of line 3
    //  lcd.print(n++,DEC);
            
    if ( MP3player.isPlaying() )
    {
        lcd.setCursor(0,3);
        lcd.print("Playing Alarm...");
    }
    else
    {
      lcd.setCursor(0,3);
      lcd.print("                 "); 
    }

    lcd.home();
    lcd.setCursor(0,3);
    //    lcd.println("Test");
        
    //int hour = 0, minute = 0, second = 0, year = 2015, month = 1, day = 1;
    int hour, minute, second, year, month, day;
    int time_hour, time_minute, time_second, time_year, time_month, time_day;          
  
   if(!alarmSet)   
   {    
       alarm_hour = 1, alarm_minute = 2, alarm_second = 3, alarm_year = 2015, alarm_month = 4, alarm_day = 5;       
       Alarm.alarmRepeat(alarm_hour, alarm_minute, alarm_second, EveryDayAlarm);
       alarmSet = true;
   }

    //setting time
    if(button == 2)
    {     
        lcd.clear();
        lcd.print("Setting TIME");

        delay(1000);
        lcd.clear();    

//      setUserTime(&time_hour, &time_minute, &time_second, &time_year, &time_month, &day);
        setUserTime();
        
        lcd.clear();       
    }

    //setting alarm
    if(button == 3)
    {  
//        alarmSet = true;
        lcd.clear();
        lcd.print("Setting Alarm");
        delay(1000);
        
        lcd.clear();        
        setAlarmTime();
        lcd.clear();                
        
    }


      if(button == 7)
      {      
        if ( MP3player.isPlaying() )
        {
            MP3player.stopTrack();
            lcd.home();
            lcd.clear();
            lcd.print("Stop Alarm");            
            lcd.clear();
            digitalClockDisplay();
            delay(500);
        }
      }
     
    //  lcd.setBacklight(LOW);      // Backlight off
    //  delay(3000);
    //  lcd.setBacklight(HIGH);     // Backlight on
    //  delay(3000);
      
      digitalClockDisplay();

        lcd.setCursor (0,2);     
        lcd.print("Alarm: ");
        lcd.print(alarm_hour);
        printDigitsLCD(alarm_minute, 1);
        printDigitsLCD(alarm_second, 1);

      Alarm.delay(1000); // wait one second between clock display      
}

void playMusic()
{
    char trackName[] = "track003.mp3";
    result = MP3player.playMP3(trackName);        
    
//    char title[10];
//    MP3player.trackTitle((char*)&title);
//    lcd.write((byte*)&title, 30);
//    lcd.print("playingALarm");                
}

//MY function.
void EveryDayAlarm()
{
  //Serial.println("Alarm: - EveryDay");    
  playMusic();  
//    char trackName[] = "track003.mp3";
//    result = MP3player.playMP3(trackName);        
}



void digitalClockDisplay()
{
// digital clock display of the time to serial
//  Serial.print(hour());
//  printDigits(minute());
//  printDigits(second());
//  Serial.println(); 

  //Display to LCD
//  lcd.clear();
  lcd.home();
  lcd.setCursor ( 0, 0 );
  lcd.print(hour());    
  printDigitsLCD(minute() , 1);
  printDigitsLCD(second(), 1);
  
    //display date
    lcd.setCursor ( 0, 1 );        // go to the 2nd line
    lcd.print( year() );
    lcd.print("/");
    lcd.print( month() );
    lcd.print("/");
    lcd.print( day() );
    
}

//print digit, no colon with option 0;
void printDigitsLCD(int digits, int option)
{

  if(option != 0)
     lcd.print(":");

  if(digits < 10)      
    lcd.print('0');
    
  lcd.print(digits);
}


void printDigits(int digits)
{


    Serial.print(":");

  if(digits < 10)
  {
    Serial.print('0');
    lcd.print('0');
  }
  Serial.print(digits);
}


//myfunction
//setUserTime(&time_hour, &time_minute, &time_second, &time_year, &time_month, &day);
void setUserTime()
{
// &time_hour, &time_minute, &time_second, &time_year, &time_month, &day
// setTime(hour,minute,second,day,month,year); // set time to Saturday 8:29:00am Jan 1 2011    

    //clear alarm time spot.
    lcd.clear();

    
    int display_hour = hour();
    int display_minute = minute();
    int display_second = 0;
    int display_year = year();
    int display_month = month();
    int display_day = day();    
    
    button = 0;  
      
    int currentValue = 1;
       
                             
    while(true)
    {
      //get button value to change time
      if(button == 6)
      {
        currentValue = currentValue - 1;
        if(currentValue < 1)
            currentValue = 1;
                   
        button = 0;
      }

      if(button == 8)
      {
        currentValue = currentValue + 1;
        if(currentValue > 6)
            currentValue = 6;
            
         button = 0;
      }
       
       
//      lcd.noBlink();
//      delay(1000);
            
      lcd.home();       
      //digitalClockDisplay();               
      //display currentTime.
      displayTime(display_hour, display_minute, display_second, display_year, display_month,  display_day  );    
        
      //move cursor for time setting.
      //hour -> min -> second -> year -> month -> day.        
      
      switch (currentValue)
      {
        case 1:
            //show hour char
            lcd.setCursor(10, 0);
            lcd.print("HOUR     ");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
               lcd.setCursor(2,0);
            
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_hour = display_hour + 1;
                    if(display_hour >= 24)
                        display_hour = 0;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_hour = display_hour - 1;
                    if(display_hour < 0)
                        display_hour = 23;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while 
            break;      

      case 2:
            //show hour char
            lcd.setCursor(10, 0);
            lcd.print("MINUTE");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
//               lcd.setCursor(2,0);
           
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_minute = display_minute + 1;
                    if(display_minute >= 60)
                        display_minute = 0;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_minute = display_minute - 1;
                    if(display_minute < 0)
                        display_minute = 59;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while 
      
      
            break;

      case 3:
            //show  char
            lcd.setCursor(10, 0);
            lcd.print("SECOND ");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
//               lcd.setCursor(2,0);
           
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_second = display_second + 1;
                    if(display_second >= 60)
                        display_second = 0;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_second = display_second - 1;
                    if(display_second < 0)
                        display_second = 59;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while 
      
      
            break;            

      case 4:
            //show  char
            lcd.setCursor(10, 0);
            lcd.print("YEAR     ");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
               lcd.setCursor(2,0);
            
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_year = display_year + 1;
                    if(display_year >= 2050)
                        display_year = 2010;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_year = display_year - 1;
                    if(display_year < 2010)
                        display_year = 2050;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while       
      
      
            break;            

      case 5:
            //show hour char
            lcd.setCursor(10, 0);
            lcd.print("MONTH   ");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
               lcd.setCursor(2,0);
            
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_month = display_month + 1;
                    if(display_month >= 12)
                        display_month = 1;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_month = display_month - 1;
                    if(display_month < 1)
                        display_month = 12;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while 
      
      
            break;            

      case 6:
            //show char
            lcd.setCursor(10, 0);
            lcd.print("DAY   ");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
               lcd.setCursor(2,0);
            
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_day = display_day + 1;
                    if(display_day >= 31)
                        display_day = 1;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_day = display_day - 1;
                    if(display_day < 1)
                        display_day = 31;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while 
            
            break;            

      default:
            break;      
      }                      
      if(button == 7)
         break;            
            
    }//while
    
    setTime(display_hour,display_minute,display_second,display_day,display_month,display_year); // set time to Saturday 8:29:00am Jan 1 2011
    RTC.set(now());                 
        
    lcd.home();
    lcd.clear();
    digitalClockDisplay();
        
    button = 0;            
}//setUserTime


void gpsSync()
{
    //get information from gps slave.
    Wire.requestFrom(4, 7);    // request 3 bytes from slave device #2
    int tmp = Wire.read();
    if(tmp != 0)
    {        
        for(int i=1;i<7;i++)
        {
            int c = Wire.read(); // receive a byte as time data.
            timeArr[i - 1] = c;                                     
   
        }
        int hour = timeArr[3];
        int minute = timeArr[4];
        int second = timeArr[5];
        int day = timeArr[0];
        int month = timeArr[1];
        int year = timeArr[2];

        //setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011
        setTime(hour,minute,second,day,month,year); // set time to Saturday 8:29:00am Jan 1 2011
        RTC.set(now());                
     }
}

void getButton()
{
    Wire.requestFrom(5, 1);    // request 1 bytes from slave device #5
    byte tmp = Wire.read();
 
    if ( tmp > 0 && tmp < 10)
    { 
        button = tmp;           
        flag = true;
    }
    else
    {
        button = 0;   
        flag = false;
    }
    
}


void displayTime(int display_hour, int display_minute, int display_second, int display_year, int display_month, int display_day )
{
    lcd.home();
    lcd.setCursor ( 0, 0 );
    //lcd.print(display_hour);    
    printDigitsLCD(display_hour, 0);    
    printDigitsLCD(display_minute, 1);
    printDigitsLCD(display_second, 1);

    //display date
    lcd.setCursor ( 0, 1 );        // go to the 2nd line
    lcd.print( display_year );
    lcd.print("/");
//    lcd.print( display_month );
    printDigitsLCD(display_month, 0);
    lcd.print("/");
//    lcd.print( display_day );
    printDigitsLCD(display_day, 0);

}//display Time on lcd.


void setAlarmTime()
{
    //clear alarm time spot.
    lcd.clear();
    
    int display_hour = alarm_hour;
    int display_minute = alarm_minute;
    int display_second = 0;
    int display_year = alarm_year;
    int display_month = alarm_month;
    int display_day = alarm_day;    
    
    button = 0;  
      
    int currentValue = 1;
     
                             
    while(true)
    {
      //get button value to change time
      if(button == 6)
      {
        currentValue = currentValue - 1;
        if(currentValue < 1)
            currentValue = 1;
                   
        button = 0;
      }

      if(button == 8)
      {
        currentValue = currentValue + 1;
        if(currentValue > 3)
            currentValue = 3;
            
         button = 0;
      }
       
       
//      lcd.noBlink();
//      delay(1000);
            
      lcd.home();       
      //digitalClockDisplay();               
      //display currentTime.
      displayTime(display_hour, display_minute, display_second, display_year, display_month,  display_day  );    
        
      //move cursor for time setting.
      //hour -> min -> second -> year -> month -> day.        
      
      switch (currentValue)
      {
        case 1:
            //show hour char
            lcd.setCursor(10, 0);
            lcd.print("HOUR     ");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
               lcd.setCursor(2,0);
            
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_hour = display_hour + 1;
                    if(display_hour >= 24)
                        display_hour = 0;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_hour = display_hour - 1;
                    if(display_hour < 0)
                        display_hour = 23;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while 
            break;      

      case 2:
            //show hour char
            lcd.setCursor(10, 0);
            lcd.print("MINUTE");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
//               lcd.setCursor(2,0);
           
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_minute = display_minute + 1;
                    if(display_minute > 59)
                        display_minute = 0;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_minute = display_minute - 1;
                    if(display_minute < 0)
                        display_minute = 59;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while 
      
      
            break;

      case 3:
            //show  char
            lcd.setCursor(10, 0);
            lcd.print("SECOND ");

                                                
            //change time by using up and down key
            while(true)
            {                        
            
//               lcd.setCursor(2,0);
           
               getButton();
               if(button == 7 || button == 6 || button == 8)
                 break;            
                
                if(button == 5) //if up key is pressed, increase value
                {
                    display_second = display_second + 1;
                    if(display_second > 59)
                        display_second = 0;
                }                                          
            
                if(button == 9) //if down key is pressed, decrease value
                {
                    display_second = display_second - 1;
                    if(display_second < 0)
                        display_second = 59;
                }                              
                
                displayTime (display_hour, display_minute, display_second, display_year, display_month,  display_day  );
                
            delay(100);

            }//while       
      
            break;            

      default:
            break;      
      }                      
      if(button == 7)
         break;            
            
    }//while
            
    alarm_hour = display_hour;
    alarm_minute = display_minute;
    alarm_second = display_second;
            
    Alarm.alarmRepeat(display_hour, display_minute, display_second, EveryDayAlarm);                
    lcd.home();
    lcd.clear();
    digitalClockDisplay();        
    button = 0;            

}//AlarmTime

