#include "RGB.h"

namespace my3DDatatypes {

	RGB::RGB() { }

	RGB::RGB(float r, float g, float b) {
		if (r > 1)
			r = 1;
		else if (r < 0)
			r = 0;
		if (g > 1)
			g = 1;
		else if (g < 0)
			g = 0;
		if (b > 1)
			b = 1;
		else if (b < 0)
			b = 0;
		red = r;
		green = g;
		blue = b;
	}

	int RGB::getred() {
		return (int) (red*255);
	}

	int RGB::getgreen() {
		return (int) (green*255);
	}

	int RGB::getblue() {
		return (int) (blue*255);
	}

	RGB RGB::addColor(const RGB& c2, float ratio) {
		return RGB((red + c2.red * ratio), (green + c2.green * ratio), (blue + c2.blue * ratio));
	}

}
