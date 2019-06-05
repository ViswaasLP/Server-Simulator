wlan: frame.o printmain.o
	g++ -g -Wall -o wlan frame.o printmain.o


frame.o: frame.cpp frame.h 
	g++ -g -Wall -c frame.cpp

printmain.o: host.h printmain.cpp frame.o
	g++ -g -Wall -c printmain.cpp

clean:
	$(RM) des printmain.o frame.o 
