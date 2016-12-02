#include <cmath>
#include "Vec3D.h"

namespace my3DDatatypes {

	Vec3D::Vec3D() { }

	Vec3D::Vec3D(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	Vec3D::Vec3D(float xx, float yy, float zz, float ww) {
		// major c++ bug Vec3D(xx, yy, zz);
		x = xx;
		y = yy;
		z = zz;
		w = ww;
	}

	Vec3D Vec3D::add(const Vec3D& v) {
		return Vec3D(x + v.x, y + v.y, z + v.z);
	}

	Vec3D Vec3D::minus(const Vec3D& v) {
		return Vec3D(x - v.x, y - v.y, z - v.z);
	}

	Vec3D Vec3D::mult(float a) {
		return Vec3D(a * x, a * y, a * z);
	}

	void Vec3D::normalize() {
		float dist = (float) std::sqrt((x * x) + (y * y) + (z * z));
		x = x / dist;
		y = y / dist;
		z = z / dist;
	}

	float Vec3D::dot(const Vec3D& v) {
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3D Vec3D::cross(const Vec3D& v) {
		return Vec3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
}
