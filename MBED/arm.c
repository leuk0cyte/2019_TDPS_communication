#include "mbed.h"

// pwm for robotic arm
PwmOut pwm1(p21);
PwmOut pwm2(p22);
PwmOut pwm3(p23);
PwmOut pwm4(p24);

int error_code = SUCCESS;

int mode_receive();
int mode_choose(int mode_code);




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

void radio(){
	char time[9];
	pc.putc('T'); //send time requests
	wait(0.1);

	while(pc.readable()!=1){
		// radio.printf("waitting for time info...");
		wait(0.1);
	}
	pc.scanf("%s",time);
	wait(0.2);
	radio.printf("read time secceffuly\r\n");
	wait(0.2);
	radio.printf("																																										\r\n\
                                                         aaaaaaaaaa                                 \r\n\
                                                 aaaaaaaaaaaaaaaaaa                                 \r\n\
                                             aaaaaaaaaaaaaaaaaaaaaa                                 \r\n\
                                           aaaaaaaaaaaaaaaaaaaaaaa                                  \r\n\
                                         aaaaaaaaaaaaaaaaaaaaaaaaa                                  \r\n\
                      aa                aaaaaaaaaaaaaaaaaaaaaaaaa           a                       \r\n\
                     aaaa               aaaaaaaaaaaaaaaaaaaaaaaaa       aaaaaaa                     \r\n\
                       aaa               aaaaaaaaaaaaaaaaaaaaaaa    aaaaaaaaaaaaaaaaaaaaa           \r\n\
                         aa              aaaaaaaaaaaaaaaaaaaaaa   aaaaaaaaaaaaaaaaaaaaaaaa          \r\n\
                          aaa           aaaaaaaaaaaaaaaaaaaaaa   aaaaaaaaaaaaaaaaaaaaaaaaaa         \r\n\
                            aaa        aaaaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa      \r\n\
                              aaaa  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa    \r\n\
                                aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\n\
                                  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \r\n\
                                    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  \r\n\
                                      aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  \r\n\
                                        aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa    \r\n\
        aaaaaaaa         aaaaaaa aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa         \r\n\
           aaaaaaa          aa     aaaaaa   aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa            \r\n\
           aaaaaaaaa        aa     aaaaaa     aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                    \r\n\
           aa aaaaaaaa      aa     aaaaaa     aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                        \r\n\
           aa   aaaaaaaa    aa     aaaaaa    aaa aaaaaaaaaaaaaaaaaaaaaaaaaaa                        \r\n\
           aa     aaaaaaaa  aa     aaaaaaaaaaaaa   aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                 \r\n\
           aa       aaaaaaaaaa     aaaaaa    aaa     aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa               \r\n\
           aa         aaaaaaaa     aaaaaa     aa       aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa             \r\n\
           aa           aaaaaa     aaaaaa           aa   aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa             \r\n\
           aa             aaaa     aaaaaa         aaa      aaaaaaaaaaaaaaaaaaaaaaaaaaaaa            \r\n\
        aaaaaaaa            aa   aaaaaaaaaaaaaaaaaaaa        aaaaaaaaaaaaaaaaaaaaaaaaaaa            \r\n\
a                                                              aaaaaaaaaaaaaaaaaaaaaaaaaa           \r\n\
  aaaa                             aaaaaa    aaaaaa  aaaaaaa     aaaaaaaaaaaaaaaaaaaaaaaaa          \r\n\
 aaaa                              aaaaaa     aaaaaa   aaaaaaa     aaaaaaaaaaaaaaaaaaaaaaaa         \r\n\
 a   a                             aaaaaa      aaaaaa    aaaaaaa     aaaaaaaaaaaaaaaaaaa            \r\n\
      aa                           aaaaaa     aaaaaa       aaaaaaa     aaaaaaaaaaaaaaaaa            \r\n\
    a                              aaaaaa    aaaaaa          aaaaaaa   aaaaaaaaaaaaaaaaaaaa         \r\n\
     aa                            aaaaaaaaaaaaaa             aaaaaaaa aa aaaaaaaaaaaaaaaaaa        \r\n\
       a                           aaaaaa  aaaaaaaa             aaaaaaaaa   aaaaaaaaaaaaaaaa        \r\n\
         aa                        aaaaaa    aaaaaaaa             aaaaaaa     aaaaaaaaaaaaaa        \r\n\
         a  a                      aaaaaa      aaaaaaaa             aaaaa       aaaaaaaaaaa         \r\n\
         a                        aaaaaaa        aaaaaaaa             aaa         aaaaaaaaa         \r\n\
         a    a                                                                     aaaaaa          \r\n\
            a a a                                                                     aaaaa         \r\n\
              a                                                                         aaa         \r\n\
                 a a  a                                                               a   a         \r\n\
                  a  a  a                                                       a a                 \r\n\
                        a                                                  a  a aa  a               \r\n\
                       a    a a                                        a a aaa   a                  \r\n\
                             a  a    a                           aaa  a    a                        \r\n\
                               a    a  aa a  a  aaaa aa    a a      a  a                            \r\n\
                                      a    a  aa  a  aa  a a a a  a  a                              \r\n\
                                           aa a   a    a a a a                                      \r\n\
    ");
    radio.printf("\
		\r\n|      GUID \t|      NAME    \t|\
		\r\n|    2289035\t|    You,Hao   \t|\
		\r\n|    2289036\t|   Deng,Chao  \t|\
		\r\n|    2289037\t|   Sun,Huili  \t|\
		\r\n|    2289039\t|    He,Feiyu  \t|\
		\r\n|    2289042\t|   Su,Haoyuan \t|\
		\r\n|    2289043\t|  Zeng,Yongqin\t|\
		\r\n|    2289044\t|    Li,Guanqi \t|\
		\r\n|    2289046\t|    Li,Yiwei  \t|\
		\r\n|       TIME: %s CST         \t|\
    ",time);
	}