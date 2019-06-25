#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL_mixer.h>


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
#define DISPLAY_G 			66

#define POS_CURSOR			0

void changeDisplay(int altera, int display, uint32_t value);
void readSwitch(int altera);
void changeLedRed(int altera, uint32_t value);
void readButton(int altera);
void reset(int altera);
void threadINPUT(int altera);
void threadPLAY(int altera);
void changeNote (int altera, int note);
void LoadPiano(Mix_Chunk **Notes);

int playing = 0; 
int positions[18][2] = {0};
int switchesOn;
int cursor = 0;
int instrumento = 0;
Mix_Chunk *PianoNotes[7];

void delay(int num_of_mili) {
	int mili_sec = 1000*num_of_mili;
	clock_t start_time = clock();
	while(clock() < start_time + mili_sec);
} 

int main() {
  
  // Abrir arquivos de drivers
  int altera = open("/dev/de2i150_altera", O_RDWR);

  Mix_Init(MIX_INIT_MID);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  LoadPiano(PianoNotes);
  
  const uint32_t led_1 = 3, led_2 = 65535, led_3 = 142545, led_4 = 192;

  //changeDisplay(altera, DISPLAY_HEX2, DISPLAY_C);
  //readSwitch(altera);
  reset(altera);
  //readButton(altera);
  #pragma omp parallel num_threads(2)
  {
  	#pragma omp master
  	{
  		//modo programador
  		threadINPUT(altera);
		
	}
	#pragma omp single
	{
		//modo play
		threadPLAY(altera);
	}
  }

 //close(altera);

  return 0;
}

void setup() {	
}

void start() {	
} 

void threadINPUT(int altera){
	while(1){
		readSwitch(altera);
		sleep(0.1);
		readButton(altera);
	}
}

void threadPLAY(int altera){
	int new;
	cursor = 0;
	int redLedValue;
	while(1){
		sleep(0.1);
		if(playing){
			//modo play
			redLedValue = pow(2, cursor);

			changeLedRed(altera, redLedValue);
			if(positions[cursor][0]){

				if(instrumento == 1){
					Mix_PlayChannel(-1, PianoNotes[positions[cursor][1]], 0);
				}
				else if(instrumento == 0){
					Mix_PlayChannel(-1, PianoNotes[positions[cursor][1]], 0);
				}
			}
			delay(500);
			cursor = (cursor+1)%18;
		}


		else{
			//modo programador
			new = pow(2,cursor);
			sleep(0.2);
			//printf("new:%d\n",new);

			static int old;
			if(old != new){
				//printf("\33[2K\r");
				printf("\n");
				changeLedRed(altera, new);
				old = new;
			}
		}
	}
}

void reset(int altera){
	//printf("OLAR\n");
	uint32_t valueDisplay = -1; 
	uint32_t aux = 0;
	cursor = POS_CURSOR;
	write(altera, &valueDisplay, SEVEN_DISPLAYS_4);
	write(altera, &valueDisplay, SEVEN_DISPLAYS_2);
	write(altera, &aux, GREEN_LED);
	readSwitch(altera);

	changeNote(altera, aux);
}

void readSwitch(int altera){
	uint32_t actualValue = 0;
	static uint32_t oldValue = 0;

	#pragma omp critical
	read(altera, &actualValue, SWITCHES);

	if(playing){
		if(actualValue != oldValue){
			switchesOn = actualValue;

			//transformando o switchesOn em binario;
			int k, i;
			for(i=31; i>=0; i--){
				k = switchesOn >> i;
				if(i<=17){
					if(k & 1){
						positions[i][0] = 1;
					}
					else{
						positions[i][0] = 0;
					}
				}
			}

			oldValue = actualValue;
		}

	}
	delay(100);
	//changeLedRed(altera, actualValue);
}

void changeLedRed(int altera, uint32_t value){
	#pragma omp critical
	write(altera, &value, RED_LED);
	delay(100);
}

void readButton(int altera){
	uint32_t actualValue = 0;
	static uint32_t oldValue = 15;
	static int nota = 0;

	#pragma omp critical
	read(altera, &actualValue, BUTTONS);
	delay(100);

	if(actualValue != oldValue){

		if(actualValue == 15){
			//printf("cursor: %d\n", cursor);
			switch(oldValue){
				case 14: //botao 0
					cursor = -1;
					playing = !playing;
					//printf("valor de playing: %d\n", playing);
					break;

				case 13: //botao 1
					nota = (nota + 1)%7;
					//printf("n %d\n", nota);
					changeNote(altera, nota);
					positions[cursor][1] = nota;
					break;

				case 11: //botao 2
					if(cursor == 0){
						cursor = 17;
						printf("2: %d\n", cursor);

					}
					else{
						cursor = (cursor-1)%18;
						printf("2: %d\n", cursor);
					}

					nota = positions[cursor][1];
  					changeNote(altera, nota);
					break;

				case 7: //botao 3
					cursor = (cursor+1)%18;
					printf("3: %d\n", cursor);
					nota = positions[cursor][1];
  					changeNote(altera, nota);

					break;

				default:
					printf("sou v1d4 l0k4 \n");
					break;
			}	
		}

		oldValue = actualValue;

	}
	 

	//printf("botao: %u\n", actualValue);
}

void changeNote (int altera, int note){
	switch(note){
		case 0:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_A);
			break;

		case 1:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_B);
			break;

		case 2:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_C);
			break;

		case 3:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_D);
			break;

		case 4:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_E);
			break;

		case 5:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_F);
			break;

		case 6:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_G);
			break;

		default:
			changeDisplay(altera, DISPLAY_HEX0, DISPLAY_ZERO);
			break;

	}
}

void changeDisplay(int altera, int display, uint32_t value) {

	uint32_t actualValue = 0;
	
	#pragma omp critical
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

void LoadPiano(Mix_Chunk **Notes) {
	Notes[0] = Mix_LoadWAV("Samples/piano_a.wav");
	Notes[1] = Mix_LoadWAV("Samples/piano_b.wav");
	Notes[2] = Mix_LoadWAV("Samples/piano_c.wav");
	Notes[3] = Mix_LoadWAV("Samples/piano_d.wav");
	Notes[4] = Mix_LoadWAV("Samples/piano_e.wav");
	Notes[5] = Mix_LoadWAV("Samples/piano_f.wav");
	Notes[6] = Mix_LoadWAV("Samples/piano_g.wav");
}

