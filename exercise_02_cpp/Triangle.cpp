#include <vector>

#include "Triangle.h"
#include "Vec3D.h"
#include "RGB.h"

namespace my3DDatatypes {

	Triangle::Triangle() { }

	Triangle::Triangle(const Vec3D& pp1, const Vec3D& pp2, const Vec3D& pp3, const RGB& col, float sh) {
		p1 = pp1;
		p2 = pp2;
		p3 = pp3;
		color = col;
		shininess = sh;
		Vec3D e1 = p2.minus(p1);
		Vec3D e2 = p3.minus(p1);
		normal = e1.cross(e2);
		normal.normalize();
	}

	std::vector<Triangle*> Triangle::createCube(int x, int y, int z, int w, int h, int d, const RGB& c, float sh) { // front
		std::vector<Triangle*> triangles;

		triangles.push_back(new Triangle(Vec3D(x, y, z), Vec3D(x + w, y, z), Vec3D(x, y + h, z), c, sh));
		triangles.push_back(new Triangle(Vec3D(x + w, y, z), Vec3D(x + w, y + h, z), Vec3D(x, y + h, z), c, sh));
		// left
		triangles.push_back(new Triangle(Vec3D(x, y, z + d), Vec3D(x, y, z), Vec3D(x, y + h, z), c, sh));
		triangles.push_back(new Triangle(Vec3D(x, y + h, z), Vec3D(x, y + h, z + d), Vec3D(x, y, z + d), c, sh));
		// right
		triangles.push_back(new Triangle(Vec3D(x + w, y, z), Vec3D(x + w, y, z + d), Vec3D(x + w, y + h, z), c, sh));
		triangles.push_back(new Triangle(Vec3D(x + w, y + h, z), Vec3D(x + w, y, z + d), Vec3D(x + w, y + h, z + d), c, sh));
		// top
		triangles.push_back(new Triangle(Vec3D(x + w, y + h, z), Vec3D(x + w, y + h, z + d), Vec3D(x, y + h, z), c, sh));
		triangles.push_back(new Triangle(Vec3D(x, y + h, z), Vec3D(x + w, y + h, z + d), Vec3D(x, y + h, z + d), c, sh));
		// bottom
		triangles.push_back(new Triangle(Vec3D(x + w, y, z), Vec3D(x, y, z), Vec3D(x, y, z + d), c, sh));
		triangles.push_back(new Triangle(Vec3D(x, y, z + d), Vec3D(x + w, y, z + d), Vec3D(x + w, y, z), c, sh));
		// back
		triangles.push_back(new Triangle(Vec3D(x, y, z + d), Vec3D(x, y + h, z + d), Vec3D(x + w, y, z + d), c, sh));
		triangles.push_back(new Triangle(Vec3D(x + w, y, z + d), Vec3D(x, y + h, z + d), Vec3D(x + w, y + h, z + d), c, sh));

		return triangles;

	}
}
