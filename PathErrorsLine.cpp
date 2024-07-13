#include "PathErrorsLine.hpp"
#include "simulation.h"

/* Constructor */
PathErrorsLine::PathErrorsLine(float xf, float yf) : endPoint{xf, yf}, pathAngle{atan2(yf, xf)},
	rho{1 / cblas_snrm2(2, endPoint, INCX_DEFAULT)} {
	
}

// position = {X, Y}
// errors = {lateral, heading}
void PathErrorsLine::getErrors(float* errors, const float* position, const float orientation) {
	/* Lateral Error <- ||pathVec x posVec|| / ||pathVec|| */
	errors[0] = endPoint[0]*position[1] - endPoint[1]*position[0];
	errors[0] *= rho;

	/* Heading Error */
	errors[1] = orientation - pathAngle;
}