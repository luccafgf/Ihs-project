# SoundBoard


## Running

Open the terminal and look for the folder __/Driver__. When entering it through the terminal, execute the commands:

	make
	sudo insmod altera_driver.ko
	dmesg
	sudo mknod /dev/de2i150_altera c 91 1
	
With this commands, you are installing and running your own custom drive file. This driver maps the board's inputs and outputs to specific memory locations and takes care of the routines required for driver open, close, write, and read operations.

Now go to the __/App__ folder also from the terminal and run the following commands:

	gcc -L/usr/local/lib -lSDL2 -lSDL2_mixer -fopenmp -lm app.c -o app
	make
	sudo ./app
	
With these commands you can execute a given program that you will be able to access through the read and write system operations.

## Authors

- [Gabriel Maciel](https://github.com/Confucioo) - gmc2@cin.ufpe.br
- [Leticia Maciel](https://github.com/leehmaciel) - lcbm2@cin.ufpe.br
- [Lucca França](https://github.com/luccafgf) - lfgf@cin.ufpe.br
- [Matheus Costa](https://github.com/costa402) - mcbs@cin.ufpe.br

