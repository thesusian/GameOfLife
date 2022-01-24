CC = g++
CFLAGS = -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

App: main.cpp
	$(CC) main.cpp -o GameOfLife $(LDFLAGS)

run: App 
	./GameOfLife
	
clean:
	rm -f GameOfLife
