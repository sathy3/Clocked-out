#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "motion.h"
#include "ranger.h"
#include "buzzer.h"
#include "ras.h"

// The running state of the system
enum{
    Run, Set, alarmSet
	}  sysState = Run;
enum{
    On, stand, disabled
} alarmState=stand;
enum{
    showTime, showAlarm, showTimeout
} display_s = showTime;
enum{
    setTime, setAlarm,
} set_mode=setTime;
enum{
    Off, detected, not_detected, not_present
} personState= detected;

typedef enum {Ac, Ic} OnOff_t;

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {
    0, 0, 0, 0, 1
};
seg7Display_t ttime= {0,0,0,0,1};

extern timeIncrement(seg7Display_t *);
volatile uint32_t sec= 0;
volatile uint32_t seated=0;
static uint32_t limit=7200000;// approx 2 hrs


void chkPerson(uint32_t time){
    static uint8_t times=0;

    uint32_t distance = rangerDetect2() * 0.34 / 98.6;      // result in mm, converts using a factor of 34,300 and then dividing to adjust the scale
    if (personState!=Off)
    {
        if (distance < 1500 )
        {
            personState=detected;
            seated=seated+3000;
            times=0;

        }
        else
        {   personState=not_present;
            if(motionSensor()==0)
            {
                if(times<255)
                {
                    times++;
                }
                else if(times==255)
                {
                    seated=0;
                    times=0;
                }

            }
            else
            {
              seated=seated+3000;
            }
         }
        if(seated==0)
        {
            personState=not_detected;
        }
        }
        ;
        if(seated>=limit&&alarmState!=disabled)
        {
            alarmState=On;

        }
        else if(alarmState==disabled)
        {
            ;
        }
        else{
            alarmState=stand;
        }

        //unit test
        if(personState==detected||personState==not_present)
        {
        uprintf("detected, times=%d,\n\r\t",times);
        }
        else
        {
            uprintf("not detected or Off\n\r\t");
        }
        //endunittest
    schdCallback(chkPerson, time + 3000);
}


void
stopWatchUpdate(uint32_t time)                          // The scheduled time
{
    // Update clock and display, only if the stopwatch is running
    if ((sysState != Set) && sec>=60 ) {
        timeIncrement(&seg7Display);
        sec=0;


    }
    if(sysState!=Set || set_mode!=setTime)
        {
            sec++;
        }
    else{
        sec=0;
    }
    // Call back after 1 seconds
    schdCallback(stopWatchUpdate, time + 1000);
}

void
checkPushButton(uint32_t time)
{
    // Read the pushbutton state; see pbRead() in launchpad.h
    int code = pbRead();
    uint32_t delay = 10;

    switch (code) {
    case 1:
		if(sysState==Set)
		{	
			sysState=Run;
		}
		else if( sysState==Run) {
		sysState=alarmSet;
		}
		else{
		sysState=Set;
		}
		delay = 250;
        break;

    case 2:
		if( set_mode==setTime)
		{
			set_mode=setAlarm;
		}
		else
		{
			set_mode=setTime;
		}		
        delay = 250;
        break;
    }
	//unit test
	if( code!=0)
	{
		uprintf("current state=%d %d", sysState, set_mode);
		
	}
    schdCallback(checkPushButton, time + delay);
}

// Buzzer state
static OnOff_t buzzerState = Ic;

// Callback function for playing the buzzer
void 
callbackBuzzerPlay(uint32_t time)   
{
    uint32_t delay = 10;

    if (alarmState != disabled )
    {
        if(alarmState==On&&personState== detected)
        // Turn the buzzer on and off alternatively
        // Adjust the time values to control the sound intensity
        switch (buzzerState)
        {
        case Ac:
            buzzerOff();
            buzzerState = Ic;
            delay = 2988;                           // off for 2988 ms
            break;

        case Ic:
            buzzerOn();
            buzzerState = Ac;
            delay = 12;                            // on for 12 ms
            break;
        }
    }

    // schedule the next callback
    schdCallback(callbackBuzzerPlay, time + delay);
}

void 
callbackdispHandler(uint32_t time)
{	int temp=(limit-seated)/60000;
	static uint32_t value=0; 
	rasRead(&value);
	
	switch(sysState)
	{	case Run: 
	    if(set_mode==setTime)
	{ledTurnOnOff(false /* red */, false /* blue */, true /* green */);}
	    else
	    {ledTurnOnOff(false /* red */, true /* blue */, true /* green */);}

	seg7DigitUpdate(&seg7Display);
		break;
	case Set:
	ledTurnOnOff(false /* red */, true /* blue */, false /* green */);
		if(set_mode==setTime )
		{	value= (value*60)/4096;
			seg7Display.d1=value%10;
			seg7Display.d2=value/10;
		}
		else
		{
			value= (value*24)/4096;
			if(value>12)
			{	value=value-12;
				seg7Display.colon=0;
				seg7Display.d3=value%10;
				seg7Display.d4=value/10;
			}
			else
			{
			    seg7Display.colon=1;
				seg7Display.d3=value%10;
				seg7Display.d4=value/10;
			}
		}
		seg7DigitUpdate(&seg7Display);
		break;
		
	case alarmSet:

		if(set_mode==setTime)
		{	
			ledTurnOnOff(true /* red */, false /* blue */, false /* green */);
			value= (value*480)/4096;
			limit= value*60000;
			ttime.d1=(value%60)%10;
			ttime.d2=(value%60)/10;
			ttime.d3=(value/60)%10;
			ttime.d4=0;
			
		}
		else
		{	
			ledTurnOnOff(true /* red */, true /* blue */, false /* green */);

            if (temp>=0)
            {
                ttime.d1=(temp%60)%10;
                ttime.d2=(temp%60)/10;
                ttime.d3=(temp/60)%10;;
            ttime.d4=0;
            }
            else
            {
                ttime.d1=0;
                ttime.d2=0;
                ttime.d3=0;
                ttime.d4=0;
            }
			
		}
		seg7DigitUpdate(&ttime);
		break;
	}
	schdCallback(callbackdispHandler, time+ 30);
}
int
main(void)
{
    lpInit();
    seg7Init();
    rangerInit();
    motionInit();
    buzzerInit();
    rasInit();


    seg7DigitUpdate(&seg7Display);
    uprintf("sysStart\n\r\t");
	schdCallback(stopWatchUpdate, 1000);
    schdCallback(chkPerson,1004);
    schdCallback(checkPushButton, 1006);
    schdCallback(callbackBuzzerPlay, 1008);
	schdCallback(callbackdispHandler, 1010);


    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
