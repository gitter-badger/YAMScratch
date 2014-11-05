/*  Names: Isaiah Bell, Dustin Hoffman, Alex Malin
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

// Steering definitions
#define GAIN 0.2
#define TURN_LEFT(angle,source,lb) (((source) - (int)(angle)*GAIN) < (lb)) ? (lb) : ((source) - (int)(angle)*GAIN); // turn left
#define TURN_RIGHT(angle,source,rb) (((source) + (int)(angle)*GAIN) > (rb)) ? (rb) : ((source) + (int)(angle)*GAIN); //turn right

// Common prototypes
void Port_Init(void);
void PCA_Init (void);
void XBR0_Init();
void PCA_ISR ( void ) __interrupt 9;
void I2C_Init(void);
//void ADC_Init();

// Ranging prototypes
void ReadRanger(void);
void Drive_Motor(void);

__sbit __at 0xB6 SS;

// Steering prototypes
void Steering_Servo(void);
void calibrate_steering(void);
unsigned int read_compass(void);
void Compass_Steering(int, int);

// Global variables
// Ranging
//unsigned char r_count = 0;
unsigned char Data[2];
unsigned int range = 0;
unsigned int new_range = 0;
unsigned char addr = 0xE0;  // I2C address of the ultrasonic sensor
unsigned int MOTOR_PW = 0;
unsigned char setup_count = 25;

// Steering
unsigned int center; //center
unsigned int left; //left bound
unsigned int right; //right bound
unsigned int r = 0;
volatile char counts;
volatile char h_counts = 0;

void main(void)
{
    int heading = 0;
    int desired_heading = 900;
    // initialize board
    Sys_Init();
    putchar(' '); //the quotes in this line may not format correctly
    Port_Init();
    XBR0_Init();
    PCA_Init();
    I2C_Init();
//  ADC_Init();

    MOTOR_PW = PW_NEUT;
    PCA0CP2 = 0xFFFF - MOTOR_PW;

    while (setup_count);
    
    //print beginning message
    printf("Integrated Ranger and Steering\n");
    calibrate_steering();
    printf("\rSteering calibrated\n");
    
    while(1)
    {
         //Steering_Servo();
        if(h_counts%2)
        {
            heading = read_compass();
        }

        if(!h_counts)
        {
            printf("\rThe Heading is: %u\n", heading); //print the heading
            Compass_Steering(heading,desired_heading);
        }

        if(h_counts%4 == 1)
        {
            ReadRanger();
            Drive_Motor();
        }
    }
}

// Port_Init -- Initialize the used pins

void Port_Init(void)
{
    P1MDOUT |= 0x01; //set output pin for CEX0 in push-pull mode

    P3MDOUT &= 0xBF;    // set Port 3 input pins to open drain mode
    P3 |= 0x40;         // set Port 3 input pins to high impedance state

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

    if (CF)
    {
        CF = 0; /* Clear overflow flag */
        PCA0 = 28672; /* or PCA0L = 0x34; low byte of start count */
        /*
        r_count++;

        if(r_count >= 4)
        {
            new_range = 1; // 4 overflows is about 80 ms
            r_count = 0;
        }*/

        if (counts){counts--;} //use a counter
        h_counts = (h_counts) ? h_counts-1 : 11; //free wheeling timer that resets
    }

    PCA0CN &= 0xC0; /* Handle other PCA interrupt sources */
}

// I2C_Init -- Initializes I2C

void I2C_Init(void)
{

    SMB0CR = 0x93;
    ENSMB = 1;
}

// ReadRanger -- Read the range

void ReadRanger(void)
{
    new_range = 0;  // Clear flag
    i2c_read_data(addr, 2, Data, 2); // read two bytes, starting at reg 2
    range = (((unsigned int)Data[0] << 8) | Data[1]);   
    Data[0] = 0x51; // Write 0x51 to reg 0 of the ranger
    i2c_write_data(addr, 0, Data, 1) ; // write one byte of data to reg 0 at addr to ping
    printf("\rRange: %u \n", range);
}

// Drive_Motor -- Drive the motor

void Drive_Motor()
{
    printf("\rPW: %u\n", MOTOR_PW);

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

    printf("\rPW: %u \n", MOTOR_PW);

    if (SS)
        MOTOR_PW = PW_NEUT;

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

    printf("\rr: %u\n", r);
    PCA0CP0 = 0xFFFF - r; //set the value of the pulse width we want to use
}

void calibrate_steering(void)
{
    char input;
    char *text;
    char i;
    //all the values to the center, we can adjust
    int b[4] = {2765,0,0,0};
    //try to find the left most set point first, then the center, then finally the right
    PCA0CP0 = 0xFFFF - b[0]; // put it near the left set point
    for (i=1; i<4;i++)
    {
        text = i == 1 ? "left" : (i == 2 ? "center" : "right"); //figure out where we are
        b[i] = b[i-1]; //start where we left off
        while (1){
            printf("\rCalibrating %s set point. Value is %d\n", text,b[i]); //tell them every time
            input = getchar();
            if (input == 'l')
            {
                b[i] = b[i] - 10; //turning left is decrementing
                PCA0CP0 = 0xFFFF - b[i];
            }
            else if(input == 'r')
            {
                b[i] = b[i] + 10; //turn right is incrementing
                PCA0CP0 = 0xFFFF - b[i];
            }
            else if (input == 'q')
            {
                break;
            }
            PCA0CP0 = 0xFFFF - b[i]; // update the position
        }
    }
    //now that we have finished up, assign the results to the globals
    left = b[1];
    center = b[2];
    right = b[3];
    r = right;
    //go back to center
    while(r>center){
        r -= 10;
        counts = 2;
        while(counts){}
        PCA0CP0 = 0xFFFF - r;
    }
}

unsigned int read_compass(void)
{
    unsigned char ad = 0xC0; // address of  the compass
    unsigned char d[2];
    //printf("deep");
    i2c_read_data(ad,2,d,2); //start at register 2 and read two bytes
    //printf("back");
    return (unsigned int) d[0] << 8 | d[1]; //combine the two bytes into one value
}

void Compass_Steering (int actual, int desired)
{
    signed int angle =  actual - desired;
    printf("this is the angle we need to turn %d\n", angle);
    if (angle > 0)
    {
        if (angle > 1800)
        {
            r = TURN_RIGHT(angle-1800,r,right); 
        }
        else
        {
            r = TURN_LEFT(angle,r,left);
        }

    }
    else
    {
        if (angle < -1800)
        {
            angle = -(angle + 1800);
            r = TURN_LEFT(angle,center,left);
        }
        else
        {
            r = TURN_RIGHT(-angle,center,right);
        }
    }
    printf("\rr: %u\n", r);
    PCA0CP0 = 0xFFFF - r; //set the value of the pulse width we want to use
}