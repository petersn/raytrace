// An example program that uses this ray tracer.

using namespace std;
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include "tracer.h"
#include "geometry.h"
#include "lighting.h"

int main(int argc, char** argv) {
	// Make a world with one sphere in it.
	World* w = new World();
//	w->clear_color = Color(0.1, 0.2, 0.7);

	Sphere* sphere1 = new Sphere(Vec(1,1,3), 0.4);
	sphere1->material_models.push_back(new LambertianScatter(0.5));
	sphere1->material_models.push_back(new PhongHighlight(0.5));
	w->thingums.push_back(sphere1);

	Sphere* sphere2 = new Sphere(Vec(-1,1,3), 0.8);
	sphere2->material_models.push_back(new LambertianScatter(0.5));
	sphere2->material_models.push_back(new PhongHighlight(0.5));
	w->thingums.push_back(sphere2);

	Sphere* sphere3 = new Sphere(Vec(0,-1,5), 1.7);
	sphere3->material_models.push_back(new LambertianScatter(0.5));
	sphere3->material_models.push_back(new PhongHighlight(0.5));
	w->thingums.push_back(sphere3);

	Plane* plane1 = new Plane(Vec(0,2,0), Vec(0, -1, 0));
	plane1->material_models.push_back(new LambertianScatter(0.5));
	plane1->material_models.push_back(new PhongHighlight(0.5));
	w->thingums.push_back(plane1);

	// Spotlight from the right to test occlusion.
	OmnidirectionalLight* light1 = new OmnidirectionalLight(Vec(1, -1, -2), Vec(0, 10, 0));
	w->lights.push_back(light1);

	// Lighting down from above.
	OmnidirectionalLight* light2 = new OmnidirectionalLight(Vec(0, -10, 2), Vec(20, 20, 40));
	w->lights.push_back(light2);

	cout << "=== Ray tracing ===" << endl;
	Canvas* canv = new Canvas(640, 480);
	w->render(canv);
	canv->save("output.png");

/*
	// Generate a canvas
	for (int i = 0; i < 100; i++) {
		light1->position(2) = 5 - i / 20.0;
		Canvas* canv = new Canvas(640, 480);
		w->render(canv);
		ostringstream os;
		os << "frames/img" << std::setw(3) << std::setfill('0') << i << ".png";
		canv->save(os.str());
		delete canv;
	}
*/

	// Clean up.
	delete w;
	return 0;
}

