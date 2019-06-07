#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>


#define SWITCHES            1
#define BUTTONS             2
#define GREEN_LED           3
#define RED_LED             4
#define SEVEN_DISPLAYS_4    5
#define SEVEN_DISPLAYS_2    6

unsigned char hexdigit[] = {0x3F, 0x06, 0x5B, 0x4F,
							0x66, 0x6D, 0x7D, 0x07, 
                            0x7F, 0x6F, 0x77, 0x7C,
			                0x39, 0x5E, 0x79, 0x71};

int main() {
  int i, j, k;

  
  int altera = open("/dev/de2i150_altera", O_RDWR);
  
  const uint32_t led_1 = 3, led_2 = 82222888, led_3 = 142545, led_4 = 192;

  write(altera, &led_2, 3);

  write(altera, &led_2, 4);  
  
  close(altera);
  return 0;
}

