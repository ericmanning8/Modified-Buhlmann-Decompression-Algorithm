all: buhlmann

buhlmann: mainTest.cpp zhl16.o data.o gas.o tissue.o
	g++ -std=c++11 -g -Wall mainTest.cpp zhl16.o data.o gas.o tissue.o -o buhlmann

zhl16.o: Team_3_UDive/zhl16.h Team_3_UDive/zhl16.cpp
	g++ -std=c++11 -g -Wall -c Team_3_UDive/zhl16.cpp -o zhl16.o

data.o: Team_3_UDive/data.h Team_3_UDive/data.cpp
	g++ -std=c++11 -g -Wall -c Team_3_UDive/data.cpp -o data.o

gas.o: Team_3_UDive/gas.h Team_3_UDive/gas.cpp
	g++ -std=c++11 -g -Wall -c Team_3_UDive/gas.cpp -o gas.o

tissue.o: Team_3_UDive/tissue.h Team_3_UDive/tissue.cpp
	g++ -std=c++11 -g -Wall -c Team_3_UDive/tissue.cpp -o tissue.o