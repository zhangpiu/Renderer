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

void smallpt() {

	// Two spheres
	auto sphere1 = make_shared<Sphere>(Vector3D(1e5+1,40.8,81.6),   1e5);   //Left
	auto sphere2 = make_shared<Sphere>(Vector3D(-1e5+99,40.8,81.6), 1e5);	//Rght
	auto sphere3 = make_shared<Sphere>(Vector3D(50, 40.8, 1e5),     1e5);   //Back
	auto sphere4 = make_shared<Sphere>(Vector3D(50,40.8,-1e5+170),  1e5);  	//Frnt
	auto sphere5 = make_shared<Sphere>(Vector3D(50, 1e5, 81.6),     1e5);	//Botm
	auto sphere6 = make_shared<Sphere>(Vector3D(50,-1e5+81.6,81.6), 1e5);	//Top
	auto sphere7 = make_shared<Sphere>(Vector3D(27,16.5,47),        16.5);	//Mirr
	auto sphere8 = make_shared<Sphere>(Vector3D(73,16.5,78),        16.5);	//Glas
	auto sphere9 = make_shared<Sphere>(Vector3D(50,681.6-.27,81.6), 600);	//Lite

	sphere1->setMaterial(make_shared<IdealMaterial>(Color(.75, .25, .25), Color::BLACK, IdealType::DIFFUSE));    //Left
	sphere2->setMaterial(make_shared<IdealMaterial>(Color(.25, .25, .75), Color::BLACK, IdealType::DIFFUSE));    //Rght
	sphere3->setMaterial(make_shared<IdealMaterial>(Color(.75, .75, .75), Color::BLACK, IdealType::DIFFUSE));    //Back
	sphere4->setMaterial(make_shared<IdealMaterial>(Color::BLACK,         Color::BLACK, IdealType::DIFFUSE));    //Frnt
	sphere5->setMaterial(make_shared<IdealMaterial>(Color(.75, .75, .75), Color::BLACK, IdealType::DIFFUSE));    //Botm
	sphere6->setMaterial(make_shared<IdealMaterial>(Color(.75, .75, .75), Color::BLACK, IdealType::DIFFUSE));    //Top
	sphere7->setMaterial(make_shared<IdealMaterial>(Color::WHITE*.999,    Color::BLACK, IdealType::SPECULAR));   //Mirr
	sphere8->setMaterial(make_shared<IdealMaterial>(Color::WHITE*.999,    Color::BLACK, IdealType::REFRACTIVE)); //Glas
	sphere9->setMaterial(make_shared<IdealMaterial>(Color::BLACK,         Color::WHITE*12, IdealType::DIFFUSE)); //Lite

	UnionGeometry geometries({ sphere1, sphere2, sphere3, sphere4, sphere5, sphere6, sphere7, sphere8, sphere9});


	auto clamp = [](double x)->double {
		if (x < 0)
			return 0;
		else if (x > 1)
			return 1;
		else
			return x;
	};

	auto toInt = [&](double x) {
		return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
	};

	clock_t start = clock();

	const int w = 256;
	const int h = 256;

	const int samps = 100; // # samples

	const Ray3D cam(Vector3D(50, 52, 295.6), Vector3D(0, -0.042612, -1).norm()); // cam pos, dir
	const Vector3D cx(w * .5135 / h, 0, 0);
	const Vector3D cy = (cx.cross(cam.getDirection())).norm() * .5135;
	Color* c = new Color[w * h]();

#pragma omp parallel for schedule(dynamic, 1)       // OpenMP
	// Loop over image rows
	for (int y = 0; y < h; y++) {
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100.*y / (h - 1));
		Random rand;

		// Loop cols
		for (unsigned short x = 0; x < w; x++) {
			// 2x2 subpixel rows
			for (int sy = 0; sy < 2; sy++) {
				const int i = (h - y - 1) * w + x;

				// 2x2 subpixel cols
				for (int sx = 0; sx < 2; sx++) {
					Color r = Color::BLACK;
					for (int s = 0; s < samps; s++) {
						double r1 = 2 * rand();
						double r2 = 2 * rand();
						double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);

						Vector3D d = cx * (((sx + .5 + dx) / 2 + x) / w - .5) +
							cy * (((sy + .5 + dy) / 2 + y) / h - .5) + cam.getDirection();

						r = r + Render::pathTraceRecursive(geometries, Ray3D(cam.getOrigin() + d * 140, d.norm()), 0, rand) * (1.0 / samps);
					}
					c[i] = c[i] + Color(clamp(r.r), clamp(r.g), clamp(r.b)) * .25;
				}
			}
		}
	}

	printf("\n%f sec\n", (float)(clock() - start) / CLOCKS_PER_SEC);

	FILE *f = fopen("E:\\image.ppm", "w"); // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
	for (int i = 0; i < w * h; i++)
		fprintf(f, "%d %d %d\n", toInt(c[i].r), toInt(c[i].g), toInt(c[i].b));
	fclose(f);
	delete[] c;
}

