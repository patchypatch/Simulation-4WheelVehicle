#include <math.h>
#include <cblas.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h> // for mmap logging
#include <sys/stat.h> // for mmap logging
#include <sys/types.h> // for mmap logging
#include <fcntl.h> // for mmap logging
#include <unistd.h> // for mmap logging
#include <string.h> // for mmap logging

// Simulation size constants
#define DIM_X 6
#define DIM_U 8

// Simulation Time
#define SIM_T_END 120U // seconds
#define SIM_TS 0.001 // seconds
#define SIM_FS 1000 // Hz
#define LOG_SIZE (size_t)(sizeof(double)*(SIM_FS*SIM_T_END + 1)) // plus 1 for IC (time 0)

// BLAS
#define INCX_DEFAULT 1
#define INCY_DEFAULT 1