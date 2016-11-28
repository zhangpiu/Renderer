////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Render.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Render
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Geometry.h"
#include "PerspectiveCamera .h"
#include "Matrix.h"
#include "Color.h"
#include "Math.h"
#include "Light.h"
#include "IdealMaterial.h"
#include "UnionGeometry.h"

#include <algorithm>
#include <ctime>
#include <random>

class Random {
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _dist;

public:

 	Random() 
 		: _mt(std::random_device()())
 		, _dist(0, 1.0)
 	{}
 
 	double operator()() {
 		return _dist(_mt);
 	}
};

class Render {
public:
	static Matrix<uint8> renderDepth(const Geometry& scene, const PerspectiveCamera& camera, double maxDepth, const Size& size);

	static Matrix<uint8> renderNormal(const Geometry& scene, const PerspectiveCamera& camera, const Size& size);

	static Matrix<uint8> rayTrace(const Geometry& scene, const PerspectiveCamera& camera, const Size& size);

	static Matrix<uint8> rayTraceReflection(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, int maxReflect, const Size& size);

	static Matrix<uint8> renderLight(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, const Size& size);

	static Matrix<uint8> pathTrace(const Geometry& scene, const PerspectiveCamera& camera, const Size& size);

	static Color pathTraceRecursive(const Geometry& scene, const Ray3D& ray, int depth, Random& rand);

private:
	static Color rayTraceRecursive(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const Ray3D& ray, int maxReflect);

};


Matrix<uint8> Render::renderDepth(const Geometry& scene, const PerspectiveCamera& camera, double maxDepth, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const IntersectResult result = scene.intersect(ray);

			if (result.getGeometry()) {
				uint8 depth = uint8(255 - std::min(result.getDistance()/maxDepth*255, 255.0));

				m(i,j,0) = depth;
				m(i,j,1) = depth;
				m(i,j,2) = depth;
			}
		}
	}

	return std::move(m);
}


Matrix<uint8> Render::renderNormal(const Geometry& scene, const PerspectiveCamera& camera, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const IntersectResult result = scene.intersect(ray);

			if (result.getGeometry()) {
				m(i, j, 0) = uint8((result.getNormal().x() + 1) * 128);
				m(i, j, 1) = uint8((result.getNormal().y() + 1) * 128);
				m(i, j, 2) = uint8((result.getNormal().z() + 1) * 128);
			}
		}
	}

	return std::move(m);
}


Matrix<uint8> Render::rayTrace(const Geometry& scene, const PerspectiveCamera& camera, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const IntersectResult result = scene.intersect(ray);

			if (result.getGeometry()) {
				const Color clr; //= result.getGeometry()->getMaterial()->sample(ray, result.getPosition(), result.getNormal());

				m(i, j, 0) = (uint8)Math::clip(clr.r * 255, 0.0, 255.0);
				m(i, j, 1) = (uint8)Math::clip(clr.g * 255, 0.0, 255.0);
				m(i, j, 2) = (uint8)Math::clip(clr.b * 255, 0.0, 255.0);
			}
		}
	}


	return std::move(m);
}



Color Render::rayTraceRecursive(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const Ray3D& ray, int maxReflect) {
	const auto result = scene.intersect(ray);

	if (result.getGeometry()) {
		const auto material = result.getGeometry()->getMaterial();
		const double reflectiveness = material->getReflectiveness();
		Color clr;

		for (auto& light : lights) {
			const LightSample lightSample = light->sample(scene, result.getPosition());
			clr += material->sample(ray, lightSample, result.getPosition(), result.getNormal());
		}

		clr *= 1 - reflectiveness;

		if (reflectiveness > 0 && maxReflect > 0) {
			const Vector3D& d = ray.getDirection();
			const Vector3D& n = result.getNormal();
			const Vector3D r = d - 2 * (d.dot(n)) * n;

			const Color reflectedClr = rayTraceRecursive(scene, lights, Ray3D(result.getPosition(), r), maxReflect - 1);
			clr += reflectedClr * reflectiveness;
		}

		return clr;
	}

	return Color::BLACK;
}


Matrix<uint8> Render::rayTraceReflection(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, int maxReflect, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	#pragma omp parallel for
	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const Color clr = rayTraceRecursive(scene, lights, ray, maxReflect);

			m(i, j, 0) = convert(clr.r);
			m(i, j, 1) = convert(clr.g);
			m(i, j, 2) = convert(clr.b);
		}
	}


	return std::move(m);
}



Matrix<uint8> Render::renderLight(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const auto result = scene.intersect(ray);

			if (result.getGeometry()) {
				Color clr = Color::BLACK;

				for (auto& light : lights) {
					auto lightSample = light->sample(scene, result.getPosition());

					if (&lightSample != &LightSample::zero) {
						double NdotL = result.getNormal().dot(lightSample.L());

						if (NdotL >= 0) {
							clr += lightSample.EL() * NdotL;
						}
					}
				}

				m(i, j, 0) = convert(clr.r);
				m(i, j, 1) = convert(clr.r);
				m(i, j, 2) = convert(clr.r);
			}
		}
	}


	return std::move(m);
}



