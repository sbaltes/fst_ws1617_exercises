#include <iostream>
#include <cmath>
#include <vector>

#include "Matrix.h"
#include "Vec3D.h"

namespace my3DDatatypes {

	Matrix::Matrix() { }

	Matrix::Matrix(std::vector<std::vector<float>> vs) {
		val = vs;
	}

	void Matrix::print() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::cout << " "  << val[i][j] << "       ";
			}
			std::cout << std::endl;
		}
	}

	Matrix Matrix::mult(const Matrix& m) {
		Matrix r = createId();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				float sum = 0;
				for (int k = 0; k < 4; k++)
					sum = sum + val[i][k] * m.val[k][j];
				r.val[i][j] = sum;
			}
		return r;
	}

	Vec3D Matrix::mult(const Vec3D& v) {
		Vec3D temp = Vec3D(val[0][0] * (v.x) + val[0][1] * (v.y) + val[0][2] * (v.z) + val[0][3] * (v.w),
				val[1][0] * (v.x) + val[1][1] * (v.y) + val[1][2] * (v.z) + val[1][3] * (v.w),
				val[2][0] * (v.x) + val[2][1] * (v.y) + val[2][2] * (v.z) + val[2][3] * (v.w),
				val[3][0] * (v.x) + val[3][1] * (v.y) + val[3][2] * (v.z) + val[3][3] * (v.w));
		// return new Vec3D(temp.x/temp.w,temp.y/temp.w,temp.z/temp.w,1);
		temp.x = temp.x / temp.w;
		temp.y = temp.y / temp.w;
		temp.z = temp.z / temp.w;
		temp.w = 1;
		return temp;
	}

	Matrix Matrix::createId() {
		std::vector<std::vector<float>> tmp;
		tmp.resize(4);
		for(int i=0; i<4; i++) {
			tmp[i].resize(4);
		}

		tmp ={{ 1, 0, 0, 0 } , { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } ;

		return Matrix(tmp);
	}

	Matrix Matrix::createXRotation(float angle) {

		Matrix m = createId();

		m.val[1] = { 0, (float) std::cos(angle), (float) -std::sin(angle), 0 };
		m.val[2] = { 0, (float) std::sin(angle), (float) std::cos(angle), 0 };

		return m;
	}

	Matrix Matrix::createYRotation(float angle) {

		Matrix m = createId();

		m.val[0] = { (float) std::cos(angle), 0, (float) std::sin(angle), 0 };
		m.val[2] = { (float) -std::sin(angle), 0, (float) std::cos(angle), 0 };

		return m;

	}

	Matrix Matrix::createZRotation(float angle) {

		Matrix m = createId();

		m.val[0] = { (float) std::cos(angle), (float) -std::sin(angle), 0, 0 };
		m.val[1] = { (float) std::sin(angle), (float) std::cos(angle), 0, 0 };

		return m;

	}

	Matrix Matrix::createTranslation(float dx, float dy, float dz) {

		Matrix m = createId();

		m.val[0] = { 1, 0, 0, dx };
		m.val[1] = { 0, 1, 0, dy };
		m.val[2] = { 0, 0, 1, dz };

		return m;

	}

	void Matrix::apply(std::vector<Triangle*> ts) {
		for (auto &t : ts) {
			t->p1 = mult(t->p1);
			t->p2 = mult(t->p2);
			t->p3 = mult(t->p3);
			Vec3D e1 = t->p2.minus(t->p1);
			Vec3D e2 = t->p3.minus(t->p1);
			t->normal = e1.cross(e2);
			t->normal.normalize();
		}
	}
}
