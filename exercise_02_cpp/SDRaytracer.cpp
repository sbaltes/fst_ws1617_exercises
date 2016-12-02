#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>

#include "SDRaytracer.h"
#include "RGB.h"
#include "Ray.h"
#include "IPoint.h"
#include "Triangle.h"
#include "Vec3D.h"
#include "Light.h"

namespace mySDRaytracer {

	/* Implementation of a very simple Raytracer
	Stephan Diehl, Universitaet Trier, 2010-2016
	*/

	using namespace my3DDatatypes;

	SDRaytracer::SDRaytracer() {
		ambient_color = RGB(0, 0, 0);
		background_color = RGB(0, 0, 0);
		black = RGB(0.0, 0.0, 0.0);

		image.resize(width);
		for(int i=0; i<height; i++) {
			image[i].resize(height);
		}
	}

	void SDRaytracer::profileRenderImage() {
		std::chrono::milliseconds end, start, time;

		renderImage(); // initialisiere Datenstrukturen, erster Lauf verfaelscht
		// sonst Messungen

		for (int procs = 1; procs < 6; procs++) {

			maxRec = procs - 1;
			std::cout << procs;
			for (int i = 0; i < 10; i++) {
				start = std::chrono::duration_cast< std::chrono::milliseconds >(
					std::chrono::system_clock::now().time_since_epoch()
				);

				renderImage();

				end = std::chrono::duration_cast< std::chrono::milliseconds >(
					std::chrono::system_clock::now().time_since_epoch()
				);
				time = end - start;
				std::cout << ";" << time.count();
			}
			std::cout << std::endl;
		}
	}

	void SDRaytracer::renderImage() {
		tan_fovx = std::tan(fovx);
		tan_fovy = std::tan(fovy);
		#pragma omp parallel num_threads(8)
		{
			#pragma omp for schedule(dynamic)
			for (int i = 0; i < width; ++i) {
				for (int j = 0; j < height; ++j) {
					image[i][j] = RGB(0, 0, 0);
					for (int k = 0; k < rayPerPixel; ++k) {
						double di = i + (((float)std::rand()/RAND_MAX) / 2 - 0.25);
						double dj = j + (((float)std::rand()/RAND_MAX) / 2 - 0.25);

						if (rayPerPixel == 1) {
							di = i;
							dj = j;
						}
						Ray eye_ray;
						eye_ray.setStart(startX, startY, startZ); // ro
						eye_ray.setDir((float) (((0.5 + di) * tan_fovx * 2.0) / width - tan_fovx),
						(float) (((0.5 + dj) * tan_fovy * 2.0) / height - tan_fovy), (float) 1.0f); // rd
						eye_ray.normalize();
						image[i][j] = image[i][j].addColor(rayTrace(eye_ray, 0), 1.0f / rayPerPixel);
					}
				}
			}
		}
	}

	RGB SDRaytracer::rayTrace(Ray& ray, int rec) {
		if (rec > maxRec)
		return black;
		IPoint ip = hitObject(ray); // (ray, p, n, triangle);
		if (ip.dist > IPoint::epsilon)
		return lighting(ray, ip, rec);
		else
		return black;
	}

	IPoint SDRaytracer::hitObject(Ray& ray) {
		IPoint isect;
		float idist = -1;

		for (auto &t : triangles) {
			IPoint ip = ray.intersect(*t);
			if (ip.dist != -1)
			if ((idist == -1) || (ip.dist < idist)) { // save that
				// intersection
				idist = ip.dist;
				isect.ipoint = ip.ipoint;
				isect.dist = ip.dist;
				isect.triangle = *t;
			}
		}
		return isect; // return intersection point and normal
	}

	RGB SDRaytracer::lighting(Ray& ray, const IPoint& ip, int rec) {
		Vec3D point = ip.ipoint;
		Triangle triangle = ip.triangle;
		RGB color = triangle.color.addColor(ambient_color, 1);
		Ray shadow_ray;
		for (auto &light : lights) {
			shadow_ray.start = point;
			shadow_ray.dir = light.position.minus(point).mult(-1);
			shadow_ray.dir.normalize();
			IPoint ip2 = hitObject(shadow_ray);
			if (ip2.dist < IPoint::epsilon) {
				float ratio = std::max(0.0f, shadow_ray.dir.dot(triangle.normal));
				color = color.addColor(light.color, ratio);
			}
		}
		Ray reflection;
		// R = 2N(N*L)-L) L ausgehender Vektor
		Vec3D L = ray.dir.mult(-1);
		reflection.start = point;
		reflection.dir = triangle.normal.mult(2 * triangle.normal.dot(L)).minus(L);
		reflection.dir.normalize();
		RGB rcolor = rayTrace(reflection, rec + 1);
		float ratio = (float) std::pow(std::max(0.0f, reflection.dir.dot(L)), triangle.shininess);
		color = color.addColor(rcolor, ratio);
		return color;
	}

}
