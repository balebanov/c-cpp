all: main1

main1.o: main1.cpp
	g++ -c main1.cpp

main1: main1.o
	g++ main1.o -o main1 -lsfml-graphics -lsfml-window -lsfml-system

clear:
	rm -rf *.o hello