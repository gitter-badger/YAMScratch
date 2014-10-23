#include <stdio.h>
#include <stdlib.h>
#include <c8051_SDCC.h>
#include <i2c.h>
#define GAIN 0.3
#define TURN_LEFT(angle,source,lb)  (((source) - (int)(angle)*GAIN) < (lb)) ? (lb) : ((source) - (int)(angle)*GAIN); // turn left
#define TURN_RIGHT(angle,source,rb)  (((source) + (int)(angle)*GAIN) > (rb)) ? (rb) : ((source) + (int)(angle)*GAIN); //turn right
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Port_Init(void);
void PCA_Init (void);
void XBR0_Init();
void Steering_Servo(void);
void PCA_ISR ( void ) __interrupt 9;
void calibrate_steering(void);
unsigned int read_compass(void);
void Compass_Steering(int, int);
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
unsigned int r2;
unsigned int r1;
unsigned int r6;
unsigned int r = 0;
volatile char counts;
volatile char h_counts = 0;
//-----------------------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------------------
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
    SMB0CR = 0x93; // set the frequency of the i2c bus to 95.41 kHz
    ENSMB = 1; //enable the SMBus
    //print beginning message
    printf("Embedded Control Steering Calibration\n");
    calibrate_steering();
    printf("\rCalibrated\n");
    while(1){
        
        //Steering_Servo();
        if(h_counts%2){
            heading = read_compass();
            
        }   
        if(!h_counts){
                printf("\rThe Heading is: %u\n", heading); //print the heading
				Compass_Steering(heading,desired_heading);
				
            }
        
        //else{printf("\rthis is h_coutn %d\n",h_counts);}
    }
}


//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Set up ports for input and output
//
void Port_Init()
{
    P1MDOUT |= 0x01;  //set output pin for CEX0 in push-pull mode
    //P0MDOUT &= ~0xC0; //set the output pin for SMB in open drain mode
}

//-----------------------------------------------------------------------------
// XBR0_Init
//-----------------------------------------------------------------------------
//
// Set up the crossbar
//
void XBR0_Init()
{
    XBR0 = 0x27;  //configure crossbar as directed in the laboratory
    //enables CEX0,CEX1,CEX2,CEX3

}

//-----------------------------------------------------------------------------
// PCA_Init
//-----------------------------------------------------------------------------
//
// Set up Programmable Counter Array
//
void PCA_Init(void)
{
    PCA0MD = 0x81; //use SYSCLK/12, enable and suspend when in idle mode 
    PCA0CPM0 = 0xC2; //enable 16 bit mode, enable comparator, enable PWM (ouput to CEX0 pin)
    EIE1 |= 0x08; //enable PCA iterrupt request
    PCA0CN = 0x40; //enable the PCA counter/timer
    EA = 1; //enable global interrupts
}

//-----------------------------------------------------------------------------
// PCA_ISR
//-----------------------------------------------------------------------------
//
// Interrupt Service Routine for Programmable Counter Array Overflow Interrupt
//
void PCA_ISR ( void ) __interrupt 9
{
    if(CF){
        CF = 0;
        PCA0 = 28672;//magic number to advance the counter
        if (counts){counts--;} //use a counter
        h_counts = (h_counts) ? h_counts-1 : 9; //free wheeling timer that resets
    }
    PCA0CN &= 0xC0; //clear all other interrupts
}

void Steering_Servo()
{
    char input;
    //center the steering
    //wait for a key to be pressed
    input = getchar();
    if(input == 'r')  // single character input to increase the pulsewidth
    {
        r = ((r +10) > r6) ? r6 : (r + 10); // check if less than pulsewidth minimum
   // set SERVO_r to a minimum value
    }
    else if(input == 'l')  // single character input to decrease the pulsewidth
    {
        r = ((r - 10) < r1) ? r1 : (r - 10); // check if pulsewidth maximum exceeded
        // set r to a maximum value
    }
    printf("\rr: %u\n", r);
    PCA0CP0 = 0xFFFF - r; //set the value of the pulse width we want to use

}
//-----------------------------------------------------------------------------
// calibrate_steering
//-----------------------------------------------------------------------------
//
// Sets the minimum and maximum value for the steering, as well as center set point
//

void calibrate_steering(void){
    char input;
    char *text;
    char i;
    //all the values to the center, we can adjust
    int b[4] = {2765,0,0,0};
    //try to find the left most set point first, then the center, then finally the right
    PCA0CP0 = 0xFFFF - b[0]; // put it near the left set point
    for (i=1; i<4;i++){
        text = i == 1 ? "left" : (i == 2 ? "center" : "right"); //figure out where we are
        b[i] = b[i-1]; //start where we left off
        while (1){
            printf("\rCalibrating %s set point. Value is %d\n", text,b[i]); //tell them every time
            input = getchar();
            if (input == 'l'){
                b[i] = b[i] - 10; //turning left is decrementing
                PCA0CP0 = 0xFFFF - b[i];
            }
            else if(input == 'r'){
                b[i] = b[i] + 10; //turn right is incrementing
                PCA0CP0 = 0xFFFF - b[i];
            }
            else if (input == 'q'){
                break;
            }
            PCA0CP0 = 0xFFFF - b[i]; // update the position
        }
    }
    //now that we have finished up, assign the results to the globals
    r1 = b[1];
    r2 = b[2];
    r6 = b[3];
    r = r6;
    //go back to center
    while(r>r2){
        r -= 10;
        counts = 2;
        while(counts){}
        PCA0CP0 = 0xFFFF - r;
    }
}

unsigned int read_compass(void){
    unsigned char ad = 0xC0; // address of  the compass
    unsigned char d[2];
    //printf("deep");
    i2c_read_data(ad,2,d,2); //start at register 2 and read two bytes
    //printf("back");
    return (unsigned int) d[0] << 8 | d[1]; //combine the two bytes into one value
}
void Compass_Steering (int actual, int desired){
    signed int angle =  actual - desired;
    //printf("this is the angle we need to turn %d\n", angle);
    if (angle > 0)
    {
        if (angle > 1800)
        {
            r = TURN_RIGHT(angle-1800,r,r6);
            
        }
        else
        {
            r = TURN_LEFT(angle,r,r1);
        }

    }
    else
    {
        if (angle < -1800)
        {
            angle = -(angle + 1800);
            r = TURN_LEFT(angle,r,r1);
        }
        else
        {
            r = TURN_RIGHT(-angle,r,r6);
        }
    }
    //printf("\rr: %u\n", r);
    PCA0CP0 = 0xFFFF - r; //set the value of the pulse width we want to use
}

