/*******************************************************************************
* EECE344 Lab 4: Basic alarm with a memory check function. The user can
* enable and disable the alarm by pressing the key 'A' and 'D'. This lab
* also has outputs a sin wave for the alarm so the user can put a 1k
* speaker onto the board to listen to the alarm.
*
*Andy Nguyen
*******************************************************************************/


#include "MCUType.h"                                                                         //Include project header file
#include "BasicIO.h"
#include "K65TWR_ClkCfg.h"
#include "Key.h"
#include "LCD.h"
#include "SysTickDelay.h"
#include "K65TWR_GPIO.h"
#include "MemoryTools.h"
#include "AlarmWave.h"

#define HIGH 0x001FFFFF
#define LOW 0x00000000

#define SLICE_PER 10             /* 10ms time slice period                 */


#define COL 1
#define ROW 1

#define ROW2 2

#define SECOND 1000U				// definging one second //



static void lab4ControlTask(void);

const INT8U clear = 1;				// row 1
const INT8U num_nib = 4;
static INT8U firstTime = 1;


// statics const for LCD display messages //
INT8C CS_M[] = "CheckSum: 0x";						//check sum message
INT8C A_OFF[] = "Alarm: OFF";
INT8C A_ON[] = "Alarm: ON";

typedef enum {ALARM_OFF,ALARM_ON} UISTATE_T;                                                    //create states for the state machine
UISTATE_T AlarmState = ALARM_OFF;

void main (void){

	K65TWR_BootClock();				//k65 Clock init
    GpioDBugBitsInit();
    SysTickDlyInit();
    KeyInit();
    LcdDispInit();
    AlarmWaveInit();

    BIOOpen(BIO_BIT_RATE_115200);   /* Initialize Serial Port                 */

    // check sum pointers //
     INT8U *high_addr = (INT8U *)0x001FFFFF;		//high
     INT8U *low_addr = (INT8U *)0x00000000;			//low
     INT16U check_sum = 0;



    // check sum that displays on terminal to see if it matches with LCD //
 	BIOPutStrg("*********************************************************");

 	BIOPutStrg("\n\r");
    BIOPutStrg("Hello welcome to lab4: \n\r");

    check_sum = MemChkSum(low_addr, high_addr);

 	BIOPutStrg("CS: ");
 	BIOOutHexWord(LOW);
 	BIOPutStrg(" - ");
 	BIOOutHexWord(HIGH);
 	BIOPutStrg(" = ");

 	BIOOutHexWord(check_sum);

 	BIOPutStrg("\n\r");
 	BIOPutStrg("*********************************************************");
 	BIOPutStrg("\n\r");

 	// checksum on LCD //

 	check_sum = MemChkSum(low_addr, high_addr); //check sum

 	LcdDispClear();								// clear disp
 	LcdCursorMove(ROW2,COL);					// shifting row and col
 	LcdDispString(CS_M);						// disp pre-written message

    LcdDispHexWord((INT32U)check_sum, num_nib);



	/*create time scheduler for the kernel*/
    while(1){                               /* Endless Time Slice cyclic scheduler  */

        SysTickWaitEvent(SLICE_PER);        //DebugBit 0
        lab4ControlTask();					//DebugBit 1
        KeyTask();							//DebugBit 2

    }


}

/******************************************************************************
*lab4ControlTask(void) - This controls the sin wave pulse and users key

******************************************************************************/
static void lab4ControlTask(void){


	static INT8C cur_key;
	static INT8U Wait = 0;
	static INT8U AlarmFlag = 0;


	    cur_key = KeyGet();

	    DB1_TURN_ON();													// setting Debug bits

			switch (AlarmState){

			case (ALARM_OFF):


				if(firstTime == 1){

					firstTime = 0;				//setting the check to 0 to say its checked

					LcdCursorMove(ROW,COL);
					LcdDispString(A_OFF);

					AlarmWaveSetMode(0);

					}


				if(cur_key == DC1){

					LcdDispLineClear(1);
					AlarmState = ALARM_ON;
					firstTime = 1;						//reseting check

					}


			break;


			case (ALARM_ON):


				if(firstTime == 1){

						firstTime = 0;

						LcdCursorMove(ROW,COL);							// shifting row and col to display on bottom row
						LcdDispString(A_ON);							// display message string


				}
					if(Wait == 49){
						Wait = 0;
						if(AlarmFlag == 1){				// if flag is enable , enable the alarm
						AlarmFlag = 0;					// reset flag
						AlarmWaveSetMode(1);
						}

						else {
							AlarmFlag = 1;
							AlarmWaveSetMode(0);
						}

					}

					else{
						Wait++;


					}


					if(cur_key == DC4){

						LcdDispLineClear(1);							// clearing line

						AlarmState = ALARM_OFF;
						firstTime = 1;
					}



			break;

				default:
					LcdDispLineClear(1);
					AlarmState = ALARM_OFF;

				break;


		}
		    DB1_TURN_OFF();

    }
