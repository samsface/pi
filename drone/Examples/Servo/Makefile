CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) -std=c++0x $(INCLUDES) Servo.cpp $(NAVIO)/PCA9685.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/gpio.cpp -o Servo

clean:
	rm Servo
