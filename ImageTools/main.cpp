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
#include "LambertMaterial.h"

// 平行光源
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

// 点光源
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


// 聚光灯
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


// 复杂场景测试
void boxAndSphereTest() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1), 0);
	auto plane2 = make_shared<Plane>(Vector3D(1, 0, 0), -20);
	auto plane3 = make_shared<Plane>(Vector3D(0, 1, 0), -30);
	auto plane4 = make_shared<Plane>(Vector3D(0, -1, 0), -30);
	auto plane5 = make_shared<Plane>(Vector3D(0, 0, -1), -42);

	// Two spheres
	auto sphere1 = make_shared<Sphere>(Vector3D(-10, -12, 10), 10);
	auto sphere2 = make_shared<Sphere>(Vector3D(-10, 12, 10), 10);
	  
	plane1->setMaterial(make_shared<CheckerMaterial>(0.1, 0.5));
	plane2->setMaterial(make_shared<LambertMaterial>(Color(0, 0.5, 0.5)));
	plane3->setMaterial(make_shared<LambertMaterial>(Color(0.5, 0.5, 0.5)));
	plane4->setMaterial(make_shared<LambertMaterial>(Color(0, 0.2, 0.5)));
	plane5->setMaterial(make_shared<LambertMaterial>(Color(0.25, 0.75, 0.25)));


	sphere1->setMaterial(make_shared<PhongMaterial>(Color(1,0,0), Color::WHITE, 10, 0.25));
	sphere2->setMaterial(make_shared<PhongMaterial>(Color(0.5,0.5,0.5), Color::WHITE, 16, 0.25));
	  
	UnionGeometry geometries({plane1, plane2, plane3, plane4, plane5, sphere1, sphere2});
	
	//vector<shared_ptr<Light>> lights{ make_shared<SpotLight>(Color::WHITE*10000, Vector3D(20,0, 80), Vector3D(-1, 0, -1), 80, 90, 1) };

	vector<shared_ptr<Light>> lights{ 
		make_shared<PointLight>(Color::WHITE * 800, Vector3D(20, -20, 40)),
		make_shared<PointLight>(Color::WHITE * 800, Vector3D(20, 0, 40)),
		make_shared<PointLight>(Color::WHITE * 800, Vector3D(20, 20, 40)),
		make_shared<PointLight>(Color::WHITE * 300, Vector3D(-30, 0, 0)),
	};
	
	int s = 1024;
	 
	Matrix<uint8> mat = Render::rayTraceReflection(geometries, 
										lights,
	 									PerspectiveCamera(Vector3D(20,0,20), Vector3D(-1,0,0), Vector3D(0,0,1), 100),
	 									50,
	 									Size(s, s, 3));
	 
	PXMImage::save(mat, "E:\\render.ppm");
}

// Lambert材质测试
void lambertTest() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1), 0);
	auto plane2 = make_shared<Plane>(Vector3D(0, 1, 0), 0);
	auto plane3 = make_shared<Plane>(Vector3D(1, 0, 0), 0);
	
	auto sphere1 = make_shared<Sphere>(Vector3D(10, 10, 10), 10);

	plane1->setMaterial(make_shared<LambertMaterial>(Color(0.75,0.25,0.25)));
	plane2->setMaterial(make_shared<LambertMaterial>(Color(0.5,0.5,0.75)));
	plane3->setMaterial(make_shared<LambertMaterial>(Color(0.5,0.5,0.5)));

	sphere1->setMaterial(make_shared<PhongMaterial>(Color(1, 0, 0), Color::WHITE, 10, 0));

	UnionGeometry geometries({ plane1, plane2, plane3, sphere1 });

	vector<shared_ptr<Light>> lights{ make_shared<DirectionalLight>(Color::WHITE, Vector3D(-1, -1, -2)) };

	int s = 512;

	Matrix<uint8> mat = Render::rayTraceReflection(geometries,
												   lights,
												   PerspectiveCamera(Vector3D(40, 20, 15), Vector3D(-1, -0.1, 0), Vector3D(0, 0, 1), 90),
												   5,
												   Size(s, s, 3));

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


	int s = 512;
	Matrix<uint8> mat =
		Render::renderLight(geometries,
		lights,
		PerspectiveCamera(Vector3D(0, 10, 10), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90),
		Size(s, s, 3));

	PXMImage::save(mat, "E:\\render.ppm", ImageType::P6);
}


