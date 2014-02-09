#include "simlib.h"
#include "global.hpp"

double NormalMinMax(double min, double max)
{
	double mi = max-min;
	double sigma = (max-mi)/6;
	return Normal(mi, sigma);
}

// EOF
