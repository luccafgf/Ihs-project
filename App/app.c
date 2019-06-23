#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <omp.h>
#include <time.h>


#define SWITCHES            1
#define BUTTONS             2
#define GREEN_LED           3
#define RED_LED             4
#define SEVEN_DISPLAYS_4    5
#define SEVEN_DISPLAYS_2    6

#define DISPLAY_HEX0        0
#define DISPLAY_HEX1        1
#define DISPLAY_HEX2        2
#define DISPLAY_HEX3        3
#define DISPLAY_HEX4        4
#define DISPLAY_HEX5        5
#define DISPLAY_HEX6        6
#define DISPLAY_HEX7        7

#define DISPLAY_ZERO        64
#define DISPLAY_ONE    		121
#define DISPLAY_TWO			36
#define DISPLAY_THREE		48
#define DISPLAY_FOUR		25
#define DISPLAY_FIVE		18
#define DISPLAY_SIX			2
#define DISPLAY_SEVEN 		7
#define DISPLAY_EIGHT		0
#define DISPLAY_NINE 		16
#define DISPLAY_A           8
#define DISPLAY_B			3
#define DISPLAY_C           70
#define DISPLAY_D			33
#define DISPLAY_E 			6
#define DISPLAY_F 			14

void changeDisplay(int altera, int display, uint32_t value);
void readSwitch(int altera);
void changeLedRed(int altera, uint32_t value);
void readButton(int altera);
void reset(int altera);

void delay(int num_of_mili) {
	int mili_sec = 1000*num_of_mili;
	clock_t start_time = clock();
	while(clock() < start_time + mili_sec);
} 

int main() {
  
  int altera = open("/dev/de2i150_altera", O_RDWR);
  
  const uint32_t led_1 = 3, led_2 = 65535, led_3 = 142545, led_4 = 192;

  //changeDisplay(altera, DISPLAY_HEX0, DISPLAY_A);
  //readSwitch(altera);
  reset(altera);

  close(altera);
  return 0;
}

void setup() {	

}

void start() {

} 

void reset(int altera){
	uint32_t valueDisplay = 4294967295; 
	write(altera, &valueDisplay, SEVEN_DISPLAYS_4);
	write(altera, &valueDisplay, SEVEN_DISPLAYS_2);
	readSwitch(altera);
}

void readSwitch(int altera){
	uint32_t actualValue = 0;

	read(altera, &actualValue, SWITCHES);
	changeLedRed(altera, actualValue);
}

void changeLedRed(int altera, uint32_t value){
	write(altera, &value, RED_LED);
}

void readButton(int altera){
	uint32_t actualValue = 0;

	read(altera, &actualValue, BUTTONS);
	printf("botao: %u\n", actualValue);
}

void changeDisplay(int altera, int display, uint32_t value) {

	uint32_t actualValue = 0;
	

	switch(display) {
	case DISPLAY_HEX0:
		read(altera, &actualValue, SEVEN_DISPLAYS_4);
		actualValue = ((actualValue & 0xffffff00) | value);
		//printf("HEX0 depois: %u \n", actualValue);
		write(altera, &actualValue, SEVEN_DISPLAYS_4);
		//printf("HEX0 final: %u \n", actualValue);
		delay(100);
		break;
	case DISPLAY_HEX1:
		read(altera, &actualValue, SEVEN_DISPLAYS_4);
		//printf("HEX1 antes: %u \n", actualValue);
		actualValue = ((actualValue & 0xffff00ff) | (value << 8));
		//printf("HEX1 depois: %u \n", actualValue);
		write(altera, &actualValue, SEVEN_DISPLAYS_4);		
		//printf("HEX1 final: %u \n", actualValue);
		delay(100);
		break;
	case DISPLAY_HEX2:
		read(altera, &actualValue, SEVEN_DISPLAYS_4);
		actualValue = ((actualValue & 0xff00ffff) | (value << 16));
		write(altera, &actualValue, SEVEN_DISPLAYS_4);
		delay(100);
		break;
	case DISPLAY_HEX3:
		read(altera, &actualValue, SEVEN_DISPLAYS_4);
		actualValue = ((actualValue & 0x00ffffff) | (value << 24));
		write(altera, &actualValue, SEVEN_DISPLAYS_4);
		delay(100);
		break;

	case DISPLAY_HEX4:
		read(altera, &actualValue, SEVEN_DISPLAYS_2);
		actualValue = ((actualValue & 0xffffff00) | value);
		write(altera, &actualValue, SEVEN_DISPLAYS_2);
		delay(100);
		break;
	case DISPLAY_HEX5:
		read(altera, &actualValue, SEVEN_DISPLAYS_2);
		actualValue = ((actualValue & 0xffff00ff) | (value << 8));
		write(altera, &actualValue, SEVEN_DISPLAYS_2);
		delay(100);
		break;
	case DISPLAY_HEX6:
		read(altera, &actualValue, SEVEN_DISPLAYS_2);
		actualValue = ((actualValue & 0xff00ffff) | (value << 16));
		write(altera, &actualValue, SEVEN_DISPLAYS_2);		
		delay(100);
		break;
	case DISPLAY_HEX7:
		read(altera, &actualValue, SEVEN_DISPLAYS_2);
		actualValue = ((actualValue & 0x00ffffff) | (value << 24));
		write(altera, &actualValue, SEVEN_DISPLAYS_2);
		delay(100);
		break;
	default:
		break;
	}


}