void globalIlluminationTest() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1),    0);    // ground
	auto plane2 = make_shared<Plane>(Vector3D(1, 0, 0),  -100);  // back
	auto plane3 = make_shared<Plane>(Vector3D(0, 1, 0),  -60);  // left
	auto plane4 = make_shared<Plane>(Vector3D(0, -1, 0), -60); // right
	auto plane5 = make_shared<Plane>(Vector3D(0, 0, -1), -100); // ceil
	auto plane6 = make_shared<Plane>(Vector3D(-1, 0, 0), -20);   // front

	plane1->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane2->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane3->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.25, 0.25), Color::BLACK, IdealType::DIFFUSE));
	plane4->setMaterial(make_shared<IdealMaterial>(Color(0.25, 0.25, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane5->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane6->setMaterial(make_shared<IdealMaterial>(Color(0.50, 0.84, 0.81), Color::BLACK, IdealType::DIFFUSE));

	auto sphere1 = make_shared<Sphere>(Vector3D(-60, -27.5, 20), 20);
	auto sphere2 = make_shared<Sphere>(Vector3D(-45, 30, 20), 20);
	auto sphere3 = make_shared<Sphere>(Vector3D(-50, 0, 100 + 97), 100); // light

	sphere1->setMaterial(make_shared<IdealMaterial>(Color(1, 1, 1), Color::BLACK, IdealType::SPECULAR));
	sphere2->setMaterial(make_shared<IdealMaterial>(Color(1, 1, 1), Color::BLACK, IdealType::REFRACTIVE));
	sphere3->setMaterial(make_shared<IdealMaterial>(Color(.75, .75, .75), Color(7.5, 7.5, 7.5), IdealType::DIFFUSE));

	UnionGeometry geometries({ plane1, plane2, plane3, plane4, plane5, plane6, sphere1, sphere2, sphere3 });

	int w = 1024;
	int h = 768;
	int samps = 10;

	clock_t start = clock();

	Matrix<uint8> mat = Render::pathTrace(geometries,
										  PerspectiveCamera(Vector3D(150, 0, 50), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 37, (1.0 * w) / h),
										  samps,
										  Size(h, w, 3));

	printf("\n%f sec\n", (float)(clock() - start) / CLOCKS_PER_SEC);

	PXMImage::save(mat, "E:\\render.ppm");
}

void globalIlluminationAnimation() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1), 0);    // ground
	auto plane2 = make_shared<Plane>(Vector3D(1, 0, 0), -100);  // back
	auto plane3 = make_shared<Plane>(Vector3D(0, 1, 0), -60);  // left
	auto plane4 = make_shared<Plane>(Vector3D(0, -1, 0), -60); // right
	auto plane5 = make_shared<Plane>(Vector3D(0, 0, -1), -100); // ceil
	auto plane6 = make_shared<Plane>(Vector3D(-1, 0, 0), 0);   // front

	plane1->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane2->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane3->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.25, 0.25), Color::BLACK, IdealType::DIFFUSE));
	plane4->setMaterial(make_shared<IdealMaterial>(Color(0.25, 0.25, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane5->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane6->setMaterial(make_shared<IdealMaterial>(Color(0.50, 0.84, 0.81), Color::BLACK, IdealType::DIFFUSE));

	auto sphere1 = make_shared<Sphere>(Vector3D(-70, -30, 20), 20);
	auto sphere3 = make_shared<Sphere>(Vector3D(-50, 0, 100 + 97), 100); // light

	sphere1->setMaterial(make_shared<IdealMaterial>(Color(1, 1, 1), Color::BLACK, IdealType::SPECULAR));
	sphere3->setMaterial(make_shared<IdealMaterial>(Color(.75, .75, .75), Color(7.5, 7.5, 7.5), IdealType::DIFFUSE));

	int frame = 3;

	for (int i = 0; i < frame; ++i) {
		double radius = std::sqrt(50*50 + 80*80);
		double theta = 30 + i * 50.0 / (frame-1 + 1e-10);
		double x = -100 + radius * std::cos(theta * Math::PI / 180);
		double y = -60 + radius * std::sin(theta * Math::PI / 180);

		auto sphere2 = make_shared<Sphere>(Vector3D(x, y, 20), 20);
		sphere2->setMaterial(make_shared<IdealMaterial>(Color(1, 1, 1), Color::BLACK, IdealType::REFRACTIVE));

		UnionGeometry geometries({ plane1, plane2, plane3, plane4, plane5, plane6, sphere1, sphere2, sphere3 });

		int w = 400;
		int h = 300;
		int samps = 10;

		clock_t start = clock();

		Matrix<uint8> mat = Render::pathTrace(geometries,
											  PerspectiveCamera(Vector3D(120, 0, 50), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 40, (1.0 * w) / h),
											  samps,
											  Size(h, w, 3));

		printf("\n%f sec\n", (float)(clock() - start) / CLOCKS_PER_SEC);

		string filename = String::format("E:\\zzz\\%d.ppm", i);
		PXMImage::save(mat, filename);
	}
}