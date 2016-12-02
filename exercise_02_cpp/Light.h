#ifndef LIGHTH
#define LIGHTH

#include "RGB.h"
#include "Vec3D.h"

namespace my3DDatatypes {
	class Light {
	public:
		RGB color;
		Vec3D position;

		Light(const Vec3D& pos, const RGB& c);
	};

}


#endif
