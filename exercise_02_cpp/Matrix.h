#ifndef MATRIXH
#define MATRIXH

#include <vector>
#include "Vec3D.h"
#include "Triangle.h"

namespace my3DDatatypes {

	class Matrix {

	public:

		std::vector<std::vector<float>> val;
		Matrix();
		Matrix(std::vector<std::vector<float>> vs);
		void print();
		Matrix mult(const Matrix& m);
		Vec3D mult(const Vec3D& v);

		static Matrix createId();
		static Matrix createXRotation(float angle);
		static Matrix createYRotation(float angle);
		static Matrix createZRotation(float angle);
		static Matrix createTranslation(float dx, float dy, float dz);
		void apply(std::vector<Triangle*> ts);
	};

}

#endif