// 动画场景测试
void animationTest() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1), 0);      // ground
	auto plane2 = make_shared<Plane>(Vector3D(1, 0, 0), -20);    // back
	auto plane3 = make_shared<Plane>(Vector3D(0, 1, 0), -29.9);  // left
	auto plane4 = make_shared<Plane>(Vector3D(0, -1, 0), -29.9); // right
	auto plane5 = make_shared<Plane>(Vector3D(0, 0, -1), -42);   // ceil

	int num = 180;

	for (int i = 0; i < num; ++i) {
		// Two spheres
		auto sphere1 = make_shared<Sphere>(Vector3D(-10, 0, 20), 8);

		const double delta = i * 2 * Math::PI / num;
		const double z = 20 + 15 * std::sin(delta);
		const double y = 15 * std::cos(delta);

		auto sphere2 = make_shared<Sphere>(Vector3D(-8, y, z), 5);
		auto sphere3 = make_shared<Sphere>(Vector3D(-8, -y, 40 - z), 5);

		plane1->setMaterial(make_shared<CheckerMaterial>(0.1, 0.5));
		plane2->setMaterial(make_shared<LambertMaterial>(Color(0, 0.5, 0.5)));
		plane3->setMaterial(make_shared<LambertMaterial>(Color(0.5, 0.5, 0.5)));
		plane4->setMaterial(make_shared<LambertMaterial>(Color(0, 0.5, 0.75)));
		plane5->setMaterial(make_shared<PhongMaterial>(Color(0.25, 0.25, 0.75), Color(0.25, 0.25, 0.75), 50, 0.4));


		sphere1->setMaterial(make_shared<PhongMaterial>(Color(1, 0, 0), Color::WHITE, 5, 0.3));
		sphere2->setMaterial(make_shared<PhongMaterial>(Color::BLUE, Color::BLUE, 10, 0.2));
		sphere3->setMaterial(make_shared<PhongMaterial>(Color(0.5, 0.5, 0.5), Color(0.5, 0.5, 0.5), 10, 0.2));

		UnionGeometry geometries({ plane1, plane2, plane3, plane4, plane5, sphere1, sphere2, sphere3 });


		vector<shared_ptr<Light>> lights{
			make_shared<PointLight>(Color::WHITE * 900, Vector3D(20, -20, 40)),
			//make_shared<PointLight>(Color::WHITE * 800, Vector3D(20, 0, 40)),
			make_shared<PointLight>(Color::WHITE * 900, Vector3D(20, 20, 40)),
			make_shared<DirectionalLight>(Color::WHITE*0.25, Vector3D(-1, 0, 0)),
		};

		int s = 800;

		Matrix<uint8> mat = Render::rayTraceReflection(geometries,
													   lights,
													   PerspectiveCamera(Vector3D(20, 0, 20), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 100),
													   50,
													   Size(s, s, 3));
		string filename = String::format("E:\\zzz\\%d.ppm", i);
		PXMImage::save(mat, filename);
	}
}


// 
void sphereTest() {
	auto sphere1 = make_shared<Sphere>(Vector3D(-10, 0, 10), 10);
	sphere1->setMaterial(make_shared<PhongMaterial>(Color(1, 0, 0), Color::WHITE, 10, 0.2));

	UnionGeometry geometries({ sphere1 });

	vector<shared_ptr<Light>> lights{
		make_shared<DirectionalLight>(Color::WHITE, Vector3D(-1, 0, -1)),

	};

	int s = 800;

	Matrix<uint8> mat = Render::rayTraceReflection(geometries,
												   lights,
												   PerspectiveCamera(Vector3D(20, 0, 20), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 100),
												   50,
												   Size(s, s, 3));
	string filename = String::format("E:\\zzz\\%d.ppm", 0);
	PXMImage::save(mat, filename);
}


