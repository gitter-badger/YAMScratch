/* 	Names: Dustin Hoffman, Alex Malin, Isaiah Bell, Shenghao Lu
	Section: 1
	Date: 11-17-2014
	File name: Lab 5
	Program description: Accelerometer Lab
*/

#include <stdio.h>
#include <stdlib.h>
#include <c8051_SDCC.h>
#include <i2c.h>

// Ranging definitions
#define PW_MIN 2028
#define PW_MAX 3502
#define PW_NEUT 2765
#define ACCEL_ADDRESS 0x30

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
void accel_steering(void);
void calibrate_accel(void);
void get_four_readings(void);
void Run_Car(void);

__sbit __at 0xB6 SSdrive;
__sbit __at 0xB7 SSgain;

// Global variables
unsigned char Data[4];
unsigned int motor_pw = 0;
unsigned char setup_count = 25;

// Steering
unsigned int center; //center
unsigned int left; //left bound
unsigned int right; //right bound
unsigned int r = 0;
volatile char counts;

short int avg_gx;
short int avg_gy;
short int gx;
short int gy;
short int gx0;
short int gy0;

// Gains
unsigned char kx;
unsigned char ky;
unsigned char ks;

int keypad;

char calibrate_flag = 1;
char prev_h_counts;

volatile char h_counts = 0;
volatile int total_time;

void main(void)
{
	char counting_loops = 0;
	// initialize board
	Sys_Init();
	putchar(' ');
	All_Init();
	Accel_Init(); // Provided function in i2c.h for accelerometer use

	PCA0CP2 = 0xFFFF - PW_NEUT;

	setup_count = 25;
	while (setup_count);

	LCD_calibrate_steering();
	printf("\rSteering Calibrated\n");
	lcd_print("Steering Calibrated\n");

	LCD_prompts();

	setup_count = 20;
	prev_h_counts = h_counts; //start the checking flag
	while(1)
	{

		if(!SSgain && calibrate_flag)
		{
			calibrate_accel();
			calibrate_flag = 0;
			//printf("calibrated once\n\r");
		}
		else if (SSgain){
			get_four_readings(); //if the flag is down then we update the car
			//printf("running damn car");
			Run_Car();

		}
		counting_loops = counting_loops ? counting_loops-1 : 5;
		//printf("once through loop %d \n\r",counting_loops);

		if(!counting_loops)
		{
			Info_LCD_print();

			//X accel. Y accel. DPW, SPW, total_time
			printf("%d, %d, %d, %d, %d\n\r", gx, gy, motor_pw, r, total_time);
		}
	}
}

// Port_Init -- Initialize the used pinss
void All_Init(void)
{
	P0MDOUT |= 0x50; //set output pin for CEX0 in push-pull mode
	// Port 1 (xx xx xx xx xx xx xx OD)
	P1MDIN &=~0x01;		// Set P1.1 as an analog input
	P1MDOUT &= ~0x01;	// Set P1.1 as a input port bit
	P1 |= 0x01;			// Set P1.1 to a high impedance state

	P3MDOUT &= 0x3F; 	// set Port 3 input pins to open drain mode
	P3 |= 0xC0; 		// set Port 3 input pins to high impedance state

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
        h_counts = (h_counts) ? h_counts-1 : 19; //free wheeling timer that resets
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

	lcd_clear();
	lcd_print("Set Ks:\nEnter xx\n");
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	ks += ((keypad - 48)*10);
	while (read_keypad() != -1);
	//////////////////////////////
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	ks += ((keypad - 48));
	while (read_keypad() != -1);
	//////////////////////////////

	lcd_print("Ks = %d", ks);

	setup_count = 30;
	while(setup_count);


////////////////////////////////////////////////////////////////////////////////////
	lcd_clear();
	lcd_print("Set Kx:\nEnter xx\n");
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	kx += ((keypad - 48)*10);
	while (read_keypad() != -1);
	//////////////////////////////
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	kx += ((keypad - 48)*1);
	while (read_keypad() != -1);
	//////////////////////////////

	lcd_print("Kx= %d", kx);

	setup_count = 30;
	while(setup_count);
////////////////////////////////////////////////////////////
	lcd_clear();
	lcd_print("Set Ky:\nEnter xx\n");
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	ky += ((keypad - 48)*10);
	while (read_keypad() != -1);
	//////////////////////////////
	//////////////////////////////
	keypad = read_keypad();
	while (keypad == -1)
		keypad = read_keypad();

	ky += (keypad - 48);
	while (read_keypad() != -1);
	//////////////////////////////

	lcd_print("Ky = %d", ky);

	setup_count = 30;
	while(setup_count);
}

