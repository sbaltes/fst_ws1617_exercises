#include <cstddef>
#include <iostream>

#include "Ray.h"
#include "Vec3D.h"
#include "IPoint.h"
#include "Triangle.h"



namespace my3DDatatypes {

	void Ray::setStart(float x, float y, float z) {
		start = Vec3D(x, y, z);
	}

	void Ray::setDir(float dx, float dy, float dz) {
		dir = Vec3D(dx, dy, dz);
	}

	void Ray::normalize() {
		dir.normalize();
	}

	// see Mueller&Haines, page 305
	IPoint Ray::intersect(Triangle &t) {
		float epsilon = IPoint::epsilon;
		Vec3D e1 = t.p2.minus(t.p1);
		Vec3D e2 = t.p3.minus(t.p1);
		Vec3D p = dir.cross(e2);
		float a = e1.dot(p);
		if ((a > -epsilon) && (a < epsilon))
		return IPoint();
		float f = 1 / a;
		Vec3D s = start.minus(t.p1);
		float u = f * s.dot(p);
		if ((u < 0.0) || (u > 1.0))
		return IPoint();
		Vec3D q = s.cross(e1);
		float v = f * dir.dot(q);
		if ((v < 0.0) || (u + v > 1.0))
		return IPoint();
		// intersection point is u,v
		float dist = f * e2.dot(q);
		if (dist < epsilon)
		return IPoint();
		Vec3D ip = t.p1.mult(1 - u - v).add(t.p2.mult(u)).add(t.p3.mult(v));
		//std::cout << "Intersection point: " << ip.x << "," << ip.y << "," << ip.z << std::endl;
		return IPoint(t, ip, dist);
	}
}
