#include "PathErrorsCircle.hpp"
#include "simulation.h"

/* Constructor */
PathErrorsCircle::PathErrorsCircle(float r) : radius{r} {
	
}

// position = {X, Y}
// errors = {lateral, heading}
void PathErrorsCircle::getErrors(float* errors, const float* position, const float orientation) {
	// Find circleAngle and determine direction of travel
	float circleAngle = atan2(-position[0], position[1]);
	float circleNormalVec[] = {cos(circleAngle), sin(circleAngle)};
	float vehicleNormalVec[] = {cos(orientation), sin(orientation)};
	// Compute lateral error
	//printf("vehicleVec = %.3f, %.3f\n", vehicleVec[0], vehicleVec[1]);
	errors[0] = cblas_snrm2(2, position, INCX_DEFAULT) - radius;
	// If vehicle is facing opposite direction of circleNormalVec, flip circleAngle and lateral error
	if( cblas_sdot(2, circleNormalVec, INCX_DEFAULT, vehicleNormalVec, INCY_DEFAULT) < 0 ) {
		circleAngle = atan2(position[0], -position[1]);
		errors[0] = -errors[0];
	}
	// Compute heading error
	errors[1] = orientation - circleAngle;
}