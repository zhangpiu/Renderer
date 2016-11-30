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
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Geometry.h"
#include "PerspectiveCamera .h"
#include "Matrix.h"
#include "Color.h"
#include "MyMath.h"
#include "Light.h"
#include "IdealMaterial.h"
#include "UnionGeometry.h"

#include <algorithm>
#include <ctime>
#include <random>
#include <array>


class Random {
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _dist;

public:

 	Random() 
 		: _mt(std::random_device()())
 		, _dist(0, 1.0) {
	}
 
 	double operator()() {
 		return _dist(_mt);
 	}
};

class Render {
public:
	static Matrix<uint8> rayTrace(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, int maxReflect, const Size& size);

	static Matrix<uint8> renderLight(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, const Size& size);

	static Matrix<uint8> pathTrace(const Geometry& scene, const PerspectiveCamera& camera, int samples, const Size& size);

	static Color pathTraceRecursive(const Geometry& scene, const Ray3D& ray, int depth, Random& rand);

private:
	static Color rayTraceRecursive(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const Ray3D& ray, int maxReflect);

};


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


/*------------------------------------------------------------------------------------------/
| function:    rayTrace
| description:
|
|
| input:       @param scene:
|              @param lights:
|              @param camera:
|              @param maxReflect:
|              @param size:
|
| blog:        http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html
| note:        [10/28/2016 vodka]
|-----------------------------------------------------------------------------------------*/
Matrix<uint8> Render::rayTrace(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, int maxReflect, const Size& size) {
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


/*------------------------------------------------------------------------------------------/
| function:    renderLight
| description:
|
|
| input:       @param scene:
|              @param lights:
|              @param camera:
|              @param size:
|
| blog:        http://www.cnblogs.com/miloyip/archive/2010/04/02/1702768.html
| note:        [10/28/2016 vodka]
|-----------------------------------------------------------------------------------------*/
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
				m(i, j, 1) = convert(clr.g);
				m(i, j, 2) = convert(clr.b);
			}
		}
	}


	return std::move(m);
}



Color Render::pathTraceRecursive(const Geometry& scene, const Ray3D& ray, int depth, Random& rand) {
	const auto result = scene.intersect(ray);

	// if miss, return black.
	if (result.getGeometry() == nullptr) return Color::BLACK;

	const auto& material = result.getGeometry()->getMaterial();
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


/*------------------------------------------------------------------------------------------/
| function:    pathTrace
| description:
|
|
| input:       @param scene:
|              @param camera:
|              @param samples:
|              @param size:
|
| blog:        http://www.cnblogs.com/miloyip/archive/2010/04/02/1702768.html
| note:        [10/28/2016 vodka]
|-----------------------------------------------------------------------------------------*/
Matrix<uint8> Render::pathTrace(const Geometry& scene, const PerspectiveCamera& camera, int samples, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();
	Random rand;

	for (int i = 0; i < height; ++i) {
		fprintf(stderr,"\rRendering (%dx4 = %d spp) %5.2f%%", samples, samples*4, 100.*i/(height-1));

#pragma omp parallel for
		for (int j = 0; j < width; ++j) {
			Color clr, sum;

 			for (int sy = 0; sy < 2; ++sy) {
 				for (int sx = 0; sx < 2; ++sx) {
 					for (int s = 0; s < samples; ++s) {
 						double r1 = 2 * rand();
 						double r2 = 2 * rand();
 						double dx = r1 < 1 ? std::sqrt(r1) - 1 : 1 - std::sqrt(2 - r1);
 						double dy = r2 < 1 ? std::sqrt(r2) - 1 : 1 - std::sqrt(2 - r2);
 
 						const Ray3D ray = camera.generateRay(((sx + 0.5 + dx) * 0.5 + j) / width, ((sy + 0.5 + dy) * 0.5 + height - 1 - i) / height);
 						
 						clr += pathTraceRecursive(scene, ray, 0, rand) * (1.0 / samples);
 					}
 
 					sum += Color(Math::clip(clr.r, .0, 1.0), Math::clip(clr.g, .0, 1.0), Math::clip(clr.b, .0, 1.0)) * .25;
 				}
 			}

			m(i, j, 0) = convert(sum.r);
			m(i, j, 1) = convert(sum.g);
			m(i, j, 2) = convert(sum.b);
		}
	}


	return std::move(m);
}
