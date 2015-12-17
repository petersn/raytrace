// Simple ray tracer.

#ifndef _TRACER_H
#define _TRACER_H

#include <vector>
#include <Eigen/Dense>

// Define the appropriate size of float, and appropriate functions for it.
typedef double Real;
typedef Eigen::Vector3d Vec;
typedef Eigen::Matrix<Real, 3, 3> Mat;

// Some basic math.
Real square(Real x);
#define real_sqrt sqrt

class Ray {
public:
	Vec origin;
	Vec direction;
};

// Provides information on the collision of a ray with a solid.
class RayCollision {
public:
	Vec hit;
	Vec normal;
	Vec reflection;
};

class Thingum {
public:
	virtual bool cast_test(const Ray& ray, RayCollision* hit) = 0;
};

class World {
public:
	std::vector<Thingum*> thingums;
};

#endif

