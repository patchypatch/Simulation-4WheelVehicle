BINARY=simulation.out

CC=g++

LFLAGS=-lm -I/opt/OpenBLAS/include

CFLAGS=-I/opt/OpenBLAS/include

BLAS=/opt/OpenBLAS/lib/libopenblas.a

all: $(BINARY)

$(BINARY): simulation.o solvers.o four_wheel_model.o PathErrorsLine.o PathErrorsCircle.o $(BLAS)
	$(CC) $^ $(LFLAGS) -o $@

simulation.o: simulation.c
	$(CC) $^ $(CFLAGS) -c

solvers.o: solvers.c
	$(CC) $^ $(CFLAGS) -c

four_wheel_model.o: four_wheel_model.c
	$(CC) $^ $(CFLAGS) -c

PathErrorsLine.o: PathErrorsLine.cpp
	$(CC) $^ $(CFLAGS) -c

PathErrorsCircle.o: PathErrorsCircle.cpp
	$(CC) $^ $(CFLAGS) -c

.PHONY: clean
clean:
	rm -f *.o *.out ./log/*.log