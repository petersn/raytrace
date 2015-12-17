// Geometries.

#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include "tracer.h"

class Sphere : public Thingum {
public:
	Vec center;
	Real radius;

	Sphere(Vec center, Real radius);
	bool cast_test(const Ray& ray, RayCollision* hit);
};

class Plane : public Thingum {
public:
	Real parameter;
	Vec normal;

	Plane(Real parameter, Vec normal);
	Plane(Vec contains, Vec normal);
	bool cast_test(const Ray& ray, RayCollision* hit);
};

#endif

