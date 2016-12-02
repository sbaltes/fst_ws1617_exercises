#include "Light.h"
#include "Vec3D.h"
#include "RGB.h"


namespace my3DDatatypes {
	Light::Light(const Vec3D& pos, const RGB& c) {
		position = pos;
		color = c;
	}
}
