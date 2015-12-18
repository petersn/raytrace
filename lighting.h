// Lighting.

#ifndef _LIGHTING_H
#define _LIGHTING_H

#include <string>
#include <stdint.h>
#include "tracer.h"

class Light {
public:
	Vec position;
	virtual Color compute_contribution(Vec p) const = 0;
};

class OmnidirectionalLight : public Light {
public:
	Color color;

	OmnidirectionalLight(Vec position, Color color);
	Color compute_contribution(Vec p) const;
};

class MaterialModel {
public:
	Real amplitude;

	virtual Color modulate_contribution(Light* light, Color color, const RayCollision& hit) const = 0;
};

class LambertianScatter : public MaterialModel {
public:
	LambertianScatter(Real amplitude);
	Color modulate_contribution(Light* light, Color color, const RayCollision& hit) const;
};

class PhongHighlight : public MaterialModel {
public:
	PhongHighlight(Real amplitude);
	Color modulate_contribution(Light* light, Color color, const RayCollision& hit) const;
};

class Canvas {
public:
	int width, height, size;
	Real gain;
	Color* pixels;
	Real* depth_buffer;

	Canvas(int width, int height);
	~Canvas();
	void zero();
	Color* pixel_ptr(int x, int y);
	Real* depth_ptr(int x, int y);
	void get_pixel(int x, int y, uint8_t* dest);
	int save(std::string path);
};

void apply_depth_of_field_effect(Canvas* source, Canvas* dest, Real pof_depth, Real dispersion_factor);

#endif

