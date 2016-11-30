#pragma once


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
#include "LambertMaterial.h"

// 平行光源
void directionalLightTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(1, 0, 0), 0),
		make_shared<Plane>(Vector3D(0, 1, 0), 0),
		make_shared<Plane>(Vector3D(0, 0, 1), 0),
		make_shared<Sphere>(Vector3D(20, 20, 10), 10),
	});

	int w = 800;
	int h = 600;

	Matrix<uint8> mat = Render::renderLight(geometries,
											vector<shared_ptr<Light>>(1, make_shared<DirectionalLight>(Color::WHITE, Vector3D(-1.5, -1.75, -2))),
											PerspectiveCamera(Vector3D(40, 20, 10), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 90, 1.*w / h),
											Size(h, w, 3));

	PXMImage::save(mat, "E:\\render.ppm");
}

// 点光源
void pointLightTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(0, 1, 0), 0),
		make_shared<Plane>(Vector3D(0, 0, 1), -50),
		make_shared<Plane>(Vector3D(1, 0, 0), -20),
		make_shared<Sphere>(Vector3D(0, 10, -10), 10),
	});


	int w = 800;
	int h = 600;

	Matrix<uint8> mat = Render::renderLight(geometries,
											vector<shared_ptr<Light>>(1, make_shared<PointLight>(Color::WHITE * 2000, Vector3D(30, 40, 20))),
											PerspectiveCamera(Vector3D(0, 10, 10), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90, 1.*w / h),
											Size(h, w, 3));

	PXMImage::save(mat, "E:\\render.ppm");
}


// 聚光灯
void spotLightTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(0, 1, 0), 0),
		make_shared<Plane>(Vector3D(0, 0, 1), -50),
		make_shared<Plane>(Vector3D(1, 0, 0), -20),
		make_shared<Sphere>(Vector3D(0, 10, -10), 10),
	});


	int w = 800;
	int h = 600;

	Matrix<uint8> mat = Render::renderLight(geometries,
											vector<shared_ptr<Light>>(1, make_shared<SpotLight>(Color::WHITE * 2000, Vector3D(30, 40, 20), Vector3D(-1, -1, -1), 20, 30, 0.5)),
											PerspectiveCamera(Vector3D(0, 10, 10), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90, 1.*w / h),
											Size(h, w, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}


// RGB圆盘
void renderRGBTest() {
	UnionGeometry geometries({
		make_shared<Plane>(Vector3D(0, 0, 1), 0),
		make_shared<Plane>(Vector3D(0, 1, 0), -20),
		make_shared<Plane>(Vector3D(1, 0, 0), -20),
	});

	vector<shared_ptr<Light>> lights{
		make_shared<PointLight>(Color::WHITE * 1000, Vector3D(0, 0, 60)),
		make_shared<SpotLight>(Color::RED * 3000, Vector3D(6, 0, 30), Vector3D(0, 0, -1), 20, 30, 1),
		make_shared<SpotLight>(Color::GREEN * 3000, Vector3D(-3, 3 * 1.732, 30), Vector3D(0, 0, -1), 20, 30, 1),
		make_shared<SpotLight>(Color::BLUE * 3000, Vector3D(-3, -3 * 1.732, 30), Vector3D(0, 0, -1), 20, 30, 1),

	};


	int w = 800;
	int h = 600;

	Matrix<uint8> mat =
		Render::renderLight(geometries,
		lights,
		PerspectiveCamera(Vector3D(25, 25, 25), Vector3D(-1, -1, -1), Vector3D(0, 0, 1), 60),
		Size(h, w, 3));

	PXMImage::save(mat, "E:\\render.ppm");
}

// 多光源
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


	int w = 800;
	int h = 600;

	Matrix<uint8> mat =
		Render::renderLight(geometries,
		lights,
		PerspectiveCamera(Vector3D(0, 10, 10), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90, 1.*w / h),
		Size(h, w, 3));

	PXMImage::save(mat, "E:\\render.ppm");
}
