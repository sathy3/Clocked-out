#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "motion.h"
#include "ranger.h"
#include "pwmbuzzer.h"
/*
 * System states
 */

// The running state of the stopwatch system
enum{
    Run, Set, Buffer
	}  sysState = Run;
enum{
    On, Off, disabled
} alarmState=Off;
enum{
    showTime, SetTimeMin, SetTimeHr,
} display_s = showTime;
enum{
    none, setTime, alarmSet
} set_mode=none;
enum{
    Off, detected, not_detected, not_present
} personState= not_detected;

enum{
	setTime, 

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {
    0, 0, 0, 0, 1
};

extern timeIncrement(seg7Display_t *);
volatile uint32_t sec= 0;
volatile uint32_t seated=0;
static uint32_t limit=120000;

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
        {   personState=not_present
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
        if(display_s==showTimeout)
        {   int temp=0;
            seg7Display_t ttime={0,0,0,0,1};
            temp= (limit-seated)/60000;
			if (temp>=0)
			{
            ttime.d1=temp%10;
            ttime.d2=(temp/10)%10;
            ttime.d3=(temp/100)%10;
            ttime.d4=(temp/1000)%10;
			}
            seg7DigitUpdate(&ttime);

        }
		if(seated>limit&&personState==seated&&alarmState!=disabled)
		{
			alarmState=On;
			
		}
		else if(alarmState==disabled)
		{
			;
		}
		else{
			alarmState=Off;
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
    if(sysState!=Set||set_mode!=setTime)
        {
            sec++;
        }
    else{
        sec=0;
    }
    if (display_s == showTime)
        {
        seg7DigitUpdate(&seg7Display);
        }
    if (0)
    {


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
		if(sysState==Buffer)
		{	
			sysState=Run;
		}
		else{
		sysState=Buffer;
		}
		delay = 250;
        break;

    case 2:
		
        delay = 250;
        break;
    }

    schdCallback(checkPushButton, time + delay);
}

int
main(void)
{
    lpInit();
    seg7Init();
    rangerInit();
    motionInit();



    seg7DigitUpdate(&seg7Display);
    uprintf("sysStart\n\r\t");
	schdCallback(stopWatchUpdate, 1000);
    schdCallback(chkPerson,1004);
    


    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
