package my3DDatatypes;
import java.awt.Color;

public class RGB {
	public float red, green, blue;
	public Color color;

	public RGB(float r, float g, float b) {
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

	public Color color() {
		if (color != null)
			return color;
		color = new Color((int) (red * 255), (int) (green * 255), (int) (blue * 255));
		return color;
	}
	
	public RGB addColor(/*RGB c1,*/ RGB c2, float ratio) {
		return new RGB((this.red + c2.red * ratio), (this.green + c2.green * ratio), (this.blue + c2.blue * ratio));
	}

}