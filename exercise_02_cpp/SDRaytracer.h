#include <memory>
#include <vector>
#include "Triangle.h"
#include "Light.h"
#include "RGB.h"
#include "Ray.h"
#include "IPoint.h"

#ifndef SDRAYTRACERH
#define SDRAYTRACERH

namespace mySDRaytracer {

	/* Implementation of a very simple Raytracer
	Stephan Diehl, Universitaet Trier, 2010-2016
	*/

	using namespace my3DDatatypes;

	class SDRaytracer {

	public:
		bool profiling = false;
		int width = 1000;
		int height = 1000;
		int nrOfProcessors;
		int maxRec = 3;
		int rayPerPixel = 1;
		int startX, startY, startZ;
		float fovx = (float) 0.628;
		float fovy = (float) 0.628;
		float y_angle_factor;
		float x_angle_factor;
		double tan_fovx;
		double tan_fovy;

		RGB ambient_color;
		RGB background_color;
		RGB black;
		Ray eye_ray;

		std::vector<Triangle*> triangles;
		std::vector<Light> lights;
		std::vector<std::vector<RGB>> image;

		SDRaytracer();
		void profileRenderImage();
		void renderImage();
		RGB rayTrace(Ray& ray, int rec);
		IPoint hitObject(Ray& ray);
		RGB lighting(Ray& ray, const IPoint& ip, int rec);

	};

}

#endif
