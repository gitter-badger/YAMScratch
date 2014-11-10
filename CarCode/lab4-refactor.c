/* 	Names: Dustin Hoffman, Alex Malin, Isaiah Bell,Shenghao Lu
	Section: 1
	Date: 10-30-2014
	File name: Lab 4
	Program description: Heading/Ranger Integration
*/

#include <stdio.h>
#include <stdlib.h>
#include <c8051_SDCC.h>
#include "i2c.h"

// Ranging definitions
#define PW_MIN 2028
#define PW_MAX 3502
#define PW_NEUT 2765

// Steering definitions
#define TURN_LEFT(angle,source,lb,gain) (((source) - (int)(angle)*gain) < (lb)) ? (lb) : ((source) - (int)(angle)*gain); // turn left
#define TURN_RIGHT(angle,source,rb,gain) (((source) + (int)(angle)*gain) > (rb)) ? (rb) : ((source) + (int)(angle)*gain); //turn right

// Common prototypes
void Port_Init(void);
void PCA_Init (void);
void XBR0_Init();
void PCA_ISR ( void ) __interrupt 9;
void I2C_Init(void);
void ADC_Init(void);
unsigned char ADC(void);
void LCD_prompts(void);
void LCD_print(void);
void LCD_calibrate_steering(void);


// Ranging prototypes
void ReadRanger(void);
void Drive_Motor(void);

__sbit __at 0xB6 SS;

// Steering prototypes
void Steering_Servo(void);
unsigned int read_compass(void);
void Compass_Steering(int, int);

// Global variables
// Ranging
//unsigned char r_count = 0;
unsigned char Data[2];
unsigned int range = 0;
unsigned char addr = 0xE0;	// I2C address of the ultrasonic sensor
unsigned int MOTOR_PW = 0;
unsigned char setup_count = 25;


// Steering
unsigned int center; //center
unsigned int left; //left bound
unsigned int right; //right bound
unsigned char ad = 0xC0; // address of  the compass
unsigned int r = 0;
volatile char counts;
volatile char h_counts = 0;
float glob_steer_gain = 0.2;
int desired_heading = 9000;
unsigned int heading = 0;
volatile char range_flag= 0;

volatile int total_time;

void main(void)
{
	
	// initialize board
	Sys_Init();
	putchar(' ');
	Port_Init();
	XBR0_Init();
	PCA_Init();
	I2C_Init();
	ADC_Init();

	MOTOR_PW = PW_NEUT;
	PCA0CP2 = 0xFFFF - MOTOR_PW;
	setup_count = 25;
	while (setup_count);
	
	LCD_calibrate_steering();
	printf("\rSteering Calibrated\n");
	lcd_print("Steering Calibrated\n");
	
	LCD_prompts();

	setup_count = 20;
	while(1)
	{
		if(h_counts%2)
		{
			heading = read_compass();
			Compass_Steering(heading,desired_heading);
			if(range < 55)
			{
				r = TURN_RIGHT(55-range,center,right,30);
				PCA0CP0 = 0xFFFF - r;
			}
			range_flag = 1;
		}

		if(!h_counts && range_flag)
		{
			//printf("\rThe Heading is: %u\n", heading); //print the heading
			ReadRanger();
			Drive_Motor();
			printf("%d\t%d\t%d\r\n",(heading-desired_heading),r,total_time);
			range_flag = 0;
		}

		if(!setup_count)
		{
			LCD_print();
			//printf("Battery Level: %d%%\tCurrent Range: %d\tCurrent Heading: %d\r\n",(ADC()  * 100)/243, range, heading);
			
		}

	}
}

// Port_Init -- Initialize the used pins

void Port_Init(void)
{
	P0MDOUT |= 0x50; //set output pin for CEX0 in push-pull mode

	// Port 1 (xx xx xx xx xx xx xx OD)
	P1MDIN &=~0x01;		// Set P1.1 as an analog input
	P1MDOUT &= ~0x01;	// Set P1.1 as a input port bit
	P1 |= 0x01;			// Set P1.1 to a high impedance state

	P3MDOUT &= 0xBF; 	// set Port 3 input pins to open drain mode
	P3 |= 0x40; 		// set Port 3 input pins to high impedance state

}

// XBR0_Init -- Set up the crossbar

void XBR0_Init(void)
{
	XBR0 = 0x25; //configure crossbar with UART, SPI, SMBus, and CEX channels as in worksheet
}

// PCA_Init -- Set up Programmable Counter Array

void PCA_Init(void)
{
	PCA0MD = 0x81; /* Enable CF interrupt */
	PCA0CPM2 = 0xC2; /* CCM2 in 16-bit compare mode */
	PCA0CPM0 = 0xC2; /* CCM0 in 16-bit compare mode */
	EIE1 |= 0x08; /* Enable PCA interrupt */
	PCA0CN = 0x40; /* Enable PCA counter */
	EA = 1; /* Enable global interrupts */
}

