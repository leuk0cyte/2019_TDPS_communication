#include "TDPS.h"
Serial extern pc;
Serial radio_pin(p28,p27,9600);

extern DigitalOut led3,led4;

extern int error_code;

int radio_init(){
  if(radio_pin.writeable()){
    return SUCCESS;
  }else{
    error_code = RADIO_INIT_ERROR;
    return FAILED;
  }
}

int radio(){
  led3 = 1;
	char time[9];

	while(pc.readable()!=1){
		// radio.printf("waitting for time info...");
    led4 = !led4;
		wait(0.1); 
	}
  led4 = 0;
  
	pc.scanf("%s",time);
	wait(0.2);
	radio_pin.printf("read time secceffuly\r\n");
	wait(0.2);
	radio_pin.printf("																																								\r\n\
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
    radio_pin.printf("\
		\r\n|      GUID \t|      NAME    \t|\
		\r\n|    2289035\t|    You,Hao   \t|\
		\r\n|    2289036\t|   Deng,Chao  \t|\
		\r\n|    2289037\t|   Sun,Huili  \t|\
		\r\n|    2289039\t|    He,Feiyu  \t|\
		\r\n|    2289042\t|   Su,Haoyuan \t|\
		\r\n|    2289043\t|  Zeng,Yongqin\t|\
		\r\n|    2289044\t|    Li,Guanqi \t|\
		\r\n|    2289046\t|    Li,Yiwei  \t|\
		\r\n|       TIME: %s CST\t|\
    ",time);
    led3 = 0;
		return SUCCESS;
	}
