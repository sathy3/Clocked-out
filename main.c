#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"

/*
 * System states
 */

// The running state of the stopwatch system
enum {
    Run, SetHr, SetMin, Clockout,
}  sysState = Run;
enum {
    alarmOn, alarmOff
} alarmState=alarmOff;

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {
    0, 0, 0, 0, 1
};

/*
 * Callback function for updating clock watch
 */

extern timeIncrement(seg7Display_t *);
uint8_t sec= 0;

void
stopWatchUpdate(uint32_t time)                          // The scheduled time
{
    // Update clock and display, only if the stopwatch is running
    if (sysState == Run && sec>=60 ) {
        timeIncrement(&seg7Display);
        sec=0;

    }
    if(sysState!=SetMin||sysState!=SetHr)
        {
            sec++;
        }
    else{
        sec=0;
    }
    seg7DigitUpdate(&seg7Display);

    // Call back after 1 seconds
    schdCallback(stopWatchUpdate, time + 1000);
}

int
main(void)
{
    lpInit();
    seg7Init();


    // Update the clock display
    seg7DigitUpdate(&seg7Display);
    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(stopWatchUpdate, 1000);
    //schdCallback(checkPushButton, 1005);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
