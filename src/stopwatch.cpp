#include "stopwatch.hpp"

double stopwatch() {
	struct timeval time;
	gettimeofday(&time, 0 );
	return 1.0 * time.tv_sec + time.tv_usec / (double)1e6;
}
