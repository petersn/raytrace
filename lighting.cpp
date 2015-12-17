// Lighting.

#include <png.h>
#include "lighting.h"

OmnidirectionalLight::OmnidirectionalLight(Vec _position, Color color) : color(color) {
	this->position = _position;
}

Color OmnidirectionalLight::compute_contribution(Vec p) const {
	Real distance = (p - position).norm();
	return color / square(distance);
}

LambertianScatter::LambertianScatter(Real _amplitude) {
	this->amplitude = _amplitude;
}

Color LambertianScatter::modulate_contribution(Light* light, Color color, const RayCollision& hit) const {
	Vec to_light = (light->position - hit.hit).normalized();
	return -hit.incoming.dot(to_light) * color;
}

PhongHighlight::PhongHighlight(Real _amplitude) {
	this->amplitude = _amplitude;
}

Color PhongHighlight::modulate_contribution(Light* light, Color color, const RayCollision& hit) const {
	Vec to_light = (light->position - hit.hit).normalized();
	Real value = -hit.incoming.dot(to_light);
	return square(square(value)) * color;
}

Canvas::Canvas(int _width, int _height) : width(_width), height(_height), gain(255.0) {
	size = width * height;
	pixels = new Color[size];
}

Canvas::~Canvas() {
	delete[] pixels;
}

Color* Canvas::pixel_ptr(int x, int y) {
	return pixels + (x + y * width);
}

void Canvas::get_pixel(int x, int y, uint8_t* dest) {
	Color c = pixels[x + y * width];
	for (int i = 0; i < 3; i++)
		dest[i] = (uint8_t)max(0.0, min(255.0, (Real)(c(i) * gain)));
}

// This function basically entirely based on: http://www.lemoda.net/c/write-png/
int Canvas::save(std::string path) {
	FILE* fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	int x, y;
	png_byte** row_pointers = NULL;
	// "status" contains the return value of this function. At first
	// it is set to a value which means 'failure'. When the routine
	// has finished its work, it is set to a value which means
	// 'success'.
	int status = -1;
	// The following number is set by trial and error only. I cannot
	// see where it it is documented in the libpng manual.
	int pixel_size = 3;
	int depth = 8;

	fp = fopen(path.c_str(), "wb");
	if (!fp)
		goto fopen_failed;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
		goto png_create_write_struct_failed;

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
		goto png_create_info_struct_failed;

	// Set up error handling.
	if (setjmp(png_jmpbuf(png_ptr)))
		goto png_failure;

	// Set image attributes.
	png_set_IHDR(png_ptr,
	             info_ptr,
	             width,
	             height,
	             depth,
	             PNG_COLOR_TYPE_RGB,
	             PNG_INTERLACE_NONE,
	             PNG_COMPRESSION_TYPE_DEFAULT,
	             PNG_FILTER_TYPE_DEFAULT);

	// Initialize rows of PNG.
	row_pointers = (png_byte**)png_malloc(png_ptr, height * sizeof(png_byte*));
	for (y = 0; y < height; y++) {
		png_byte *row = (png_byte*)png_malloc(png_ptr, sizeof(uint8_t) * width * pixel_size);
		row_pointers[y] = row;
		for (x = 0; x < width; x++) {
			// Here's where we extract the all important color info.
			get_pixel(x, y, row);
			row += 3;
		}
	}

	// Write the image data to "fp".
	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	// The routine has successfully written the file, so we set
	// "status" to a value which indicates success.

	status = 0;

	for (y = 0; y < height; y++)
		png_free(png_ptr, row_pointers[y]);
	png_free(png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
	png_destroy_write_struct(&png_ptr, &info_ptr);
png_create_write_struct_failed:
	fclose(fp);
fopen_failed:
	return status;
}

