del *.o
del myrsa.exe
g++ -O2 -c main.cpp -o main.o
g++ -c simpleBigint.cpp simpleModexp.cpp quickModexp.cpp layersModexp.cpp simpleMontgomery.cpp myRSA.cpp crtRSA.cpp main.cpp
g++ -o myrsa main.o myRSA.o crtRSA.o simpleMontgomery.o layersModexp.o quickModexp.o simpleModexp.o simpleBigint.o
