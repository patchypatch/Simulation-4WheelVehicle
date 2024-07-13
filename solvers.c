#include "simulation.h"

#ifdef __cplusplus
extern "C" {
#endif

void forward_euler_step(double* x, double* xdot, void(*pf)(double*,double*,double*), double* u) {
	(*pf)(xdot, x, u); // xdot

	// add contribution of forward Euler step to x ( x <- (SIM_TS) * xdot + x)
	cblas_daxpy(DIM_X, SIM_TS, xdot, INCX_DEFAULT, x, INCY_DEFAULT);
}

void rk4_step(double* x, double* xdot, void(*pf)(double*,double*,double*), double* u) {
	static double k[DIM_X];
	static double xf[DIM_X];
	cblas_dcopy(DIM_X, x, INCX_DEFAULT, xf, INCY_DEFAULT); // start result xf at x ( xf <- x )

	(*pf)(xdot, x, u); // f1
	// add contribution from f1 to result ( xf <- (SIM_TS/6.0) * xdot + xf)
	cblas_daxpy(DIM_X, SIM_TS/6.0, xdot, INCX_DEFAULT, xf, INCY_DEFAULT);
	
	// use k for input used to determine f2 ( k2 <- (SIM_TS/2.0) * f1 + x)
	cblas_dcopy(DIM_X, x, INCX_DEFAULT, k, INCY_DEFAULT); // ( k <- x)
	cblas_daxpy(DIM_X, SIM_TS/2.0, xdot, INCX_DEFAULT, k, INCY_DEFAULT); // ( k <- (SIM_TS/2.0) * f1 + k )
	(*pf)(xdot, k, u); // f2
	// add contribution from f2 to result ( xf <- (SIM_TS/3.0) * xdot + xf)
	cblas_daxpy(DIM_X, SIM_TS/3.0, xdot, INCX_DEFAULT, xf, INCY_DEFAULT);

	// use k for input used to determine f3 ( k3 <- (SIM_TS/2.0) * f2 + x )
	cblas_dcopy(DIM_X, x, INCX_DEFAULT, k, INCY_DEFAULT); // ( k <- x)
	cblas_daxpy(DIM_X, SIM_TS/2.0, xdot, INCX_DEFAULT, k, INCY_DEFAULT); // ( k <- (SIM_TS/2.0) * f2 + k )
	(*pf)(xdot, x, u); // f3
	// add contribution from f3 to result ( xf <- (SIM_TS/3.0) * xdot + xf)
	cblas_daxpy(DIM_X, SIM_TS/3.0, xdot, INCX_DEFAULT, xf, INCY_DEFAULT);

	// use k for input used to determine f4 ( k4 <- (SIM_TS) * f3 + x )
	cblas_dcopy(DIM_X, x, INCX_DEFAULT, k, INCY_DEFAULT); // ( k <- x)
	cblas_daxpy(DIM_X, SIM_TS, xdot, INCX_DEFAULT, k, INCY_DEFAULT); // ( k <- (SIM_TS) * f3 + k )
	(*pf)(xdot, x, u); // f4
	// add contribution from f4 to result ( xf <- (SIM_TS/6.0) * xdot + xf)
	cblas_daxpy(DIM_X, SIM_TS/6.0, xdot, INCX_DEFAULT, xf, INCY_DEFAULT);

	// back-calculate xdot ( xdot <- (1/SIM_TS) * (xf - x) )
	cblas_dcopy(DIM_X, xf, INCX_DEFAULT, xdot, INCY_DEFAULT); // ( xdot <- xf )
	cblas_daxpy(DIM_X, -1.0, x, INCX_DEFAULT, xdot, INCY_DEFAULT); // ( xdot <- -x + xdot )
	cblas_dscal(DIM_X, SIM_FS, xdot, INCX_DEFAULT); // ( xdot <- (1/SIM_TS) * xdot )

	// update x with xf
	cblas_dcopy(DIM_X, xf, INCX_DEFAULT, x, INCY_DEFAULT); // ( x <- xf )
}

#ifdef __cplusplus
}
#endif