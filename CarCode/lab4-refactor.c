/* 	Names: Dustin Hoffman, Alex Malin, Isaiah Bell, Shenghao Lu
	Section: 1
	Date: 10-30-2014
	File name: Lab 4
	Program description: Heading/Ranger Integration
*/

#include <stdio.h>
#include <stdlib.h>
#include <c8051_SDCC.h>
#include <i2c.h>

// Ranging definitions
#define PW_MIN 2028
#define PW_MAX 3502
#define PW_NEUT 2765
#define COMPASS_ADDRESS 0xC0
#define RANGER_ADRESS 0xE0

// Steering definitions
#define TURN_LEFT(angle,source,lb,gain) (((source) - (int)(angle)*gain) < (lb)) ? (lb) : ((source) - (int)(angle)*gain); // turn left
#define TURN_RIGHT(angle,source,rb,gain) (((source) + (int)(angle)*gain) > (rb)) ? (rb) : ((source) + (int)(angle)*gain); //turn right

// Common prototypes
void All_Init(void);
void PCA_ISR ( void ) __interrupt 9;

unsigned char ADC_read(void);
void LCD_prompts(void);
void Info_LCD_print(void);
void LCD_calibrate_steering(void);

__sbit __at 0xB6 SS;

// Steering prototypes
unsigned int read_compass(void);

// Global variables
unsigned char Data[2];
unsigned int range = 0;
unsigned int motor_pw = 0;
unsigned char setup_count = 25;


// Steering
unsigned int center; //center
unsigned int left; //left bound
unsigned int right; //right bound
unsigned int r = 0;

float glob_steer_gain = 0.2;
int desired_heading = 9000;
unsigned int heading = 0;

volatile char range_flag= 0;
volatile char counts;
volatile char h_counts = 0;
volatile int total_time;

void main(void)
{
	int angle;
	// initialize board
	Sys_Init();
	putchar(' ');
	All_Init();

	PCA0CP2 = 0xFFFF - PW_NEUT;

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
			//read the compass
    		i2c_read_data(COMPASS_ADDRESS,2,Data,2); //start at register 2 and read two bytes
    		heading = (unsigned int) Data[0] << 8 | Data[1]; //combine the two bytes into one value

			{ //this is the compass steering
			    angle =  heading - desired_heading;
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
			    PCA0CP0 = 0xFFFF - r; //set the value of the pulse width we want to use
			}

			if(range < 55)
			{
				r = TURN_RIGHT(55-range,center,right,60);
				PCA0CP0 = 0xFFFF - r;
			}
			range_flag = 1;
		}

		if(!h_counts && range_flag)
		{
			{ 
				//this is the read ranger function unrolled
				i2c_read_data(RANGER_ADRESS, 2, Data, 2); // read two bytes, starting at reg 2
				range = (((unsigned int)Data[0] << 8) | Data[1]);	
				i2c_start();               	//initiate I2C transfer
			    i2c_write(RANGER_ADRESS & ~0x01);   	//write the desired address to the bus
			    i2c_write(0); 				//write the start register
			    i2c_write_and_stop(0x51); 	//Stop transfer
			}
			PCA0CP2 = 0xFFFF - ((SS) ? PW_NEUT: PW_MAX);
			printf("%d\t%d\t%d\t%d\r\n",(heading-desired_heading),r,total_time,range);
			range_flag = 0;
		}

		if(!setup_count)
		{
			Info_LCD_print();	
		}

	}
}

// Port_Init -- Initialize the used pins

void All_Init(void)
{
	P0MDOUT |= 0x50; //set output pin for CEX0 in push-pull mode
	// Port 1 (xx xx xx xx xx xx xx OD)
	P1MDIN &=~0x01;		// Set P1.1 as an analog input
	P1MDOUT &= ~0x01;	// Set P1.1 as a input port bit
	P1 |= 0x01;			// Set P1.1 to a high impedance state

	P3MDOUT &= 0xBF; 	// set Port 3 input pins to open drain mode
	P3 |= 0x40; 		// set Port 3 input pins to high impedance state

	// XBR0_Init -- Set up the crossbar
	XBR0 = 0x25; 		//configure crossbar with UART, SPI, SMBus, and CEX channels as in worksheet
	
	// PCA_Init -- Set up Programmable Counter Array
	PCA0MD = 0x81; 		/* Enable CF interrupt */
	PCA0CPM2 = 0xC2; 	/* CCM2 in 16-bit compare mode */
	PCA0CPM0 = 0xC2; 	/* CCM0 in 16-bit compare mode */
	EIE1 |= 0x08; 		/* Enable PCA interrupt */
	PCA0CN = 0x40; 		/* Enable PCA counter */
	EA = 1; 			/* Enable global interrupts */

	//ADC Init
	REF0CN  = 0x03;		// Configure ADC1 to use VREF
	ADC1CF |= 0x01;		// Set Gain to 1
	ADC1CF &= ~0x02;	//
	ADC1CN |= 0x80;		// Enables ADC1

	//I2C Init
	SMB0CR = 0x93;		//Set the clock speed to 93kHZ
	ENSMB = 1;			//enable the I2C bus
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


unsigned char ADC_read()
{
	AMX1SL  = 0x01;			// Set the Port pin number Port 1.n, where n is a parameter passed to the function
	ADC1CN &= ~0x20;		// Clear the flag from previous conversion
	ADC1CN |= 0x10;			// Start A/D conversion
	while ((ADC1CN & 0x20) == 0x00);		// Wait for Conversion to be complete
	return ADC1;
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
		if ((keypad == 49))
			desired_heading = 0;
		else if ((keypad == 50))
			desired_heading = 900;
		else if ((keypad == 51))
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
void Info_LCD_print(void)
{
	lcd_clear();
	lcd_print("Battery Level: %d%%\nCurrent Range: %u\nCurrent Heading:%d",ADC_read()*100/243, range, heading);
	setup_count=20;
}

/////////////////
void LCD_calibrate_steering(void)
{
	char keypad;
	char *text;
	char i;
	int b[4] = {2765,2020,2585,3120};	//all the values to the center, we can adjust

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
		text = (i-1) ? ((i-2) ? "right" : "center"): "left" ;

		while(1)
		{
			PCA0CP0 = 0xFFFF - b[i]; // update the position
			lcd_clear();
			lcd_print("Calibrating %s \nValue: %d\n", text, b[i]);
			
			keypad = read_keypad();
			while (keypad == -1 || !(keypad == 49 || keypad  == 50 || keypad == 51))
				keypad = read_keypad();

			while (read_keypad() != -1);
			
			
			if ((keypad == 49))	//turning left is decrementing
			{
				b[i] = b[i] - 20; //turning left is decrementing
				PCA0CP0 = 0xFFFF - b[i];
			}
            		
			else if((keypad == 50))
			{
				b[i] = b[i] + 20; //turn right is incrementing
				PCA0CP0 = 0xFFFF - b[i];
			}
            	
			else if ((keypad == 51))
				break;          
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
