#ifndef IPOINTH
#define IPOINTH

#include "Triangle.h"
#include "Vec3D.h"

namespace my3DDatatypes {

	class IPoint {

	public:
		static constexpr float epsilon = 0.0001f;
		Triangle triangle;
		Vec3D ipoint;
		float dist;

		IPoint();

		IPoint(const Triangle& tt, const Vec3D& ip, float d);

	};

}

#endif
