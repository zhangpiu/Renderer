#include <iostream>

#include "LightTest.h"
#include "LocalIlluminationTest.h"
#include "GlobalIllumination.h"


int main(int argc, char *argv[]){
	string filename = "Render.ppm";
	int samples = 10, w = 800, h = 600;
	Size size(h, w, 3);

	if (argc > 1) samples = atoi(argv[1]);

	if (argc > 3) {
		w = atoi(argv[2]);
		h = atoi(argv[3]);
		size = Size(h, w, 3);
	}

	if (argc > 4) filename = argv[4];

	auto mat = renderICM(size, samples);

	//globalIlluminationTest();
	//planeAndSphereTest();

	//smallpt();

	//animationTest();

	//directionalLightTest();
	//pointLightTest();
	//spotLightTest();
	//renderRGBTest();
	//render36LightsTest();


	PXMImage::save(mat, filename);

	return 0;
}