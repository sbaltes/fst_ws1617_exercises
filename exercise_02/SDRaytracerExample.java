import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JPanel;

import my3DDatatypes.Light;
import my3DDatatypes.Matrix;
import my3DDatatypes.RGB;
import my3DDatatypes.Triangle;
import my3DDatatypes.Vec3D;
import myRaytracer.SDRaytracer;

public class SDRaytracerExample extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	SDRaytracer sd;

	public static void main(String argv[]) {
		long start = System.currentTimeMillis();
		SDRaytracerExample sdr = new SDRaytracerExample();
		long end = System.currentTimeMillis();
		long time = end - start;
		System.out.println("time: " + time + " ms");
		System.out.println("nrprocs=" + sdr.sd.nrOfProcessors);
	}

	public void createScene() {
		List<Triangle> triangles = new ArrayList<Triangle>();

		triangles.addAll(Triangle.createCube(0, 35, 0, 10, 10, 10, new RGB(0.3f, 0, 0), 0.4f));
		triangles.addAll(Triangle.createCube(-70, -20, -20, 20, 100, 100, new RGB(0f, 0, 0.3f), .4f));
		triangles.addAll(Triangle.createCube(-30, 30, 40, 20, 20, 20, new RGB(0, 0.4f, 0), 0.2f));
		triangles.addAll(Triangle.createCube(50, -20, -40, 10, 80, 100, new RGB(.5f, .5f, .5f), 0.2f));
		triangles.addAll(Triangle.createCube(-70, -26, -40, 130, 3, 40, new RGB(.5f, .5f, .5f), 0.2f));

		Matrix mRx = Matrix.createXRotation((float) (sd.x_angle_factor * Math.PI / 16));
		Matrix mRy = Matrix.createYRotation((float) (sd.y_angle_factor * Math.PI / 16));
		Matrix mT = Matrix.createTranslation(0, 0, 200);
		Matrix m = mT.mult(mRx).mult(mRy);
		m.print();
		m.apply(triangles);

		Light mainLight = new Light(new Vec3D(0, 100, 0), new RGB(0.1f, 0.1f, 0.1f));

		Light lights[] = new Light[] { mainLight, new Light(new Vec3D(100, 200, 300), new RGB(0.5f, 0, 0.0f)),
				new Light(new Vec3D(-100, 200, 300), new RGB(0.0f, 0, 0.5f))
				// ,new Light(new Vec3D(-100,0,0), new RGB(0.0f,0.8f,0.0f))
		};

		sd.lights = lights;

		sd.fovx = (float) 0.628;
		sd.fovy = (float) 0.628;
		sd.ambient_color = new RGB(0.01f, 0.01f, 0.01f);
		sd.background_color = new RGB(0.05f, 0.05f, 0.05f);

		sd.triangles = triangles;

	}

	public SDRaytracerExample() {
		sd = new SDRaytracer();

		sd.y_angle_factor = 4;
		sd.x_angle_factor = -4;

		createScene();

		if (!sd.profiling)
			sd.renderImage();
		else
			sd.profileRenderImage();

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		Container contentPane = this.getContentPane();
		contentPane.setLayout(new BorderLayout());
		JPanel area = new JPanel() {
			public void paint(Graphics g) {
				System.out.println("fovx=" + sd.fovx + ", fovy=" + sd.fovy + ", xangle=" + sd.x_angle_factor
						+ ", yangle=" + sd.y_angle_factor);
				if (sd.image == null)
					return;
				for (int i = 0; i < sd.width; i++)
					for (int j = 0; j < sd.height; j++) {
						g.setColor(sd.image[i][j].color());
						// zeichne einzelnen Pixel
						g.drawLine(i, sd.height - j, i, sd.height - j);
					}
			}
		};

		addKeyListener(new KeyAdapter() {
			public void keyPressed(KeyEvent e) {
				boolean redraw = true;

				switch (e.getKeyCode()) {

				case KeyEvent.VK_DOWN:
					sd.x_angle_factor--;
					break;

				case KeyEvent.VK_UP:
					sd.x_angle_factor++;
					break;

				case KeyEvent.VK_LEFT:
					sd.y_angle_factor--;
					break;

				case KeyEvent.VK_RIGHT:
					sd.y_angle_factor++;
					break;

				default:
					redraw = false;
					break;

				}

				if (redraw) {
					createScene();
					sd.renderImage();
					repaint();
				}
			}
		});

		area.setPreferredSize(new Dimension(sd.width, sd.height));
		contentPane.add(area);
		this.pack();
		this.setVisible(true);
	}

}
