# DENISE
Dynamical Evolution of Numerous Interacting Stars Engine

A collisional N-body code that computes the trajectories of particles under the influence of their gravity. Numerically, the code is based on the Hermite integration method. Besides, it is parallelized for shared memory architectures.

INSTALL:
go to 'src' and build the executable:
g++ -std=c++11 -pthread main.cpp -o DENISE -Ofast

USAGE:
modify 'parameters.txt' and execute the binary in the same directory
