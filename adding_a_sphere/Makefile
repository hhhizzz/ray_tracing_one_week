debug: main.cpp
	g++ -std=c++17 -fdiagnostics-color=always -g -I .. main.cpp

all: main.cpp
	g++ -O3 -std=c++17 -I .. main.cpp -o sphere

run: all
	./sphere > a.ppm

clean:
	-rm sphere a.ppm a.out
	-rm -r a.out.dSYM