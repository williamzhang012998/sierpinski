#Makefile for Final Project

build:
	clang++ -O3 `fltk-config --use-gl --ldflags` -fsanitize=address MyGLCanvas.cpp main.cpp -Wno-deprecated-declarations -Wno-nonportable-include-path -o fp

clean:
	rm fp