// PCA_ISR -- Interrupt Service Routine for Programmable Counter Array Overflow Interrupt

void PCA_ISR ( void ) __interrupt 9
{
	setup_count--;
	total_time++; //global timer for output	

	if (CF)
	{
		CF = 0; /* Clear overflow flag */
		PCA0 = 28672; /* or PCA0L = 0x34; low byte of start count */

        if (counts){counts--;} //use a counter
        h_counts = (h_counts) ? h_counts-1 : 4; //free wheeling timer that resets
	}

	PCA0CN &= 0xC0; /* Handle other PCA interrupt sources */
}

// I2C_Init -- Initializes I2C

void I2C_Init(void)
{
	SMB0CR = 0x93;
	ENSMB = 1;
}

void ADC_Init(void)
{
	REF0CN  = 0x03;		// Configure ADC1 to use VREF
	ADC1CF |= 0x01;		// Set Gain to 1
	ADC1CF &= ~0x02;
	ADC1CN |= 0x80;		// Enables ADC1
}

unsigned char ADC()
{
	AMX1SL  = 0x01;			// Set the Port pin number Port 1.n, where n is a parameter passed to the function
	ADC1CN &= ~0x20;		// Clear the flag from previous conversion
	ADC1CN |= 0x10;			// Start A/D conversion
	while ((ADC1CN & 0x20) == 0x00);		// Wait for Conversion to be complete
	return ADC1;
}

// ReadRanger -- Read the range

void ReadRanger(void)
{
	i2c_read_data(addr, 2, Data, 2); // read two bytes, starting at reg 2
	range = (((unsigned int)Data[0] << 8) | Data[1]);	
	Data[0] = 0x51;	// Write 0x51 to reg 0 of the ranger
	i2c_write_data(addr, 0, Data, 1) ; // write one byte of data to reg 0 at addr to ping
	//printf("\rRange: %u \n", range);
}

// Drive_Motor -- Drive the motor

void Drive_Motor()
{
	//printf("\rPW: %u\n", MOTOR_PW);
	/*
	if (range <= 10)
		MOTOR_PW = PW_MAX;	
	else if ((range >= 40) && (range <= 50))
		MOTOR_PW = PW_NEUT;	
	else if (range >= 90)
		MOTOR_PW = PW_MIN;
	else if ((range > 10) && (range < 40)) // Map from max to neut
		MOTOR_PW = PW_MAX - (PW_MAX - PW_NEUT) * (range - 10) / 30;
	else
		MOTOR_PW = PW_NEUT - (PW_NEUT - PW_MIN) * (range - 50) / 40; 
	*/
	MOTOR_PW = PW_MAX;
	//printf("\rPW: %u \n", MOTOR_PW);

	if (SS) 
	{
		MOTOR_PW = PW_NEUT;
		//printf("turning it off\n");
	}

	PCA0CP2 = 0xFFFF - MOTOR_PW;
}

void Steering_Servo()
{
	char input;
	//center the steering
	//wait for a key to be pressed
	input = getchar();

	if(input == 'r') // single character input to increase the pulsewidth
	{
		r = ((r +10) > right) ? right : (r + 10); // check if less than pulsewidth minimum
		// set SERVO_r to a minimum value
	}
	else if(input == 'l') // single character input to decrease the pulsewidth
	{
		r = ((r - 10) < left) ? left : (r - 10); // check if pulsewidth maximum exceeded
		// set r to a maximum value
	}

	PCA0CP0 = 0xFFFF - r; //set the value of the pulse width we want to use
}


unsigned int read_compass(void)
{
    unsigned char d[2];
    i2c_read_data(ad,2,d,2); //start at register 2 and read two bytes
    return (unsigned int) d[0] << 8 | d[1]; //combine the two bytes into one value
}

void Compass_Steering (int actual, int desired)
{
    signed int angle =  actual - desired;
    //printf("this is the angle we need to turn %d\n", angle);
    if (angle > 0)
    {
        if (angle > 1800)
        {
            r = TURN_RIGHT(3600-angle,center,right,glob_steer_gain);
            
        }
        else
        {
            r = TURN_LEFT(angle,center,left,glob_steer_gain);
        }

    }
    else
    {
        if (angle < -1800)
        {
            angle = (angle + 3600);
            r = TURN_LEFT(angle,center,left,glob_steer_gain);
        }
        else
        {
            r = TURN_RIGHT(-angle,center,right,glob_steer_gain);
        }
    }
    //printf("\rr: %u\n", r);
    PCA0CP0 = 0xFFFF - r; //set the value of the pulse width we want to use
}

