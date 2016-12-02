#ifndef TRIANGLEH
#define TRIANGLEH

#include <vector>
#include "Vec3D.h"
#include "RGB.h"

namespace my3DDatatypes {

	class Triangle {

	public:
		Vec3D p1;
		Vec3D p2;
		Vec3D p3;
		RGB color;
		Vec3D normal;
		float shininess;

		Triangle();

		Triangle(const Vec3D& pp1, const Vec3D& pp2, const Vec3D& pp3, const RGB& col, float sh);

		static std::vector<Triangle*> createCube(int x, int y, int z, int w, int h, int d, const RGB& c, float sh);

	};

}

#endif
