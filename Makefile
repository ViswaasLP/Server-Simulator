wlan: frame.o main.o
	g++ -g -Wall -o wlan frame.o main.o


frame.o: frame.cpp frame.h 
	g++ -g -Wall -c frame.cpp

main.o: host.h main.cpp frame.o
	g++ -g -Wall -c main.cpp

clean:
	$(RM) des main.o frame.o 
