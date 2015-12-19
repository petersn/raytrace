// Simple ray tracer.

#ifndef _TRACER_H
#define _TRACER_H

// For debugging purposes, do these two!
// XXX: Remove these later!
using namespace std;
#include <iostream>

#include <vector>
#include <Eigen/Dense>

// This forward declaration is just for the reference on render.
// TODO: Evaluate if I'm okay with this...
class Canvas;
class Light;
class MaterialModel;
class Thingum;

// Define the appropriate size of float, and appropriate functions for it.
typedef double Real;
typedef Eigen::Vector3d Vec;
typedef Eigen::Matrix<Real, 3, 3> Mat;

// Some basic math.
#define real_sqrt sqrt
inline Real square(Real x) {
	return x * x;
}

#define EPSILON 1e-3

typedef Vec Color;

class Ray {
public:
	Vec origin;
	Vec direction;

	Ray();
	Ray(Vec origin, Vec direction);
	Real distance_along_ray(Vec p) const;
};

// Provides information on the collision of a ray with a solid.
class RayCollision {
public:
	Vec hit;
	Vec incoming;
	Vec normal;
	Vec reflection;
	Thingum* thingum;
};

class Thingum {
public:
	std::vector<MaterialModel*> material_models;

	virtual bool cast_test(const Ray& ray, RayCollision* hit) = 0;
};

class World {
public:
	Ray camera;
	Color clear_color;
	std::vector<Thingum*> thingums;
	std::vector<Light*> lights;

	World();
	bool cast_test(const Ray& ray, RayCollision* hit);
	Color illuminate_point(RayCollision& p, int recursions);
	Color color_ray(const Ray& ray, int recursions, Real* depth_information);
	void render(Canvas* canv, int bundle_width, Real dispersion, Real pof_depth);
};

#endif

