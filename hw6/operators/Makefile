default: Mvector MvectorTest Poly

Mvector: Mvector.o
	g++ -g -Wall -std=c++11 Mvector.o -o Mvector

Mvector.o: Mvector.cpp Mvector.h
	g++ -c Mvector.cpp

MvectorTest: MvectorTest.o
	g++ -g -Wall -std=c++11 MvectorTest.o -o MvectorTest

MvectorTest.o: MvectorTest.cpp Mvector.h
	g++ -c MvectorTest.cpp

Poly: Poly.o
	g++ -g -Wall -std=c++11 Poly.o -o Poly

Poly.o: Poly.cpp Poly.h
	g++ -c Poly.cpp