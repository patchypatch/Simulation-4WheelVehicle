#include "simulation.h"
#include "solvers.h"
#include "models.h"
#include "pathErrors.hpp"

#ifdef __cplusplus
extern "C" {
#endif

double* init_logging(int* fd, const char* filepath) {
	/* Open data logging */
	double* log_in_memory;

	// Open file, create file if does not exist, truncate to length 0
	if((*fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600)) == -1) {
		printf("Error opening file for writing\n");
		exit(-1);
	}

	// Stretch the file size to the size of the (mmapped) array of char
	if (lseek(*fd, LOG_SIZE-1, SEEK_SET) == -1) {
		close(*fd);
		printf("Error calling lseek() to 'stretch' the file\n");
		exit(-1);
	}

	/* Something needs to be written at the end of the file to
	 * have the file actually have the new size.
	 * Just writing an empty string at the current file position will do.
	 *
	 * Note:
	 *  - The current position in the file is at the end of the stretched 
	 *    file due to the call to lseek().
	 *  - An empty string is actually a single '\0' character, so a zero-byte
	 *    will be written at the last byte of the file.
	 */
	if (write(*fd, "", 1) == -1) {
		close(*fd);
		printf("Error writing last byte of the file\n");
		exit(-1);
	}

	// Now the file is ready to be mmapped.
	log_in_memory = (double*)( mmap(0, LOG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0) );
	if (log_in_memory == MAP_FAILED) {
		close(*fd);
		printf("Error mmapping the file\n");
		exit(-1);
	}

	return log_in_memory;
}

void write_log(int* fd, double* log_in_memory) {
	// Write log to disk
	if (msync(log_in_memory, LOG_SIZE, MS_SYNC) == -1) {
		printf("Could not sync the file to disk\n");
	}

	// Free the mmapped memory
	if (munmap(log_in_memory, LOG_SIZE) == -1) {
		close(*fd);
		printf("Error un-mmapping the file\n");
		exit(-1);
	}

	// Close file since un-mmaping doesn't close it
	close(*fd);
}

int main(void) {	

	/* Setup data logging */
	int fd_time;
	int fd_x0;	int fd_x1;	int fd_x2;	int fd_x3;	int fd_x4;	int fd_x5;
	int fd_error0;	int fd_error1;
	double* log_error0 = init_logging(&fd_error0, "./log/error0[m].log");
	double* log_error1 = init_logging(&fd_error1, "./log/error1[deg].log");
	double* log_time = init_logging(&fd_time, "./log/time[s].log");
	double* log_x0 = init_logging(&fd_x0, "./log/x0[mps].log");
	double* log_x1 = init_logging(&fd_x1, "./log/x1[mps].log");
	double* log_x2 = init_logging(&fd_x2, "./log/x2[dps].log");
	double* log_x3 = init_logging(&fd_x3, "./log/x3[m].log");
	double* log_x4 = init_logging(&fd_x4, "./log/x4[m].log");
	double* log_x5 = init_logging(&fd_x5, "./log/x5[deg].log");

	/* Simulation Data Structures */
	double* xdot = (double*)malloc(DIM_X * sizeof(double));
	double* x = (double*)malloc(DIM_X * sizeof(double));
	double* u = (double*)malloc(DIM_U * sizeof(double));

	/* Initial Conditions */
	x[0] = 1; x[1] = 0; x[2] = 0; x[3] = 10; x[4] = 0; x[5] = -M_PI / 2;
	// Log IC data
	log_time[0] = (double) 0;
	log_x0[0] = (double) x[0];
	log_x1[0] = (double) x[1];
	log_x2[0] = (double) x[2]*180/M_PI;
	log_x3[0] = (double) x[3];
	log_x4[0] = (double) x[4];
	log_x5[0] = (double) x[5]*180/M_PI;
	// Terminal Output for IC's
	printf("[%d] x0[m/s] = %.3f   x1[m/s] = %.3f   x2[deg/s] = %.3f   x3[m] = %.3f   x4[m] = %.3f   x5[deg] = %.3f\n",
			0, log_x0[0], log_x1[0], log_x2[0], log_x3[0], log_x4[0], log_x5[0]);

	/* Setup Path */
	//PathErrorsLine Path(1.0f, 1.0f);
	PathErrorsCircle Path(10.0f);
	float* errors = (float*)malloc(2 * sizeof(float));
	float* positionVec = (float*)malloc(2 * sizeof(float));

	/* Simulate! */
	for(int i = 1; i <= SIM_T_END * SIM_FS; i++) {
		// Calculate Errors
		positionVec[0] = (float) x[3];
		positionVec[1] = (float) x[4];
		Path.getErrors(errors, positionVec, (float) x[5]);

		// Determine Control
		u[0] = -9*M_PI/180; u[1] = -9*M_PI/180; u[2] = 0*M_PI/180; u[3] = 0*M_PI/180;
		u[4] = 100; u[5] = 100; u[6] = 100; u[7] = 100;
		
		// Step Plant
		rk4_step(x, xdot, &four_wheel_model, u); // RK4
		//forward_euler_step(x, xdot, &four_wheel_model, u); // update x

		// Log data
		log_time[i] = (double) i * SIM_TS;
		log_x0[i] = (double) x[0];
		log_x1[i] = (double) x[1];
		log_x2[i] = (double) x[2] * 180/M_PI;
		log_x3[i] = (double) x[3];
		log_x4[i] = (double) x[4];
		log_x5[i] = (double) x[5] * 180/M_PI;
		log_error0[i-1] = (double) errors[0];
		log_error1[i-1] = (double) errors[1] * 180/M_PI;

		// Terminal Output (every SIM_FS/10 ms)
		if(i%(SIM_FS/10) == 0) {
			printf("[%d.%d]  x0[m/s] = %.3f   x1[m/s] = %.3f   x2[deg/s] = %.3f   x3[m] = %.3f   x4[m] = %.3f   x5[deg] = %.3f\n",
					i/SIM_FS, i/(SIM_FS/10)-i/SIM_FS*10,
					log_x0[i], log_x1[i], log_x2[i], log_x3[i], log_x4[i], log_x5[i]);
		}
	}

	// Last step does not need errors so hold last value for plots
	log_error0[SIM_T_END * SIM_FS] = log_error0[SIM_T_END * SIM_FS - 1];
	log_error1[SIM_T_END * SIM_FS] = log_error1[SIM_T_END * SIM_FS - 1];

	// Write logs
	write_log(&fd_time, log_time);
	write_log(&fd_x0, log_x0);
	write_log(&fd_x1, log_x1);
	write_log(&fd_x2, log_x2);
	write_log(&fd_x3, log_x3);
	write_log(&fd_x4, log_x4);
	write_log(&fd_x5, log_x5);
	write_log(&fd_error0, log_error0);
	write_log(&fd_error1, log_error1);

	// free heap memory
	free(xdot);
	free(x);
	free(u);
	free(errors);
	free(positionVec);
}

#ifdef __cplusplus
}
#endif