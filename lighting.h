// Lighting.

#ifndef _LIGHTING_H
#define _LIGHTING_H

#include <string>
#include "tracer.h"

typedef Vec Color;

class Light {
public:
	Color color;
};

class Canvas {
public:
	int width, height, size;
	Color* pixels;

	Canvas(int width, int height);
	~Canvas();
	void save(std::string path);
};

#endif

