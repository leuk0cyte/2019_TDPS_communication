#include "mbed.h"
#include "string.h"
#include <cmath>
Serial pc(USBTX, USBRX); // tx, rx
DigitalOut S0(p6); 
DigitalOut S1(p7); 
DigitalOut LED(p18);
BusOut color(p19,p20);//S3 ,S2 
//0-red;1-blue;2-clear;3-green
InterruptIn channal(p5);// used to read pwm ,out brown line

bool once=true;
int r,g,b;
int R,G,B;
int Max, Result;
float con[3];
int n[4];// white balance start
int ct=500;// count time
volatile int number = 0;

void count(){
    ++number;
    }
    
void getchannal(int time){ // count the number of pulse in one second
    number = 0;
    wait_ms(time);
    pc.printf("number=%d\n",number);
}

int main(){
    channal.rise(&count);
    while(1){
        LED=1;
        S0=0;
        S1=1;
        
        for (int i=0;i<4;i++){
            if(i!=2){
                color=i;
                getchannal(ct);
                n[i]=number;
            }
        }
        r=n[0];
        b=n[1];
        g=n[3];
        pc.printf("r=%d,g=%d,b=%d\n",r,g,b);
        if(once){
            con[0]=255.0/r; // get the ratio factor for red
            con[1]=255.0/b; // get the ratio fact
            con[2]=255.0/g; // get the ratio factor for blue
            pc.printf(" white balance sucessful");
            once = false;
        }
        R=con[0]*n[0];
        B=con[1]*n[1];
        G=con[2]*n[3];
        pc.printf("R=%d,G=%d,B=%d\n",R,G,B);
        LED = 0;
        //wait(3);
        if (R<G) {
            Max = G;
            Result=2;
        }else {
            Max = R;//red
            Result=1;
        }
            
        if  (Max<B) 
        {
            Max = B;// blue
            Result=3;
            }
        pc.printf("%d\n",Result);
        
        getchar();
    }
}
