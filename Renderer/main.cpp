#include <iostream>

#include "LightTest.h"
#include "LocalIlluminationTest.h"
#include "GlobalIllumination.h"

int main(int argc, char *argv[]){
	int samples = 10;
	string fileName = "Render.ppm";

	if (argc > 1) samples = atoi(argv[1]);
	
	if (argc > 2) fileName = argv[2];

	globalIlluminationTest();
	//planeAndSphereTest();

	//smallpt();

	//animationTest();

	//directionalLightTest();
	//pointLightTest();
	//spotLightTest();
	//renderRGBTest();
	//render36LightsTest();

	return 0;
}