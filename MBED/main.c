#include "mbed.h"
#include "TDPS.h"

// error code list
#define SUCCESS         000
#define FIELD           001
#define RGB_ERROR       002
#define ARM_ERROR       003
#define RADIO_ERROR     004
#define MODE_CODE_ERROR 005

// mode code list
#define RGB   002
#define ARM   003
#define RADIO 004

Serial pc(USBTX,USBRX,9600); 

//led for debugging
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

int main() {
    led4=0;
    int mode_code = 0;

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
    case RGB:
        rgb();
        break;

    case ARM:
        arm();
        break;
    
    case RADIO:
        radio();
        break;

    default:
        error_code = MODE_CODE_ERROR;
        break;
    }
    return mode_code;
}