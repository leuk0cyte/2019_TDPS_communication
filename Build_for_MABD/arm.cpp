#include "TDPS.h"

// pwm for robotic arm
PwmOut pwm1(p21);
PwmOut pwm2(p22);
PwmOut pwm3(p23);
PwmOut pwm4(p24);

extern DigitalOut led1;

void arm_init(){
    pwm1.period(0.020);
    pwm2.period(0.020);
    pwm3.period(0.020);
    pwm4.period(0.020);

    pwm1=0.078;
    pwm2=0.080;
    pwm3=0.055;
    pwm4=0.095;
}
int arm(){
    led1=1;
    float i,j,k,l,m,n,o,p;
    pwm1=0.078;
    pwm2=0.080;
    pwm3=0.055;
    pwm4=0.095;
    wait(1);
    for(i=0.080;i>=0.035;i=i-0.001){
        pwm2=i;
        wait(0.05);
    }
    for(k=0.055;k<=0.100;k=k+0.001){
        pwm3=k;
        wait(0.05);
    }
    for(l=0.095;l>=0.070;l=l-0.001){
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
    for(m=0.100;m>=0.055;m=m-0.001){
        pwm3=m;
        wait(0.05);
    }
    for(n=0.070;n<=0.095;n=n+0.001){
        pwm4=n;
        wait(0.05);
    }
    for(j=0.035;j<=0.080;j=j+0.001){
        pwm2=j;
        wait(0.05);
    }
    led1=0;
    return SUCCESS;
}
