#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h> 

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

//create and name the library object
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
const int buttonPinDOWN = 8;

int n = 1;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


bool alarmSet = false;
int myVal = 0;


void setup()
{
  lcd.begin (20,4);
  
// Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();                   // go home
  
  //Time Module
//  Serial.begin(9600);
  setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms 
//  Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
//  Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day 
//  Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday 

    if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) 
        sd.initErrorHalt(); // newly required in 1.01.00 and higher

    MP3player.begin();


}

void loop()
{
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
       hour = 8, minute = 29, second = 10, year = 2015, month = 1, day = 1;       
       Alarm.alarmRepeat(hour, minute, second, EveryDayAlarm);
   }

    //setting time
    if(buttonStateOne == LOW)
    {  
        alarmSet = true;
        delay(1000);
        lcd.clear();
        lcd.println("Setting TIME");
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


  if(buttonStateUP == LOW || buttonStateDOWN == LOW || buttonStateLEFT == LOW || buttonStateRIGHT == LOW)
//  if(buttonStateUP == LOW)
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

// functions to be called when an alarm triggers:
void MorningAlarm(){
//  Serial.println("Alarm: - turn lights off");    
}

//MY function.
void EveryDayAlarm()
{
  //Serial.println("Alarm: - EveryDay");    
  playMusic();  
//    char trackName[] = "track003.mp3";
//    result = MP3player.playMP3(trackName);        

}


void EveningAlarm(){
//  Serial.println("Alarm: - turn lights on");           
}

void WeeklyAlarm(){
//  Serial.println("Alarm: - its Monday Morning");      
}

void ExplicitAlarm(){
//  Serial.println("Alarm: - this triggers only at the given date and time");       
}

void Repeats(){
//  Serial.println("15 second timer");         
}

void OnceOnly(){
//  Serial.println("This timer only triggers once");  
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


//my function
void changeHour(int n)
{
    if(n == 1)
    {
      time_t t = now();  
      setTime(t +  3600);         
    }
    else
    {
        time_t t = now();  
        setTime(t -  3600);         
    }

}

void changeMinute(int n)
{
    if(n == 1)
    {
        time_t t = now();  
        setTime(t +  60);                  
    }
    else
    {
        time_t t = now();  
        setTime(t -  60);                  
    }
  
}

void changeSecond(int n)
{
    if(n == 1)
    {
        time_t t = now();  
        setTime(t +  1);                         
    }
    else
    {
        time_t t = now();  
        setTime(t -  1);                         
    }

}


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