Color Render::pathTraceRecursive(const Geometry& scene, const Ray3D& ray, int depth, Random& rand) {
	const auto result = scene.intersect(ray);

	// if miss, return black.
	if (result.getGeometry() == nullptr) return Color::BLACK;

	const auto material = result.getGeometry()->getMaterial();
	const Color& emission = material->getEmission();
	const Color& color = material->getColor();
	const IdealType& type = material->getIdealType();
	const Vector3D& dir = ray.getDirection();
	const int newDepth = depth + 1;
	const bool isMaxDepth = newDepth > 100;

	// Russian roulette for path termination
	const double maxC = Math::max3(color.r, color.g, color.b);
	const bool isUseRR = newDepth > 5;
	const bool isRR = isUseRR && rand() < maxC;

	if (isMaxDepth || (isUseRR && !isRR)) return emission;

	Color f = (isUseRR && isRR) ? color * (1.0 / maxC) : color;
	const Vector3D& x = result.getPosition();
	const Vector3D& n = result.getNormal();
	Vector3D nl = n.dot(dir) < 0 ? n : n * -1;

	if (type == IdealType::DIFFUSE) {
		double r1 = 2 * Math::PI * rand();
		double r2 = rand();
		double r2s = std::sqrt(r2);

		const Vector3D& w = nl;
		const Vector3D& wo = (w.x() > 0.1 || w.x() < -0.1) ? Vector3D::Yaxis : Vector3D::Xaxis;
		Vector3D u = wo.cross(w).norm();
		Vector3D v = w.cross(u);

		Vector3D d = (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1 - r2)).norm();
	
		return emission + f.modulate(pathTraceRecursive(scene, Ray3D(x, d), newDepth, rand));

	} else if (type == IdealType::SPECULAR) {
		Vector3D reflDirection = dir - n.dot(dir) * 2 * n;
		return emission + f.modulate(pathTraceRecursive(scene, Ray3D(x, reflDirection), newDepth, rand));
	} else {
		Vector3D reflDirection = dir - n * (2 * n.dot(dir));
		Ray3D reflRay(x, reflDirection);
		bool into = n.dot(nl) > 0;
		double nc = 1, nt = 1.5;
		double nnt = into ? nc / nt : nt / nc;
		double ddn = dir.dot(nl);
		double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

		if (cos2t < 0) return emission + f.modulate(pathTraceRecursive(scene, reflRay, newDepth, rand));
		else {
			Vector3D tdir = (dir * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
			double a = nt - nc;
			double b = nt + nc;
			double R0 = a * a / (b * b);
			double c = 1 - (into ? -ddn : tdir.dot(n));
			double Re = R0 + (1 - R0) * c * c * c * c * c;
			double Tr = 1 - Re;
			double P = .25 + .5*Re;
			double RP = Re / P;
			double TP = Tr / (1 - P);

			Color result;

			if (newDepth > 2){
				if (rand() < P) 
					result = pathTraceRecursive(scene, reflRay, newDepth, rand) * RP;
				else 
					result = pathTraceRecursive(scene, Ray3D(x, tdir), newDepth, rand) * TP;
			} else {
				result = pathTraceRecursive(scene, reflRay, newDepth, rand) * Re + pathTraceRecursive(scene, Ray3D(x, tdir), newDepth, rand) * Tr;
			}

			return emission + f.modulate(result);
		}
	}
}


Matrix<uint8> Render::pathTrace(const Geometry& scene, const PerspectiveCamera& camera, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();


	for (int i = 0; i < height; ++i) {
		const double y = 1 - i / double(height);
		Random rand;

		fprintf(stderr,"\rRendering %5.2f%%",100.*i/(height-1));

#pragma omp parallel for
		for (int j = 0; j < width; ++j) {
			const double x = j / double(width);
			Color clr, tmp;
			int samps = 10;

 			for (int sy = 0; sy < 2; ++sy) {
 				for (int sx = 0; sx < 2; ++sx) {
 					for (int s = 0; s < samps; ++s) {
 						double r1 = 2 * rand();
 						double r2 = 2 * rand();
 						double dx = r1 < 1 ? std::sqrt(r1) - 1 : 1 - std::sqrt(2 - r1);
 						double dy = r2 < 1 ? std::sqrt(r2) - 1 : 1 - std::sqrt(2 - r2);
 
 						const Ray3D ray = camera.generateRay(((sx + 0.5 + dx) * 0.5 + j) / width, ((sy + 0.5 + dy) * 0.5 + height - 1 - i) / height);
 						
 						clr += pathTraceRecursive(scene, ray, 0, rand) * (1.0 / samps);
 					}
 
 					tmp += Color(Math::clip(clr.r, .0, 1.0), Math::clip(clr.g, .0, 1.0), Math::clip(clr.b, .0, 1.0)) * .25;
 				}
 			}

			m(i, j, 0) = convert(tmp.r);
			m(i, j, 1) = convert(tmp.g);
			m(i, j, 2) = convert(tmp.b);
		}
	}


	return std::move(m);
}






void test() {

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

//#pragma omp parallel for schedule(dynamic, 1)       // OpenMP
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
}


