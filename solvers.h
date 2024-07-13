#ifdef __cplusplus
extern "C" {
#endif

void forward_euler_step(double* x, double* xdot, void(*pf)(double*,double*,double*), double* u);
void rk4_step(double* x, double* xdot, void(*pf)(double*,double*,double*), double* u);

#ifdef __cplusplus
}
#endif