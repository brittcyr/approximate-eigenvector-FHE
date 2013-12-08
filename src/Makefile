CC = g++
TARGET := main
CFLAGS = -g -Wshadow -Wall -I/afs/athena.mit.edu/user/c/y/cyrbritt/sw/include 

GMP=-lgmp 
LDLIBS = -L/afs/athena.mit.edu/user/c/y/cyrbritt/sw/lib -lntl $(GMP) -lm

HEADER = FHE.h Ctxt.h timing.h matrix_ops.h

SRC = FHE.cpp Ctxt.cpp timing.cpp matrix_ops.cpp
OBJ := $(addsuffix .o, $(basename $(SRC)))

%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c $< $(LDLIBS)

$(TARGET): $(OBJ) main.o
		$(CC) $(CFLAGS) $(OBJ) $(LDLIBS) main.o -o $@

clean:
	rm -f *.o *_x *_x.exe *.a core.*
	rm -rf *.dSYM
	rm main



