// Simple ray tracer.

#include "tracer.h"
#include "lighting.h"

Ray::Ray() {
	origin = Vec(0, 0, 0);
	direction = Vec(0, 0, 1);
}

Ray::Ray(Vec _origin, Vec _direction) : origin(_origin), direction(_direction) {
	// We must make sure this vector is normalized at all times!
	direction.normalize();
}

Real Ray::distance_along_ray(Vec p) const {
	return direction.dot(p - origin);
}

World::World() {
	camera = Ray(Vec(0, 0, 0), Vec(0, 0, 1));
	clear_color = Color(0, 0, 0);
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

Color World::illuminate_point(RayCollision& hit, int recursions) {
	Color result(0, 0, 0);
	RayCollision temp_hit;
	// Iterate through all the lights, finding all the lights that affect this point.
	for (unsigned int i = 0; i < lights.size(); i++) {
		Light& light = *lights[i];
		Real distance_to_light = (light.position - hit.hit).norm();
		// Cast a ray towards the light.
		Ray light_to_point(light.position, hit.hit - light.position);
//		Ray light_to_point(p, light.position - p);
		bool success = cast_test(light_to_point, &temp_hit);
		// If the colliding distance is less than the distance to the light, then this light is occluded.
		if (success and (light.position - temp_hit.hit).norm() < distance_to_light - EPSILON)
			continue;
		// If not occluded, then we can compute its contribution.
		Color illuminated_color = light.compute_contribution(hit.hit);
		std::vector<MaterialModel*>& material_models = hit.thingum->material_models;
		for (unsigned int j = 0; j < material_models.size(); j++) {
			MaterialModel& mm = *material_models[j];
			result += mm.amplitude * mm.modulate_contribution(&light, illuminated_color, hit);
		}
	}
	// Test if we should recursively color based on an ideal reflection
	if (recursions > 0) {
		Ray bounce(hit.hit, hit.reflection);
		Color recursive_color = color_ray(bounce, recursions-1, nullptr);
		result += recursive_color * 1.0;
	}
	// Also recurse Lambertianly.
	/*
	if (recursions > 0) {
		for (int i = 0; i < 50; i++) {
			Vec random_direction = hit.normal + (Vec::Random() * 0.8);
			Ray bounce(hit.hit, random_direction);
			Color recursive_color = color_ray(bounce, recursions-1, nullptr);
			result += recursive_color / 20.0;
		}
	}
	//*/
	return result;
}

Color World::color_ray(const Ray& ray, int recursions, Real* depth_information) {
	RayCollision hit;
	bool success = cast_test(ray, &hit);
	if (success) {
		if (depth_information != nullptr)
			*depth_information = (hit.hit - ray.origin).norm();
		return illuminate_point(hit, recursions);
//		Real amp = 1.0 / square((hit.hit - camera.origin).norm());
//		return Vec(amp, amp, amp);
	} else {
		if (depth_information != nullptr)
			*depth_information = INFINITY;
		return clear_color;
	}
}

void World::render(Canvas* canv, int bundle_width, Real dispersion, Real pof_depth) {
	Real aspect_ratio = canv->width / (Real)canv->height;
	Real scale = 1.1;
	Real normalization = 1.0 / square(2 * bundle_width + 1);

	#pragma omp parallel for
	for (int x = 0; x < canv->width; x++) {
		for (int y = 0; y < canv->height; y++) {
			Ray test_ray = camera;
			Color& pixel = *canv->pixel_ptr(x, y);
			for (int bundle_x = -bundle_width; bundle_x < bundle_width+1; bundle_x++) {
				for (int bundle_y = -bundle_width; bundle_y < bundle_width+1; bundle_y++) {
					Real x_dev = x / (Real)canv->width;
					Real y_dev = y / (Real)canv->height;

					Real dispersion_x = (dispersion * bundle_x) / (Real) (2*bundle_width+1);
					Real dispersion_y = (dispersion * bundle_y) / (Real) (2*bundle_width+1);

					test_ray.origin(0) = camera.origin(0) + dispersion_x;
					test_ray.origin(1) = camera.origin(1) + dispersion_y;

					test_ray.direction(0) = (x_dev - 0.5) * aspect_ratio * scale - dispersion_x / pof_depth;
					test_ray.direction(1) = (y_dev - 0.5) * scale - dispersion_y / pof_depth;
					test_ray.direction.normalize();
					pixel += color_ray(test_ray, 4, canv->depth_ptr(x, y)) * normalization;
				}
			}
		}
	}
}

