#include <unistd.h>
#include <fcntl.h>
#include <time.h>


#define SWITCHES            1
#define BUTTONS             2
#define GREEN_LED           3
#define RED_LED             4
#define SEVEN_DISPLAYS_4    5
#define SEVEN_DISPLAYS_2    6


unsigned char display_left[]
unsigned char hexdigit[] = {0x3F, 0x06, 0x5B, 0x4F,
							0x66, 0x6D, 0x7D, 0x07, 
                            0x7F, 0x6F, 0x77, 0x7C,
			                0x39, 0x5E, 0x79, 0x71};

int main() {
  int i, j, k;

  
  int altera = open("/dev/de2i150_altera", O_RDWR);
  
  const uint32_t led_1 = 3, led_2 = 48, led_3 = 142, led_4 = 192;

  write(altera, &led_1, 4);
  
  write(altera, &led_1, 5);
  
  
  
  
  
  
  
  
  
/*
  for (i=0; i>-1; i++) {
    read(altera, j&, 4);
    k =  hexdigit[j & 0xF]
      | (hexdigit[(j >>  4) & 0xF] << 8)
      | (hexdigit[(j >>  8) & 0xF] << 16)
      | (hexdigit[(j >> 12) & 0xF] << 24);
    k = ~k;
    write(altera, &k, 4);
  }
*/
  close(altera);
  return 0;
}