void globalIlluminationTest() {
	auto plane1 = make_shared<Plane>(Vector3D(0, 0, 1),    0);    // ground
	auto plane2 = make_shared<Plane>(Vector3D(1, 0, 0),  -100);  // back
	auto plane3 = make_shared<Plane>(Vector3D(0, 1, 0),  -60);  // left
	auto plane4 = make_shared<Plane>(Vector3D(0, -1, 0), -60); // right
	auto plane5 = make_shared<Plane>(Vector3D(0, 0, -1), -100); // ceil
	auto plane6 = make_shared<Plane>(Vector3D(-1, 0, 0), 0);   // front

	plane1->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane2->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane3->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.25, 0.25), Color::BLACK, IdealType::DIFFUSE));
	plane4->setMaterial(make_shared<IdealMaterial>(Color(0.25, 0.25, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane5->setMaterial(make_shared<IdealMaterial>(Color(0.75, 0.75, 0.75), Color::BLACK, IdealType::DIFFUSE));
	plane6->setMaterial(make_shared<IdealMaterial>(Color(0.50, 0.75, 0.50), Color::BLACK, IdealType::DIFFUSE));

	auto sphere1 = make_shared<Sphere>(Vector3D(-70, -30, 20), 20);
	auto sphere2 = make_shared<Sphere>(Vector3D(-50, 20, 20), 20);
	auto sphere3 = make_shared<Sphere>(Vector3D(-50, 0, 100 + 97), 100); // light

	sphere1->setMaterial(make_shared<IdealMaterial>(Color(1, 1, 1), Color::BLACK, IdealType::SPECULAR));
	sphere2->setMaterial(make_shared<IdealMaterial>(Color(1, 1, 1), Color::BLACK, IdealType::REFRACTIVE));
	sphere3->setMaterial(make_shared<IdealMaterial>(Color(.75, .75, .75), Color(7.5, 7.5, 7.5), IdealType::DIFFUSE));

	UnionGeometry geometries({ plane1, plane2, plane3, plane4, plane5, plane6, sphere1, sphere2, sphere3 });

	int w = 400;
	int h = 300;

	clock_t start = clock();

	Matrix<uint8> mat = Render::pathTrace(geometries,
										  PerspectiveCamera(Vector3D(120, 0, 50), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 40, (1.0 * w) / h),
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

	for (int i = 5; i < frame; ++i) {
		double radius = std::sqrt(50*50 + 80*80);
		double theta = 30 + i * 50.0 / (frame-1 + 1e-10);
		double x = -100 + radius * std::cos(theta * Math::PI / 180);
		double y = -60 + radius * std::sin(theta * Math::PI / 180);

		auto sphere2 = make_shared<Sphere>(Vector3D(x, y, 20), 20);
		sphere2->setMaterial(make_shared<IdealMaterial>(Color(1, 1, 1), Color::BLACK, IdealType::REFRACTIVE));

		UnionGeometry geometries({ plane1, plane2, plane3, plane4, plane5, plane6, sphere1, sphere2, sphere3 });

		int w = 400;
		int h = 300;

		clock_t start = clock();

		Matrix<uint8> mat = Render::pathTrace(geometries,
											  PerspectiveCamera(Vector3D(120, 0, 50), Vector3D(-1, 0, 0), Vector3D(0, 0, 1), 40, (1.0 * w) / h),
											  Size(h, w, 3));

		printf("\n%f sec\n", (float)(clock() - start) / CLOCKS_PER_SEC);

		string filename = String::format("E:\\zzz\\%d.ppm", i);
		PXMImage::save(mat, filename);
	}
}

int main(){

	globalIlluminationAnimation();
	//boxAndSphereTest();

	//test();


	return 0;
}