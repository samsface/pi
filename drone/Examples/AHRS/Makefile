CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) -std=c++0x $(INCLUDES) AHRS.cpp $(NAVIO)/MPU9250.cpp -o AHRS -lrt -lpthread

clean:
	rm AHRS
