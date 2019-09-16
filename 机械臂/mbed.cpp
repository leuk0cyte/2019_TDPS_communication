#include "mbed.h"

// error code list
#define SUCCESS   000
#define FIELD     001
#define ARM_ERROR 002

// mode code list
#define ARM 002

PwmOut pwm1(p21);
PwmOut pwm2(p22);
PwmOut pwm3(p23);
PwmOut pwm4(p24);
// pwm for robotic arm

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

Serial pc(USBTX,USBRX,9600);

uint_8 error_code = SUCCESS;

int mode_receive();
int mode_choose(int mode_code);
int arm();

int main() {
    led4=0;
    int mode_code = 0;
    int status_code = 0;

    while (error_code==SUCCESS)
    {
        mode_code=mode_receive();
        mode_choose(mode_code);
			
        if(error_code!=SUCCESS){
            break;
        }else
        {
            pc.printf("mode %i has been executed successfully\n",mode_code);
        }
    }

    pc.printf("program exit unsuccessfully, error code %i\n",error_code);
    led4 = 1;
}

int mode_receive(){
    int mode_code;
    pc.scanf("%i",&mode_code);
    return mode_code;
}

int mode_choose(int mode_code){
    switch (mode_code)
    {
    case ARM:
        arm();
        break;
    
    default:
        break;
    }
    return mode_code;
}

int arm(){
    led1=1;
    float i,j,k,l,m,n,o,p;
    pwm1.period(0.020);
    pwm2.period(0.020);
    pwm3.period(0.020);
    pwm4.period(0.020);
    pwm1=0.078;
    pwm2=0.065;
    pwm3=0.055;
    pwm4=0.095;
    wait(1);
    for(i=0.065;i>=0.025;i=i-0.001){
        pwm2=i;
        wait(0.05);
    }
    for(k=0.055;k<=0.095;k=k+0.001){
        pwm3=k;
        wait(0.05);
    }
    for(l=0.095;l>=0.055;l=l-0.001){
        pwm4=l;
        wait(0.05);
    }
    for(o=0.078;o<=0.095;o=o+0.001){
        pwm1=o;
        wait(0.05);
    }
    wait(1);
    for(p=0.095;p>=0.078;p=p-0.001){
        pwm1=p;
        wait(0.05);
    }
    for(m=0.095;m>=0.055;m=m-0.001){
        pwm3=m;
        wait(0.05);
    }
    for(n=0.055;n<=0.095;n=n+0.001){
        pwm4=n;
        wait(0.05);
    }
    for(j=0.025;j<=0.065;j=j+0.001){
        pwm2=j;
        wait(0.05);
    }
    led1=0;
    return SUCCESS;
}

