
g++ -c -g -Wall -O0 -o /tmp/picoGL.o picoGL.cpp
g++ -c -g -Wall -O0 -o /tmp/raster.o raster.cpp
g++ -c -g -Wall -O0 -o /tmp/example_tri.o example_tri.cpp

g++ -o test.elf /tmp/picoGL.o /tmp/raster.o /tmp/example_tri.o -lSDL -lm 
