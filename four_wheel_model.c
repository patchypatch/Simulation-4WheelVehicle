#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

// Simulation parameters
#define m 1000.0 // kg
#define I_zz 100.0 // kg*m2
#define l_F_B 1.0 // meter
#define l_F_C0 0.5 // meter
#define l_F_C1 0.5 // meter
#define l_R_B 1.0 // meter
#define l_R_C2 0.5 // meter
#define l_R_C3 0.5 // meter
#define k_s_C0 500.0 // tire constant
#define k_s_C1 500.0 // tire constant
#define k_s_C2 500.0 // tire constant
#define k_s_C3 500.0 // tire constant
#define k_t_C0 100.0 // tire constant
#define k_t_C1 100.0 // tire constant
#define k_t_C2 100.0 // tire constant
#define k_t_C3 100.0 // tire constant
#define r_C0 0.1 // tire radius (meters)
#define r_C1 0.1 // tire radius (meters)
#define r_C2 0.1 // tire radius (meters)
#define r_C3 0.1 // tire radius (meters)

/* x[0]=v_x, x[1]=v_y, x[2]=omega_z, x[3]=X, x[4]=Y, x[5]=Psi
 * u[0]=delta_C0, u[1]=delta_C1, u[2]=delta_C2, u[3]=delta_C3
 * u[4]=rpm_C0, u[5]=rpm_C1, u[6]=rpm_C2, u[7]=rpm_C3
 * 
*/
void four_wheel_model(double* xdot, double* x, double* u) {
	xdot[0] = (-k_s_C0*sin(u[0])*atan2(-(l_F_B*x[2] + x[1])*cos(u[0]) + (l_F_C0*x[2] + x[0])*sin(u[0]), (l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0]))
			- k_s_C1*sin(u[1])*atan2(-(l_F_B*x[2] + x[1])*cos(u[1]) + (-l_F_C1*x[2] + x[0])*sin(u[1]), (l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1]))
			- k_s_C2*sin(u[2])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[2]) + (l_R_C2*x[2] + x[0])*sin(u[2]), (-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2]))
			- k_s_C3*sin(u[3])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[3]) + (-l_R_C3*x[2] + x[0])*sin(u[3]), (-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3]))
			+ k_t_C0*((1.0/30.0)*M_PI*r_C0*u[4]/((l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0])) - 1)*cos(u[0])
			+ k_t_C1*((1.0/30.0)*M_PI*r_C1*u[5]/((l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1])) - 1)*cos(u[1])
			+ k_t_C2*((1.0/30.0)*M_PI*r_C2*u[6]/((-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2])) - 1)*cos(u[2])
			+ k_t_C3*((1.0/30.0)*M_PI*r_C3*u[7]/((-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3])) - 1)*cos(u[3]) + m*x[1]*x[2])/m;

	xdot[1] = (k_s_C0*cos(u[0])*atan2(-(l_F_B*x[2] + x[1])*cos(u[0]) + (l_F_C0*x[2] + x[0])*sin(u[0]), (l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0]))
			+ k_s_C1*cos(u[1])*atan2(-(l_F_B*x[2] + x[1])*cos(u[1]) + (-l_F_C1*x[2] + x[0])*sin(u[1]), (l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1]))
			+ k_s_C2*cos(u[2])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[2]) + (l_R_C2*x[2] + x[0])*sin(u[2]), (-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2]))
			+ k_s_C3*cos(u[3])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[3]) + (-l_R_C3*x[2] + x[0])*sin(u[3]), (-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3]))
			+ k_t_C0*((1.0/30.0)*M_PI*r_C0*u[4]/((l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0])) - 1)*sin(u[0])
			+ k_t_C1*((1.0/30.0)*M_PI*r_C1*u[5]/((l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1])) - 1)*sin(u[1])
			+ k_t_C2*((1.0/30.0)*M_PI*r_C2*u[6]/((-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2])) - 1)*sin(u[2])
			+ k_t_C3*((1.0/30.0)*M_PI*r_C3*u[7]/((-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3])) - 1)*sin(u[3]) - m*x[0]*x[2])/m;

	xdot[2] = (k_s_C0*l_F_B*cos(u[0])*atan2(-(l_F_B*x[2] + x[1])*cos(u[0]) + (l_F_C0*x[2] + x[0])*sin(u[0]), (l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0]))
			- k_s_C0*l_F_C0*sin(u[0])*atan2(-(l_F_B*x[2] + x[1])*cos(u[0]) + (l_F_C0*x[2] + x[0])*sin(u[0]), (l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0]))
			+ k_s_C1*l_F_B*cos(u[1])*atan2(-(l_F_B*x[2] + x[1])*cos(u[1]) + (-l_F_C1*x[2] + x[0])*sin(u[1]), (l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1]))
			+ k_s_C1*l_F_C1*sin(u[1])*atan2(-(l_F_B*x[2] + x[1])*cos(u[1]) + (-l_F_C1*x[2] + x[0])*sin(u[1]), (l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1]))
			- k_s_C2*l_R_B*cos(u[2])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[2]) + (l_R_C2*x[2] + x[0])*sin(u[2]), (-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2]))
			- k_s_C2*l_R_C2*sin(u[2])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[2]) + (l_R_C2*x[2] + x[0])*sin(u[2]), (-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2]))
			- k_s_C3*l_R_B*cos(u[3])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[3]) + (-l_R_C3*x[2] + x[0])*sin(u[3]), (-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3]))
			+ k_s_C3*l_R_C3*sin(u[3])*atan2(-(-l_R_B*x[2] + x[1])*cos(u[3]) + (-l_R_C3*x[2] + x[0])*sin(u[3]), (-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3]))
			+ k_t_C0*l_F_B*((1.0/30.0)*M_PI*r_C0*u[4]/((l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0])) - 1)*sin(u[0])
			+ k_t_C0*l_F_C0*((1.0/30.0)*M_PI*r_C0*u[4]/((l_F_B*x[2] + x[1])*sin(u[0]) + (l_F_C0*x[2] + x[0])*cos(u[0])) - 1)*cos(u[0])
			+ k_t_C1*l_F_B*((1.0/30.0)*M_PI*r_C1*u[5]/((l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1])) - 1)*sin(u[1])
			- k_t_C1*l_F_C1*((1.0/30.0)*M_PI*r_C1*u[5]/((l_F_B*x[2] + x[1])*sin(u[1]) + (-l_F_C1*x[2] + x[0])*cos(u[1])) - 1)*cos(u[1])
			- k_t_C2*l_R_B*((1.0/30.0)*M_PI*r_C2*u[6]/((-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2])) - 1)*sin(u[2])
			+ k_t_C2*l_R_C2*((1.0/30.0)*M_PI*r_C2*u[6]/((-l_R_B*x[2] + x[1])*sin(u[2]) + (l_R_C2*x[2] + x[0])*cos(u[2])) - 1)*cos(u[2])
			- k_t_C3*l_R_B*((1.0/30.0)*M_PI*r_C3*u[7]/((-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3])) - 1)*sin(u[3])
			- k_t_C3*l_R_C3*((1.0/30.0)*M_PI*r_C3*u[7]/((-l_R_B*x[2] + x[1])*sin(u[3]) + (-l_R_C3*x[2] + x[0])*cos(u[3])) - 1)*cos(u[3]))/I_zz;

	xdot[3] = x[0]*cos(x[5]) - x[1]*sin(x[5]);

	xdot[4] = x[0]*sin(x[5]) + x[1]*cos(x[5]);

	xdot[5] = x[2];
}

#ifdef __cplusplus
}
#endif