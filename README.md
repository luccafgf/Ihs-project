Aplicação usando a placa FPGA, integração Arduino e RFID. 

Em ../Driver

	make
	sudo insmod altera_driver.ko
	dmesg
	sudo mknod /dev/de2i150_altera c 91 1

Em ../App

	gcc -L/usr/local/lib -lSDL2 -lSDL2_mixer -fopenmp -lm app.c -o app
	make
	sudo ./app