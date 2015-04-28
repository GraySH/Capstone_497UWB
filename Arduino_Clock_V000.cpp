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
const int buttonPinOne = 7;
const int buttonPinTwo = 6;
const int buttonPinUP = 5;
const int buttonPinLEFT = 4;
const int buttonPinRIGHT = 3;
const int buttonPinDOWN = 9;

int n = 1;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

bool alarmSet = false;
int myVal = 0;

int timeArr[6];

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
    //sync every hour
    if( second() % 30 == 0 && minute() % 30 == 0) 
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
    
    int buttonStateOne = digitalRead(buttonPinOne);
    int buttonStateTwo = digitalRead(buttonPinTwo);
    int buttonStateUP = digitalRead(buttonPinUP);
    int buttonStateDOWN = digitalRead(buttonPinDOWN);
    int buttonStateLEFT = digitalRead(buttonPinLEFT);
    int buttonStateRIGHT = digitalRead(buttonPinRIGHT);        


   if(!alarmSet)   
   {    
       hour = 1, minute = 2, second = 3, year = 2015, month = 4, day = 5;       
       Alarm.alarmRepeat(hour, minute, second, EveryDayAlarm);
   }

    //setting time
    if(buttonStateOne == LOW)
    {  
        alarmSet = true;
        delay(1000);
        lcd.clear();
        lcd.print("Setting TIME");
        setUserTime(&time_hour, &time_minute, &time_second, &time_year, &time_month, &day);
        delay(1000);        
    }

    //setting alarm
    if(buttonStateTwo == LOW)
    {  
        alarmSet = true;
        delay(1000);
        lcd.clear();
        lcd.println("Setting Alarm");
        setAlarmTime(&hour, &minute, &second);
        delay(1000);        
    }

    if(buttonStateUP == LOW)
        increaseVolume();

    if(buttonStateDOWN == LOW)
        decreaseVolume();

      if(buttonStateLEFT == LOW || buttonStateRIGHT == LOW)
      {
        lcd.home();
        lcd.clear();
        lcd.print("Stop Alarm");
        if ( MP3player.isPlaying() )
            MP3player.stopTrack();

        lcd.clear();
        digitalClockDisplay();
        delay(500);

      }
     
    //  lcd.setBacklight(LOW);      // Backlight off
    //  delay(3000);
    //  lcd.setBacklight(HIGH);     // Backlight on
    //  delay(3000);
      
      digitalClockDisplay();

       lcd.setCursor (0,2);     
        lcd.print("Alarm: ");
       lcd.print(hour);
        printDigitsLCD(minute);
        printDigitsLCD(second);

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
  printDigitsLCD(minute());
  printDigitsLCD(second());
  
    //display date
    lcd.setCursor ( 0, 1 );        // go to the 2nd line
    lcd.print( year() );
    lcd.print("/");
    lcd.print( month() );
    lcd.print("/");
    lcd.print( day() );
    
}

