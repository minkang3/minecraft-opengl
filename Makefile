all:
	mkdir -p build
	g++ -g0 -o ./build/MC src/* -lglfw -I./include
