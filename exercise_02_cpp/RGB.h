#ifndef RGBH
#define RGBH

namespace my3DDatatypes {

	class RGB {

	public:
		float red, green, blue;

		RGB();
		RGB(float r, float g, float b);

		int getred();
		int getgreen();
		int getblue();

		RGB addColor(const RGB &c2, float ratio);

	};

}

#endif
