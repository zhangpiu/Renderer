#include <iostream>

#include "PXMImage.h"
#include "IP.h"
#include "Ray3D.h"
#include "Sphere.h"
#include "PerspectiveCamera .h"
#include "Render.h"
#include "PhongMaterial.h"
#include "Plane.h"
#include "CheckerMaterial .h"
#include "UnionGeometry.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

void directionalLightTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(1, 0, 0), 0),
		make_shared<Plane>(Vector3D(0, 1, 0), 0),
		make_shared<Plane>(Vector3D(0, 0, 1), 0),
		make_shared<Sphere>(Vector3D(20, 20, 10), 10),
	});


	int s = 512;
	Matrix<uint8> mat = Render::renderLight(geometries,
											vector<shared_ptr<Light>>(1, make_shared<DirectionalLight>(Color::WHITE, Vector3D(-1.5, -1.75, -2))),
											PerspectiveCamera(Vector3D(40, 20, 10), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 90),
											Size(s, s, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}

void pointLightTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(0, 1, 0), 0),
		make_shared<Plane>(Vector3D(0, 0, 1), -50),
		make_shared<Plane>(Vector3D(1, 0, 0), -20),
		make_shared<Sphere>(Vector3D(0, 10, -10), 10),
	});


	int s = 512;
	Matrix<uint8> mat = Render::renderLight(geometries,
											vector<shared_ptr<Light>>(1, make_shared<PointLight>(Color::WHITE*2000, Vector3D(30, 40, 20))),
											PerspectiveCamera(Vector3D(0, 10, 10), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90),
											Size(s, s, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}


void spotLightTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(0, 1, 0), 0),
		make_shared<Plane>(Vector3D(0, 0, 1), -50),
		make_shared<Plane>(Vector3D(1, 0, 0), -20),
		make_shared<Sphere>(Vector3D(0, 10, -10), 10),
	});


	int s = 512;
	Matrix<uint8> mat = Render::renderLight(geometries,
											vector<shared_ptr<Light>>(1, make_shared<SpotLight>(Color::WHITE * 2000, Vector3D(30, 40, 20), Vector3D(-1,-1,-1), 20, 30, 0.5)),
											PerspectiveCamera(Vector3D(0, 10, 10), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90),
											Size(s, s, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}

void reflectionTest() {
	// Two spheres
	auto plane = make_shared<Plane>(Vector3D(0,0,1), 0);
	auto sphere1 = make_shared<Sphere>(Vector3D(-10, -10, 10), 10);
	auto sphere2 = make_shared<Sphere>(Vector3D(-10, 10, 10), 10);
	auto sphere3 = make_shared<Sphere>(Vector3D(-1e8-20, 0, 0), 1e8);
	  
	plane->setMaterial(make_shared<CheckerMaterial>(0.1, 0.5));
	sphere1->setMaterial(make_shared<PhongMaterial>(Color(1,0,0), Color::WHITE, 10, 0.25));
	sphere2->setMaterial(make_shared<PhongMaterial>(Color::BLUE, Color::WHITE, 16, 0.25));
	sphere3->setMaterial(make_shared<PhongMaterial>(Color::GREEN, Color(0.75, 0.75, 0.75), 16, 0));
	  
	UnionGeometry geometries({sphere3, sphere1, sphere2, plane});
	
	vector<shared_ptr<Light>> lights{ make_shared<SpotLight>(Color::WHITE*10000, Vector3D(20,0, 80), Vector3D(-1, 0, -1), 80, 90, 1) };
	
	int s = 512;
	 
	Matrix<uint8> mat = Render::rayTraceReflection(geometries, 
										lights,
	 									PerspectiveCamera(Vector3D(15,0,5), Vector3D(-1,0,0), Vector3D(0,0,1), 90),
	 									5,
	 									Size(s, s, 3));
	 
	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}


void planeTest() {
	// Two spheres
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1), 0);
	auto plane2 = make_shared<Plane>(Vector3D(0, 1, 0), 0);
	auto plane3 = make_shared<Plane>(Vector3D(1, 0, 0), 0);
	
	auto sphere1 = make_shared<Sphere>(Vector3D(10, 10, 10), 10);
	//auto sphere2 = make_shared<Sphere>(Vector3D(10, 35, 10), 10);

	plane1->setMaterial(make_shared<PhongMaterial>(Color(0.75,0.25,0.25), Color::WHITE, 10, 0.2));
	plane2->setMaterial(make_shared<PhongMaterial>(Color(0.5,0.5,0.75), Color::WHITE, 10, 0));
	plane3->setMaterial(make_shared<PhongMaterial>(Color(0.5,0.5,0.5), Color::WHITE, 10, 0));

	sphere1->setMaterial(make_shared<PhongMaterial>(Color(1, 0, 0), Color::WHITE, 10, 1));
	//sphere2->setMaterial(make_shared<PhongMaterial>(Color::BLUE, Color::WHITE, 16, 0.25));

	UnionGeometry geometries({ plane1, plane2, plane3, sphere1 });

	vector<shared_ptr<Light>> lights{ make_shared<DirectionalLight>(Color::WHITE, Vector3D(-1, -1, -1)) };

	int s = 512;

	Matrix<uint8> mat = Render::rayTraceReflection(geometries,
												   lights,
												   PerspectiveCamera(Vector3D(40, 20, 15), Vector3D(-1, -0.1, 0), Vector3D(0, 0, 1), 90),
												   5,
												   Size(s, s, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}


void renderRGBTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(0, 0, 1), 0),
		make_shared<Plane>(Vector3D(0, 1, 0), -20),
		make_shared<Plane>(Vector3D(1, 0, 0), -20),
	});

	vector<shared_ptr<Light>> lights{
		make_shared<PointLight>(Color::WHITE*1000, Vector3D(0,0,60)),
		make_shared<SpotLight>(Color::RED * 3000, Vector3D(6, 0, 30), Vector3D(0, 0, -1), 20, 30, 1),
		make_shared<SpotLight>(Color::GREEN * 3000, Vector3D(-3, 3*1.732, 30), Vector3D(0, 0, -1), 20, 30, 1),
		make_shared<SpotLight>(Color::BLUE * 3000, Vector3D(-3, -3*1.732, 30), Vector3D(0, 0, -1), 20, 30, 1),

	};


	int s = 512;
	Matrix<uint8> mat = 
		Render::renderLight(geometries,
							lights,
							PerspectiveCamera(Vector3D(25, 25, 25), Vector3D(-1, -1, -1), Vector3D(0, 0, 1), 60),
							Size(s, s, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}


void render36LightsTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(0, 1, 0), 0),
		make_shared<Plane>(Vector3D(0, 0, 1), -50),
		make_shared<Plane>(Vector3D(1, 0, 0), -20),
		make_shared<Sphere>(Vector3D(0, 10, -10), 10)
	});

	vector<shared_ptr<Light>> lights;

	for (int x = 10; x <= 30; x += 4) {
		for (int z = 20; z <= 40; z += 4) {
			lights.push_back(make_shared<PointLight>(Color::WHITE * 80, Vector3D(x, 50, z)));
		}
	}

	auto fillLight = make_shared<DirectionalLight>(Color::WHITE*0.25, Vector3D(1.5, 1, 0.5));
	lights.push_back(fillLight);


	int s = 512;
	Matrix<uint8> mat =
		Render::renderLight(geometries,
		lights,
		PerspectiveCamera(Vector3D(0, 10, 10), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90),
		Size(s, s, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}

int main(){

	planeTest();


	return 0;
}