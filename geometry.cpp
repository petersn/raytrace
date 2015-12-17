// Geometries.

#include "geometry.h"

Sphere::Sphere(Vec center, Real radius) : center(center), radius(radius) {
}

bool Sphere::cast_test(const Ray& ray, RayCollision* hit) {
	// Computed according to: https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
	Vec offset = center - ray.origin;
	Real base_d = ray.direction.dot(offset);
	Real discriminant = square(base_d) - square(offset.norm()) + square(radius);
	if (discriminant < 0)
		return false;
	Real root = real_sqrt(discriminant);
	// Now we compute the closest positive-direction intersection INTO the sphere.
	// NOTE: Intersections going out of the sphere are ignored!
	if (base_d - root < 0)
		return false;
	// Store the hit if the pointer passed was non-NULL.
	if (hit != nullptr) {
		hit->hit = ray.origin + ray.direction * (base_d - root);
		hit->incoming = ray.direction;
		Vec normal = hit->normal = (hit->hit - center).normalized();
		hit->reflection = (ray.direction - 2 * normal * normal.dot(ray.direction)).normalized();
		hit->thingum = this;
	}
	return true;
}

