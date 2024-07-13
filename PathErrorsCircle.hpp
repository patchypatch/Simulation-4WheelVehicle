class PathErrorsCircle {
	public:
	/* public data */

	/* public member functions */
	// constructor
	PathErrorsCircle(float r);

	void getErrors(float* errors, const float* position, const float orientation);

	private:
	/* private data */
	float radius;

	/* private member functions */
};