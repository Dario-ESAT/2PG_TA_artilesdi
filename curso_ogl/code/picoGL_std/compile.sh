
g++ -c -g -Wall -O0 -o /tmp/picoGL.o picoGL.cpp
g++ -c -g -Wall -O0 -o /tmp/raster.o raster.cpp
g++ -c -g -Wall -O0 -o /tmp/example_rings.o example_rings.cpp

g++ -o test.elf /tmp/picoGL.o /tmp/raster.o /tmp/example_rings.o -lSDL -lm 
