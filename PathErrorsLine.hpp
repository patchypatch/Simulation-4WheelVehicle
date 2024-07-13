class PathErrorsLine {
	public:
	/* public data */

	/* public member functions */
	// constructor
	PathErrorsLine(float xf, float yf);

	void getErrors(float* errors, const float* position, const float orientation);

	private:
	/* private data */
	float endPoint[2];
	float pathAngle;
	float rho;

	/* private member functions */
};