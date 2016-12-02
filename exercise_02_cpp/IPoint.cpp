#include "IPoint.h"

namespace my3DDatatypes {

	IPoint::IPoint() {
		dist = -1;
	}

	IPoint::IPoint(const Triangle &tt, const Vec3D &ip, float d) {
		triangle = tt;
		ipoint = ip;
		dist = d;
	}

}
