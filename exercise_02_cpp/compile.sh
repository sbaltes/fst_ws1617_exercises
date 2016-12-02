#!/bin/bash
g++ -I./blaze-3.0 -Ofast -fno-math-errno -fno-signed-zeros -fomit-frame-pointer -fassociative-math -freciprocal-math -fno-trapping-math -frename-registers -funroll-loops -flto -march=native --std=c++1y -Werror -Wextra -o test *.cpp -lSDL2 -fopenmp
./test
