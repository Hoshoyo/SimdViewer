all:
	mkdir -p bin
	gcc -g -march=znver4 -Iinclude src/*.c -o bin/SimdViewer -Llib -lraylib -lm