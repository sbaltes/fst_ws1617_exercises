#ifndef RAYH
#define RAYH

#include "Vec3D.h"
#include "IPoint.h"
#include "Triangle.h"

namespace my3DDatatypes {

class Ray {
	public:
		Vec3D start = Vec3D(0, 0, 0);
	  Vec3D dir = Vec3D(0, 0, 0);

	void setStart(float x, float y, float z);
	void setDir(float dx, float dy, float dz);
	void normalize();

	IPoint intersect(Triangle& t);
};

}

#endif
