#!/bin/sh
g++ -std=c++11 -Ofast -g -fopenmp *.cpp GeometricTypes/*.cpp ObjectTypes/*.cpp ObjectTypes/ObjectSubclasses/*.cpp Lights/*.cpp Utilities/*.cpp -lm -o RayTracer
