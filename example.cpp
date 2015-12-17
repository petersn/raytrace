// An example program that uses this ray tracer.

using namespace std;
#include <iostream>

#include "tracer.h"
#include "geometry.h"
#include "lighting.h"


int main(int argc, char** argv) {
	// Make a world with one sphere in it.
	World* w = new World();
	Sphere* sphere1 = new Sphere(Vec(0,0,3), 1);
	w->thingums.push_back(sphere1);

	// Do a test collision off the sphere.
	Ray r(Vec(0, 0, 0), Vec(0.1, 0, 1));
	RayCollision rc;
	bool success = w->cast_test(r, &rc);
	if (success) {
		cout << "Spot: " << rc.hit.transpose() << endl;
		cout << "Normal:" << rc.normal.transpose() << endl;
		cout << "Reflection:" << rc.reflection.transpose() << endl;
	} else {
		cout << "Miss." << endl;
	}

	// Generate a canvas
	Canvas* canv = new Canvas(640, 480);
	w->render(canv);
	canv->save("output.png");

	// Clean up.
	delete w;
	return 0;
}

