// Simple ray tracer.

#ifndef _TRACER_H
#define _TRACER_H

// For debugging purposes, do these two!
// XXX: Remove these later!
using namespace std;
#include <iostream>

#include <vector>
#include <Eigen/Dense>

// Define the appropriate size of float, and appropriate functions for it.
typedef double Real;
typedef Eigen::Vector3d Vec;
typedef Eigen::Matrix<Real, 3, 3> Mat;

// Some basic math.
#define real_sqrt sqrt
inline Real square(Real x) {
	return x * x;
}

// TODO: Evaluate if I like this pattern of inclusion...
#include "lighting.h"

class Ray {
public:
	Vec origin;
	Vec direction;

	Ray(Vec origin, Vec direction);
	Real distance_along_ray(Vec p) const;
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

	bool cast_test(const Ray& ray, RayCollision* hit);
	void render(Canvas* canv);
};

#endif

