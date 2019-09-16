#include "TDPS.h"
extern Serial pc;
extern uint8_t error_code;
extern DigitalOut led1;

//S0=L;S1=H: output frequency scaling = 2%
//S0=H;S1=L: output frequency scaling = 20%
//S0=L;S1=L: power off
//rgb_LED: LED power
DigitalOut S0(p6),S1(p7),rgb_LED(p18);

//0-red;1-blue;2-clear;3-green;
BusOut set_color(p19,p20);

//to count pwn rise edge
InterruptIn pwm_read(p5);

extern char mbed_result;

const int CT = 500;//countting period, unit ms.

bool done_white_banlance = false;
double factor[3];//factor for white balance
int rgb_read[4];
volatile int counter = 0;

void count(){
	++counter;
}

//initialize rgb module
void rgb_init(){
	pwm_read.enable_irq();
	pwm_read.rise(&count);
	rgb_LED=1;
    S0=0;
    S1=1;
}

void rgb_off(){
	pwm_read.disable_irq();
	rgb_LED = 0;
	S0 = 0;
	S1 = 0;
}

//read the rise edge in a certain time period
int get_channel(int CT){
	counter = 0;
	wait_ms(CT);
	int count_value = counter;
	return count_value;
}

//read the rgb value and store it in memory
void get_rgb(){
	for (int i=0;i<4;i++){
		if(i!=2){
			set_color = i;
			rgb_read[i] = get_channel(CT);
		}
	}
}

//for white_balance
void white_balance(){
	rgb_init();
	int r,g,b;
	get_rgb();
	r=rgb_read[0]; b=rgb_read[1]; g=rgb_read[3];
	factor[0]=255.0/r; factor[1]=255.0/b;factor[2]=255.0/g;
	done_white_banlance = true;
	rgb_off();
//	pc.printf("\r\nrgb_read[0]= %i rgb_read[1]=%i rgb_read[3]=%i",rgb_read[0],rgb_read[1],rgb_read[3]);
//	pc.printf("\r\nfactor[0]= %f factor[1]= %f factor[2]=%f ",factor[0],factor[1],factor[2]);
}


//main function
char rgb(){
	led1=0;
	rgb_init();
	if(done_white_banlance==false){
		error_code = NO_WHITE_BALANCE;
		return FAILED;
	}
	int Maximum;
	int R,G,B;
	get_rgb();
	R=float(factor[0]*rgb_read[0]*0.85);
	B=float(factor[1]*rgb_read[1]);
	G=float(factor[2]*rgb_read[3]);
	Maximum=R>G?R:G;
	Maximum=Maximum>B?Maximum:B;
//	pc.printf("\r\nrgb_read[0]= %i rgb_read[1]=%i rgb_read[3]=%i",rgb_read[0],rgb_read[1],rgb_read[3]);
//	pc.printf("\r\nR= %i B=%i G=%i",R,B,G);
//	pc.printf("\r\nMaximum = %i",Maximum);
	if (Maximum==R)
	{
		mbed_result='r';
	}else if (Maximum==G)
	{
		mbed_result='g';
	}else if (Maximum==B)
	{
		mbed_result='b';
	}else
	{
		error_code = RGB_ERROR;
		rgb_off();
		return FAILED;
	}
//	pc.printf("\r\nrgb_result = %c",mbed_result);
	rgb_off();
	led1=0;
	return SUCCESS;
}

