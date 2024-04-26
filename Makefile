main: main.cpp
	g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o game -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f *.o game prog *~ core
