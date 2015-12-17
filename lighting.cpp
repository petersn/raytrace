// Lighting.

#include "lighting.h"

Canvas::Canvas(int _width, int _height) : width(_width), height(_height) {
	size = width * height;
	pixels = new Color[size];
}

Canvas::~Canvas() {
	delete[] pixels;
}

void Canvas::save(std::string path) {
	// Pass for now.
}

