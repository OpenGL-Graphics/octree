#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "octree.hpp"
#include "stopwatch.hpp"

// Used for testing
std::vector<glm::vec3> points;
Octree octree;
glm::vec3 qmin, qmax;

float rand11() {
  // Random number between [-1,1]
  return -1.f + (2.f*rand()) * (1.f / (float) RAND_MAX);
}

glm::vec3 randVec3() {
  // Random vector with components in the range [-1,1]
  return glm::vec3(rand11(), rand11(), rand11());
}

// Determine if 'point' is within the bounding box [bmin, bmax]
bool naivePointInBox(const glm::vec3& point, const glm::vec3& bmin, const glm::vec3& bmax) {
	return 
		point.x >= bmin.x &&
		point.y >= bmin.y &&
		point.z >= bmin.z &&
		point.x <= bmax.x &&
		point.y <= bmax.y &&
		point.z <= bmax.z;
}

void init() {
	// Create a new Octree centered at the origin
	// with physical dimension 2x2x2
	octree = Octree(glm::vec3(0,0,0), glm::vec3(1,1,1));

	// Create a bunch of random points
	const int nPoints = 1 * 1000 * 1000;
	for(int i=0; i<nPoints; ++i) {
		points.push_back(randVec3());
	}
	printf("Created %ld points\n", points.size()); fflush(stdout);

	// Insert the points into the octree
	for(int i=0; i<nPoints; ++i) {
		octree.insert(points[i]);
	}
	printf("Inserted points to octree\n"); fflush(stdout);

	// Create a very small query box. The smaller this box is
	// the less work the octree will need to do. This may seem
	// like it is exagerating the benefits, but often, we only
	// need to know very nearby objects.
	qmin = glm::vec3(-.05,-.05,-.05);
	qmax = glm::vec3(.05,.05,.05);

	// Remember: In the case where the query is relatively close
	// to the size of the whole octree space, the octree will
	// actually be a good bit slower than brute forcing every point!
}

// Query using brute-force
void testNaive() {
	double start = stopwatch();

	std::vector<int> results;
	for(int i=0; i<points.size(); ++i) {
		if(naivePointInBox(points[i], qmin, qmax)) {
			results.push_back(i);
		}
	}

	double T = stopwatch() - start;
	printf("testNaive found %ld points in %.5f sec.\n", results.size(), T);
}

// Query using Octree
void testOctree() {
	double start = stopwatch();

	std::vector<glm::vec3> results;
	octree.getPointsInsideBox(qmin, qmax, results);

	double T = stopwatch() - start;
	printf("testOctree found %ld points in %.5f sec.\n", results.size(), T);
}


int main(int argc, char **argv) {
	init();
	testNaive();
	testOctree();

	return 0;
}