void printDigitsLCD(int digits)
{
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
void setUserTime(int* hour, int* minute, int* second, int* year, int* month, int* day)
{
     *hour = 0, *minute = 0, *second = 0;
     *year = 2015, *month = 1, *day = 1;

    int buttonStateOne = digitalRead(buttonPinOne);
    int buttonStateTwo = digitalRead(buttonPinTwo);
    int buttonStateUP = digitalRead(buttonPinUP);
    int buttonStateDOWN = digitalRead(buttonPinDOWN);
    int buttonStateLEFT = digitalRead(buttonPinLEFT);
    int buttonStateRIGHT = digitalRead(buttonPinRIGHT);        
    delay(1000);

    int val;			
    while(buttonStateOne == HIGH)
    {	

        lcd.home();	    	    
        //buttonStateOne = digitalRead(buttonPinOne);
        buttonStateTwo = digitalRead(buttonPinTwo);
        buttonStateUP = digitalRead(buttonPinUP);
        buttonStateDOWN = digitalRead(buttonPinDOWN);
        buttonStateLEFT = digitalRead(buttonPinLEFT);
        buttonStateRIGHT = digitalRead(buttonPinRIGHT);        

	    if( buttonStateRIGHT == LOW)		
	    {
	        val++;
//		    val = val % 6;
            if(val > 5)
                val = 5;
        }

	    if( buttonStateLEFT == LOW)		
	    {
	        val--;		    	    
            if(val < 0)
                val = 0;
        }

//        lcd.clear();
        switch (val)
        {
            case 0:             //hour change                        
                lcd.setCursor(10,1);
                lcd.print("Hour      ");		                    
	            if( buttonStateUP == LOW)		
	            {		        
		            *hour = *hour + 1;
		            if(*hour > 23)
		                *hour = 0;
	            }   
	            else if(buttonStateDOWN == LOW)
	            {
		            *hour = *hour - 1;
		            if(*hour < 0)
		                *hour = 23;		        
	            }		        
                break;
            case 1:             //minute change
                lcd.setCursor(10,1);
                lcd.print("Minute      ");		                    

	            if( buttonStateUP == LOW)		
	            {
		            *minute = *minute + 1;
		            if(*minute > 59)
		                *minute = 0;
	            }                           
	            else if(buttonStateDOWN == LOW)
	            {
		            *minute = *minute - 1;
		            if(*minute < 0)
		                *minute = 59;		        
	            }		        		        
                break;            
            case 2:             //second change
                lcd.setCursor(10,1);
                lcd.print("Second     ");		                    
	            if( buttonStateUP == LOW)		
	            {
		            *second = *second + 1;
		            if(*second > 59)
		                *second = 0;
	            }                                   
	            else if(buttonStateDOWN == LOW)
	            {
		            *second = *second - 1;
		            if(*second < 0)
		                *second = 59;		        
	            }		        		        		            
                break;            
            case 3:             //year change
                lcd.setCursor(10,1);
                lcd.print("Year     ");
	            if( buttonStateUP == LOW)				        
		            *year = *year + 1;		                                           
	            else if(buttonStateDOWN == LOW)
	            {
		            *year = *year - 1;
		            if(*year < 0)
		                *year = 0;		        
	            }		            
                		                                
                break;
            case 4:             //month change
                lcd.setCursor(10,1);
                lcd.print("Month     ");		                                
	            if( buttonStateUP == LOW)				        
		            *month = *month + 1;		                                           
	            else if(buttonStateDOWN == LOW)
	            {
		            *month = *month - 1;
		            if(*year < 0)
		                *year = 0;		        
	            }
                break;                
            case 5:             //day change
                lcd.setCursor(10,1);
                lcd.print("Day     ");		                                
	            if( buttonStateUP == LOW)				        
		            *day = *day + 1;		                                           
	            else if(buttonStateDOWN == LOW)
	            {
		            *day = *day - 1;
		            if(*day < 0)
		                *day = 0;		        
	            }	        
                break;                            
            default:
                break;
        
        }//switch

								
	    //show user setting time.
	    lcd.home();
//		lcd.clear();
	    lcd.print("Setting TIME: ");
	    setTime(*hour,*minute, *second, *day, *month, *year); 			
        
        //set rtc time.
        RTC.set(now());

        lcd.setCursor ( 0, 1 );        //go to the 2nd line
        lcd.print(*hour);
        printDigitsLCD(*minute);
        printDigitsLCD(*second);

        lcd.setCursor ( 0, 2 );
        lcd.print(*year); lcd.print("/"); lcd.print(*month);lcd.print("/");lcd.print(*day);


	
	    delay(200);
	    buttonStateOne = digitalRead(buttonPinOne);        
    }

//    delay(1000);
//	time_t t = now();
//	Serial.println("time set to: ");
//	digitalClockDisplay();

    lcd.home();
    lcd.clear();
    lcd.print("Time set to: ");
    lcd.setCursor ( 0, 1 );        // go to the 2nd line
    lcd.print(*hour);
    printDigitsLCD(*minute);
    printDigitsLCD(*second);

    lcd.setCursor ( 0, 2 );
    lcd.print(*year); lcd.print("/"); lcd.print(*month);lcd.print("/");lcd.print(*day);


//    Alarm.alarmRepeat(*hour, *minute, *second, EveryDayAlarm);
    delay(3000);
    lcd.clear();


}//setUserTime


void setAlarmTime(int* hour, int* minute, int* second)
{
     *hour = 0, *minute = 0, *second = 0;
//     *year = 2015, *month = 1, *day = 1;

    int buttonStateOne = digitalRead(buttonPinOne);
    int buttonStateTwo = digitalRead(buttonPinTwo);
    int buttonStateUP = digitalRead(buttonPinUP);
    int buttonStateDOWN = digitalRead(buttonPinDOWN);
    int buttonStateLEFT = digitalRead(buttonPinLEFT);
    int buttonStateRIGHT = digitalRead(buttonPinRIGHT);        
    delay(1000);

    int val;			
    while(buttonStateTwo == HIGH)
    {	

        lcd.home();	    	    
        //buttonStateOne = digitalRead(buttonPinOne);
        buttonStateTwo = digitalRead(buttonPinTwo);
        buttonStateUP = digitalRead(buttonPinUP);
        buttonStateDOWN = digitalRead(buttonPinDOWN);
        buttonStateLEFT = digitalRead(buttonPinLEFT);
        buttonStateRIGHT = digitalRead(buttonPinRIGHT);        


	    if( buttonStateRIGHT == LOW)		
	    {
	        val++;
//		    val = val % 6;
            if(val > 2)
                val = 2;
        }

	    if( buttonStateLEFT == LOW)		
	    {
	        val--;		    	    
            if(val < 0)
                val = 0;
        }


  //      lcd.clear();
        switch (val)
        {
            case 0:             //hour change                        
                lcd.setCursor(10,1);
                lcd.print("Hour    ");		                    
	            if( buttonStateUP == LOW)		
	            {		        
		            *hour = *hour + 1;
		            if(*hour > 23)
		                *hour = 0;
	            }   
	            else if(buttonStateDOWN == LOW)
	            {
		            *hour = *hour - 1;
		            if(*hour < 0)
		                *hour = 23;		        
	            }		        
                break;
            case 1:             //minute change
                lcd.setCursor(10,1);
                lcd.print("Minute    ");		                    

	            if( buttonStateUP == LOW)		
	            {
		            *minute = *minute + 1;
		            if(*minute > 59)
		                *minute = 0;
	            }                           
	            else if(buttonStateDOWN == LOW)
	            {
		            *minute = *minute - 1;
		            if(*minute < 0)
		                *minute = 59;		        
	            }		        		        
                break;            
            case 2:             //second change
                lcd.setCursor(10,1);
                lcd.print("Second    ");		                    
	            if( buttonStateUP == LOW)		
	            {
		            *second = *second + 1;
		            if(*second > 59)
		                *second = 0;
	            }                                   
	            else if(buttonStateDOWN == LOW)
	            {
		            *second = *second - 1;
		            if(*second < 0)
		                *second = 59;		        
	            }		        		        		            

            default:
                break;
        
        }//switch

								
	    //show user setting time.
	    lcd.home();
//		lcd.clear();
	    lcd.print("Setting Alarm: ");

        lcd.setCursor ( 0, 1 );        //go to the 2nd line
        lcd.print(*hour);
        printDigitsLCD(*minute);
        printDigitsLCD(*second);
	
	    delay(200);
	    buttonStateTwo = digitalRead(buttonPinTwo);        
    }

//    delay(1000);
//	time_t t = now();
//	Serial.println("time set to: ");
//	digitalClockDisplay();

    lcd.home();
    lcd.clear();
    lcd.print("Alarm set to:                ");
    lcd.setCursor ( 0, 1 );        // go to the 2nd line

    //show user setting time.
//	lcd.clear();
    lcd.print(*hour);
    printDigitsLCD(*minute);
    printDigitsLCD(*second);

    Alarm.alarmRepeat(*hour, *minute, *second, EveryDayAlarm);
//    Alarm.alarmRepeat(8,30,0, MorningAlarm);
    delay(3000);
    lcd.clear();


}//setUserTime

void increaseVolume()
{

    uint8_t volume = volume - 15;
    if( volume < 15 )
        volume = 15;
              
    MP3player.setVolume(volume, volume);           
    lcd.home();
    lcd.setCursor(0,3);
    lcd.print("                 ");
    lcd.setCursor(0,3);
    lcd.print(volume);    
    delay(1000);

}

void decreaseVolume()
{
    uint8_t volume = volume + 15;
    if( volume > 150 )
        volume = 150;              
        
    lcd.setCursor(0,3);
    lcd.print("                 ");
    lcd.setCursor(0,3);
    lcd.print(volume);    
    MP3player.setVolume(volume, volume);   
    delay(1000);

}


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