/////////////////////////////////////////////////////////////////////////////////////////
void Info_LCD_print(void)
{
	lcd_clear();
	lcd_print("Kx:%d  Ky:%d  Ks:%d\nGx:%d  Gy:%d\nSPW:%d DPW:%d\nBattery Level: %d%%\n",(int)(kx/100),(int)(ky/100),(int)(ks/10), gx, gy, r, motor_pw, ADC_read()*100/243);
	setup_count=20;
}

void LCD_calibrate_steering(void)
{
	char keypad;
	char *text;
	char i;
	//int b[4] = {2765,2380,2880,3400};	//all the values to the center, we can adjust
	int b[4] = {2765,2100,2680,3260};	//all the values to the center, we can adjust
	setup_count = 40;
	lcd_clear();
	lcd_print("Steering Calibration");
	lcd_print(" Set Points L,C,& R\n");
	lcd_print(" Increment 1-L 2-R\n");
	lcd_print("3-Store Value");
	while(setup_count)
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
			{
				keypad = read_keypad();

			}
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

void accel_steering(void)
{

	if(gx >= 0)
	{
		r = TURN_RIGHT(gx, center, right, ks/10);
	}
	else
	{
		r = TURN_LEFT(-gx, center, left, ks/10);
	}

	PCA0CP0 = 0xFFFF - r;
}

void calibrate_accel(void){
	char i = 0;
	avg_gx = 0; //zero the values before calibrating
	avg_gy = 0; 

	for (i =0; i < 4; i++){ //do four times to get total

		while(prev_h_counts == h_counts); //this waits
		prev_h_counts = h_counts; //update

		i2c_read_data(ACCEL_ADDRESS, 0x27, Data, 1); //get new reading
		while((Data[0] & 0x03) != 0x03) //wait for a new reading
		{
			i2c_read_data(ACCEL_ADDRESS, 0x27, Data, 1); //get new reading
		}

		if((Data[0] & 0x03) == 0x03)
		{
			i2c_read_data(ACCEL_ADDRESS, 0x28|0x80, Data, 4);
			avg_gx += ((Data[1] << 8) >> 4);
			avg_gy += ((Data[3] << 8) >> 4);
		}
	}
	gx0 = avg_gx / 4; //calculate the offset
	gy0 = avg_gy / 4;
	printf("Calibrating Gx, avg: %d offset: %d\n\r", avg_gx, gx0); //test print statements
	printf("Calibrating Gy, avg: %d offset: %d\n\r", avg_gy, gy0);
	avg_gx = 0; //zero the average values so other code can use
	avg_gy = 0;
}

void get_four_readings(void){
	char i = 0;
	prev_h_counts = h_counts;
	avg_gx = 0; //zero the values before calibrating
	avg_gy = 0; 

	for (i =0; i < 4; i++) { //do four times to get total
		while(prev_h_counts == h_counts); //this waits
		prev_h_counts = h_counts; //update
		while((Data[0] & 0x03) != 0x03) //wait for a new reading
		{
			i2c_read_data(ACCEL_ADDRESS, 0x27, Data, 1); //get new reading
		}
		if((Data[0] & 0x03) == 0x03)
		{
			i2c_read_data(ACCEL_ADDRESS, 0x28|0x80, Data, 4);
			avg_gx += ((Data[1] << 8) >> 4);
			avg_gy += ((Data[3] << 8) >> 4);
		}
	}
}

void Run_Car(void) //running the car
{
	if (SSgain) calibrate_flag = 1; //set the flag high once we are back to business
	gx = (avg_gx / 4)-gx0;
	gy = (avg_gy / 4)-gy0;
	avg_gx = 0;
	avg_gy = 0;
	accel_steering();
	if(!SSdrive)	// Drive if activated
	{
		motor_pw = PW_NEUT - (ky * gy)/10 + (kx * abs(gx))/10;		// If all y no x if all x no y, thus PW_MAX-PWNEUT/2047 or max accel that can be read on ramp=ky = kx=0.36
		if(motor_pw > 3502)
		{
			motor_pw = 3502;
		}
		else if(motor_pw < 2028)
		{
			motor_pw = 2028;
		}
		if(gy>0)
		{
			gy = 0.25*gy;
		}
		PCA0CP2 = 0xFFFF - motor_pw;
	}
	else		// Neutral if not activated
	{
		motor_pw = PW_NEUT;
		PCA0CP2 = 0xFFFF - motor_pw;
	}
	if(gy == 0 && gx == 0)
	{
		motor_pw = PW_NEUT;
		r = center;
	}
}
