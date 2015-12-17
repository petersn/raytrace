// Sphere object.

#ifndef _SPHERE_H
#define _SPHERE_H

#include "tracer.h"

class Sphere : Thingum {
	Vec center;
	Real radius;

	Sphere(Vec center, Real radius);
	bool cast_test(const Ray& ray, RayCollision* hit);
};

#endif

