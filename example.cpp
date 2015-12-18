// An example program that uses this ray tracer.

using namespace std;
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdlib.h>

#include "tracer.h"
#include "geometry.h"
#include "lighting.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <offset> <step>" << endl;
	}

	int offset = atoi(argv[1]);
	int step = atoi(argv[2]);
	cout << "Running with offset=" << offset << " step=" << step << endl;

	// Make a world with one sphere in it.
	World* w = new World();
	w->clear_color = Color(0.02, 0.08, 0.2) * 0.0;

	Sphere* sphere1 = new Sphere(Vec(1,1,3), 0.4);
	sphere1->material_models.push_back(new LambertianScatter(0.5));
	sphere1->material_models.push_back(new PhongHighlight(1.5));
	w->thingums.push_back(sphere1);

	Sphere* sphere2 = new Sphere(Vec(-1,1,3), 0.8);
	sphere2->material_models.push_back(new LambertianScatter(0.5));
	sphere2->material_models.push_back(new PhongHighlight(1.5));
	w->thingums.push_back(sphere2);

	Sphere* sphere3 = new Sphere(Vec(0,-1,5), 1.7);
	sphere3->material_models.push_back(new LambertianScatter(0.5));
	sphere3->material_models.push_back(new PhongHighlight(1.5));
	w->thingums.push_back(sphere3);

/*
	Sphere* sphere4 = new Sphere(Vec(-1,-2,-4), 4.0);
	sphere4->material_models.push_back(new LambertianScatter(0.5));
	sphere4->material_models.push_back(new PhongHighlight(0.5));
	w->thingums.push_back(sphere4);
*/

	Plane* plane1 = new Plane(Vec(0,2,0), Vec(0, -1, 0));
	plane1->material_models.push_back(new LambertianScatter(0.5));
	plane1->material_models.push_back(new PhongHighlight(1.5));
	w->thingums.push_back(plane1);

	// Spotlight from the right to test occlusion.
	OmnidirectionalLight* light1 = new OmnidirectionalLight(Vec(1, -1, -2), Vec(0, 10, 0));
	w->lights.push_back(light1);

	// Lighting down from above.
	OmnidirectionalLight* light2 = new OmnidirectionalLight(Vec(0, -10, 2), Vec(20, 20, 40));
	w->lights.push_back(light2);

	// Spotlight from the right to test occlusion.
	OmnidirectionalLight* light3 = new OmnidirectionalLight(Vec(-4, -3, 3), Vec(10, 0, 0));
	w->lights.push_back(light3);

	cout << "=== Ray tracing ===" << endl;
	Canvas* canv = new Canvas(1920 * 1, 1080 * 1);//1366, 768);
	w->render(canv);
	canv->save("output.png");
	return 0;

//*
	// Generate a canvas
	//for (int i = offset; i < 200; i += step) {
//	#pragma omp parallel for
	for (int i = 0; i < 200; i++) {
//		light1->position(1) = 9 - i / 20.0;
//		light1->position(2) = 2 - i / 40.0;
//		light3->position(0) = -4 + i / 40.0;
		sphere1->center(0) -= 0.005;
		sphere2->center(2) += 0.01;
		sphere3->center(1) -= 0.01;
		if ((step + i - offset) % step == 0) {
			w->render(canv);
			ostringstream os;
			os << "frames/img" << std::setw(3) << std::setfill('0') << i << ".png";
			canv->save(os.str());
			cout << "\rSaved: " << os.str();
			cout.flush();
		}
	}
//*/

	// Clean up.
	delete w;
	delete canv;
	return 0;
}

