#ifndef SDRAYTRACEREXAMPLEH
#define SDRAYTRACEREXAMPLEH

#include "SDRaytracer.h"

namespace SDRaytracerExample {

	using namespace mySDRaytracer;

	class SDRaytracerExample {

	public:

		const float  PI_F=3.14159265358979f;

		SDRaytracer sd;

		void createScene();
		void updateScene();
		SDRaytracerExample();

	};

}

#endif