void LCD_prompts(void)
{
	int keypad;
	int buffer;

	lcd_clear();
	lcd_print("Set Heading\n1 Select from list\n2 Enter value");

	//////////////////////////////
	keypad = read_keypad();
	while ((keypad == -1) || !(keypad == 49 || keypad == 50))
		keypad = read_keypad();

	while (read_keypad() != -1);
	//////////////////////////////

	lcd_clear();
	if (keypad == 49)
	{
		lcd_print("Choose from list:\n[1]0 deg   [2]90 deg[3]180 deg\n[4]270 deg");

		//////////////////////////////
		keypad = read_keypad();
		while (keypad == -1 || !(keypad >= 49 && keypad <= 52))
			keypad = read_keypad();

		while (read_keypad() != -1);
		if (keypad == 49)
		//////////////////////////////

			desired_heading = 0;
		else if (keypad == 50)
			desired_heading = 900;
		else if (keypad == 51)
			desired_heading = 1800;
		else 
			desired_heading = 2700;
		
		lcd_clear();
		lcd_print("You chose: %d",desired_heading/10);
	}
	else
	{
		while (desired_heading > 3600)
		{
			lcd_clear();
			lcd_print("Enter 3 digit value\nbetween 000 & 360:");
			//////////////////////////////
			keypad = read_keypad();
			while (keypad == -1)
				keypad = read_keypad();
		
			buffer = (keypad - 48) * 100;
			while (read_keypad() != -1);
			//////////////////////////////
			//////////////////////////////
			keypad = read_keypad();
			while (keypad == -1)
				keypad = read_keypad();

			buffer += (keypad - 48) * 10;
			while (read_keypad() != -1);
			//////////////////////////////
			//////////////////////////////
			keypad = read_keypad();
			while (keypad == -1)
				keypad = read_keypad();

			buffer += keypad - 48;
			while (read_keypad() != -1);
			//////////////////////////////

			lcd_print("\n%d", buffer);
			desired_heading = buffer * 10;
		}
	}
	
	setup_count = 50;
	while(setup_count);

	lcd_clear();
	lcd_print("Set gain:\nEnter x.xx\n");

	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	glob_steer_gain = ((float)keypad - 48);
	while (read_keypad() != -1);
	//////////////////////////////
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	glob_steer_gain += (((float)keypad - 48)/10);
	while (read_keypad() != -1);
	//////////////////////////////
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	glob_steer_gain += (((float)keypad - 48)/100);
	while (read_keypad() != -1);
	//////////////////////////////
	
	lcd_print("Gain * 100 = %d", (int)(glob_steer_gain*100));

	setup_count = 50;
	while(setup_count);
}	

//////////////////
void LCD_print(void)
{
	lcd_clear();
	lcd_print("Battery Level: %d%%\nCurrent Range: %u\nCurrent Heading:%d",ADC()*100/243, range, heading);
	setup_count=20;
}

/////////////////
void LCD_calibrate_steering(void)
{
	int keypad;
	char *text;
	char i;
	int b[4] = {2765,0,0,0};	//all the values to the center, we can adjust

	/////////
	setup_count = 20;

	lcd_clear();
	lcd_print("Steering Calibration");
	lcd_print(" Set Points L,C,& R\n");
	lcd_print(" Increment 1-L 2-R\n");
	lcd_print("3-Store Value");
	
	while(setup_count)
	///////////
	
	//try to find the left most set point first, then the center, then finally the right
	PCA0CP0 = 0xFFFF - b[0]; // put it near the left set point
	for (i=1; i<4;i++)
	{
		text = i == 1 ? "left" : (i == 2 ? "center" : "right");
        	b[i] = b[i-1]; //start where we left off

		while(1)
		{
			lcd_clear();
			lcd_print("Calibrating %s \nValue: %d\n", text, b[i]);
			
			keypad = read_keypad();
			while ((keypad == -1) || !(keypad == 49 || keypad == 50 || keypad == 51))
				keypad = read_keypad();
			while (read_keypad() != -1);
			
			
			if (keypad == 49)	//turning left is decrementing
			{
				b[i] = b[i] - 20; //turning left is decrementing
				PCA0CP0 = 0xFFFF - b[i];
			}
            		
			else if(keypad == 50)
			{
				b[i] = b[i] + 20; //turn right is incrementing
				PCA0CP0 = 0xFFFF - b[i];
			}
            	
			else if (keypad == 51)
				break;
            
			PCA0CP0 = 0xFFFF - b[i]; // update the position
        }


	}
	 //now that we have finished up, assign the results to the globals
    left = b[1];
    center = b[2];
    right = b[3];
    r = right;
    //go back to center
    while(r>center)
	{
        r -= 10;
        counts = 2;
        while(counts){}
        PCA0CP0 = 0xFFFF - r;
    }
}