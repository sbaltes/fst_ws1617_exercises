#ifndef VEC3DH
#define VEC3DH

namespace my3DDatatypes {

	class Vec3D {

	public:
		float x, y, z, w = 1;

		Vec3D();
		Vec3D(float xx, float yy, float zz);
		Vec3D(float xx, float yy, float zz, float ww);
		Vec3D add(const Vec3D& v);
		Vec3D minus(const Vec3D& v);
		Vec3D mult(float a);
		void normalize();
		float dot(const Vec3D& v);
		Vec3D cross(const Vec3D& v);

	};

}

#endif
