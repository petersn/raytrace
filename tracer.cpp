// Simple ray tracer.

#include "tracer.h"

Ray::Ray(Vec _origin, Vec _direction) : origin(_origin), direction(_direction) {
	// We must make sure this vector is normalized at all times!
	direction.normalize();
}

Real Ray::distance_along_ray(Vec p) const {
	return direction.dot(p - origin);
}

bool World::cast_test(const Ray& ray, RayCollision* hit) {
	RayCollision temp_hit;
	Real best_distance = INFINITY;
	bool did_hit = false;
	for (unsigned int i = 0; i < thingums.size(); i++) {
		Thingum& thingum = *thingums[i];
		// Check if a ray cast against this object provides a closer hit.
		if (thingum.cast_test(ray, &temp_hit)) {
			did_hit = true;
			Real distance = ray.distance_along_ray(temp_hit.hit);
			if (distance < best_distance) {
				// Store our best hit so far.
				if (hit != nullptr)
					*hit = temp_hit;
				best_distance = distance;
			}
		}
	}
	return did_hit;
}

void World::render(Canvas* canv) {
	// Pass for now.
}

