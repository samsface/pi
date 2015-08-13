CC = g++
NAVIO = Navio
INCLUDES = -I ../..

all:
	$(CC) -std=c++0x $(INCLUDES) drone.cpp $(NAVIO)/MPU9250.cpp $(NAVIO)/PCA9685.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/gpio.cpp -o drone -lrt
	$(CC) -std=c++0x $(INCLUDES) debug.cpp $(NAVIO)/MPU9250.cpp $(NAVIO)/PCA9685.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/gpio.cpp -o debug -lrt



clean:
	rm drone
