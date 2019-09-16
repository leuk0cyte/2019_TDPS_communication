#include "TDPS.h"

Serial pc(USBTX,USBRX,9600); 

//led for debugging
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

uint8_t error_code = SUCCESS;
uint8_t mode_code = NONE;

char mbed_result = 0;

int mode_receive();
int mode_choose(uint8_t mode_code);
int init();

int main() {
    led4=0;
    int mode_code = '\0';
    while (error_code==SUCCESS)
    {
        mode_code=mode_receive();
        mode_choose(mode_code);
			
        if(error_code!=SUCCESS){
            break;
        }else
        {
            pc.printf("%i%i%c",SUCCESS,mode_code,mbed_result);
        }
        mbed_result = '\0';
    }

    pc.printf("%i%i",FAILED,error_code);
    led4 = 1;
}
int init(){
	arm_init();
    rgb_init();
    if(radio_init()==SUCCESS){
        return SUCCESS;
    }else    {
        return FAILED;
    }
}
int mode_receive(){
    int mode_code;
    pc.scanf("%i",&mode_code);
    return mode_code;
}

int mode_choose(uint8_t mode_code){
    switch (mode_code)
    {
        case INIT:
            init();
            break;

        case RGB:
            rgb();
            break;

        case WHITE_BALANCE:
            white_balance();
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
