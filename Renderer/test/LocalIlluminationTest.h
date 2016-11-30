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

// ∏¥‘”≥°æ∞≤‚ ‘
void planeAndSphereTest() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1), 0);    // bottom
	auto plane2 = make_shared<Plane>(Vector3D(1, 0, 0), -20);  // back
	auto plane3 = make_shared<Plane>(Vector3D(0, 1, 0), -30);  // left
	auto plane4 = make_shared<Plane>(Vector3D(0, -1, 0), -30); // right
	auto plane5 = make_shared<Plane>(Vector3D(0, 0, -1), -42); // top

	// Two spheres
	auto sphere1 = make_shared<Sphere>(Vector3D(-10, -12, 10), 10);
	auto sphere2 = make_shared<Sphere>(Vector3D(-10, 12, 10), 10);

	plane1->setMaterial(make_shared<CheckerMaterial>(0.1, 0.5));
	plane2->setMaterial(make_shared<LambertMaterial>(Color(0, 0.5, 0.5)));
	plane3->setMaterial(make_shared<LambertMaterial>(Color(0.5, 0.5, 0.5)));
	plane4->setMaterial(make_shared<LambertMaterial>(Color(0, 0.2, 0.5)));
	plane5->setMaterial(make_shared<LambertMaterial>(Color(0.25, 0.75, 0.25)));


	sphere1->setMaterial(make_shared<PhongMaterial>(Color(1, 0, 0), Color::WHITE, 10, 0.25));
	sphere2->setMaterial(make_shared<PhongMaterial>(Color(0.5, 0.5, 0.5), Color::WHITE, 16, 0.25));

	UnionGeometry geometries({ plane1, plane2, plane3, plane4, plane5, sphere1, sphere2 });

	vector<shared_ptr<Light>> lights{
		make_shared<PointLight>(Color::WHITE * 800, Vector3D(20, -20, 40)),
		make_shared<PointLight>(Color::WHITE * 800, Vector3D(20, 0, 40)),
		make_shared<PointLight>(Color::WHITE * 800, Vector3D(20, 20, 40)),
	};

	int w = 800;
	int h = 600;

	Matrix<uint8> mat = Render::rayTrace(geometries,
										 lights,
										 PerspectiveCamera(Vector3D(20, 0, 20), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 90, 1.*w / h),
										 50,
										 Size(h, w, 3));

	PXMImage::save(mat, "E:\\render.ppm");
}

// ∂Øª≠≥°æ∞≤‚ ‘
void animationTest() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1), 0);      // ground
	auto plane2 = make_shared<Plane>(Vector3D(1, 0, 0), -20);    // back
	auto plane3 = make_shared<Plane>(Vector3D(0, 1, 0), -29.9);  // left
	auto plane4 = make_shared<Plane>(Vector3D(0, -1, 0), -29.9); // right
	auto plane5 = make_shared<Plane>(Vector3D(0, 0, -1), -42);   // ceil

	plane1->setMaterial(make_shared<CheckerMaterial>(0.1, 0.5));
	plane2->setMaterial(make_shared<LambertMaterial>(Color(0, 0.5, 0.5)));
	plane3->setMaterial(make_shared<LambertMaterial>(Color(0.5, 0.5, 0.5)));
	plane4->setMaterial(make_shared<LambertMaterial>(Color(0, 0.5, 0.75)));
	plane5->setMaterial(make_shared<PhongMaterial>(Color(0.75, 0.75, 0.75), Color(0.75, 0.75, 0.75), 50, 0));

	int num = 180;

	for (int i = 0; i < num; ++i) {
		const double delta = i * 2 * Math::PI / num;
		const double hh = 19.;
		const double z = hh + 10 * std::sin(delta);
		const double y = 10 * std::cos(delta);

		auto sphere2 = make_shared<Sphere>(Vector3D(-10, y, z), 8);
		auto sphere3 = make_shared<Sphere>(Vector3D(-10, -y, 2 * hh - z), 8);

		sphere2->setMaterial(make_shared<PhongMaterial>(Color::RED, Color::RED, 10, 0.3));
		sphere3->setMaterial(make_shared<PhongMaterial>(Color(0.5, 0.5, 0.5), Color(0.5, 0.5, 0.5), 10, 0.1));

		UnionGeometry geometries({ plane1, plane2, plane3, plane4, plane5, sphere2, sphere3 });


		vector<shared_ptr<Light>> lights{
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(-15, -15, 41)),
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(-5, -15, 41)),
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(5, -15, 41)),
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(15, -15, 41)),
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(-15, 15, 41)),
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(-5, 15, 41)),
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(5, 15, 41)),
			make_shared<PointLight>(Color::WHITE * 150, Vector3D(15, 15, 41)),
		};

		int w = 400;
		int h = 300;

		Matrix<uint8> mat = Render::rayTrace(geometries,
											 lights,
											 PerspectiveCamera(Vector3D(50, 0, 20), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 50, 1.*w / h),
											 50,
											 Size(h, w, 3));
		string filename = String::format("E:\\zzz\\%d.ppm", i);
		PXMImage::save(mat, filename);
	}
}