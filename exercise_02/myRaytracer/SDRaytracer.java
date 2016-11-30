package myRaytracer;

import my3DDatatypes.IPoint;
import my3DDatatypes.Light;
import my3DDatatypes.RGB;
import my3DDatatypes.Ray;
import my3DDatatypes.Triangle;
import my3DDatatypes.Vec3D;

import java.util.List;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

/* Implementation of a very simple Raytracer
   Stephan Diehl, Universitaet Trier, 2010-2016
*/

public class SDRaytracer {
	public boolean profiling = false;
	public int width = 1000;
	public int height = 1000;

	Future[] futureList = new Future[width];
	public int nrOfProcessors = Runtime.getRuntime().availableProcessors();
	ExecutorService eservice = Executors.newFixedThreadPool(nrOfProcessors);

	int maxRec = 3;
	int rayPerPixel = 1;
	public int startX, startY, startZ;

	public List<Triangle> triangles;

	public Light[] lights;

	public RGB[][] image = new RGB[width][height];

	public float fovx = (float) 0.628;
	public float fovy = (float) 0.628;
	public RGB ambient_color = new RGB(0f, 0f, 0f);
	public RGB background_color = new RGB(0f, 0f, 0f);
	RGB black = new RGB(0.0f, 0.0f, 0.0f);
	public int y_angle_factor;
	public int x_angle_factor;

	public void profileRenderImage() {
		long end, start, time;

		renderImage(); // initialisiere Datenstrukturen, erster Lauf verfaelscht
						// sonst Messungen

		for (int procs = 1; procs < 6; procs++) {

			maxRec = procs - 1;
			System.out.print(procs);
			for (int i = 0; i < 10; i++) {
				start = System.currentTimeMillis();

				renderImage();

				end = System.currentTimeMillis();
				time = end - start;
				System.out.print(";" + time);
			}
			System.out.println("");
		}
	}

	Ray eye_ray = new Ray();
	double tan_fovx;
	double tan_fovy;

	public void renderImage() {
		tan_fovx = Math.tan(fovx);
		tan_fovy = Math.tan(fovy);
		for (int i = 0; i < width; i++) {
			futureList[i] = (Future) eservice.submit(new RaytraceTask(this, i));
		}

		for (int i = 0; i < width; i++) {
			try {
				RGB[] col = (RGB[]) futureList[i].get();
				for (int j = 0; j < height; j++)
					image[i][j] = col[j];
			} catch (InterruptedException e) {
			} catch (ExecutionException e) {
			}
		}
	}

	RGB rayTrace(Ray ray, int rec) {
		if (rec > maxRec)
			return black;
		IPoint ip = hitObject(ray); // (ray, p, n, triangle);
		if (ip.dist > IPoint.epsilon)
			return lighting(ray, ip, rec);
		else
			return black;
	}

	IPoint hitObject(Ray ray) {
		IPoint isect = new IPoint(null, null, -1);
		float idist = -1;
		for (Triangle t : triangles) {
			IPoint ip = ray.intersect(t);
			if (ip.dist != -1)
				if ((idist == -1) || (ip.dist < idist)) { // save that
															// intersection
					idist = ip.dist;
					isect.ipoint = ip.ipoint;
					isect.dist = ip.dist;
					isect.triangle = t;
				}
		}
		return isect; // return intersection point and normal
	}

	RGB lighting(Ray ray, IPoint ip, int rec) {
		Vec3D point = ip.ipoint;
		Triangle triangle = ip.triangle;
		RGB color = triangle.color.addColor(ambient_color, 1);
		Ray shadow_ray = new Ray();
		for (Light light : lights) {
			shadow_ray.start = point;
			shadow_ray.dir = light.position.minus(point).mult(-1);
			shadow_ray.dir.normalize();
			IPoint ip2 = hitObject(shadow_ray);
			if (ip2.dist < IPoint.epsilon) {
				float ratio = Math.max(0, shadow_ray.dir.dot(triangle.normal));
				color = color.addColor(light.color, ratio);
			}
		}
		Ray reflection = new Ray();
		// R = 2N(N*L)-L) L ausgehender Vektor
		Vec3D L = ray.dir.mult(-1);
		reflection.start = point;
		reflection.dir = triangle.normal.mult(2 * triangle.normal.dot(L)).minus(L);
		reflection.dir.normalize();
		RGB rcolor = rayTrace(reflection, rec + 1);
		float ratio = (float) Math.pow(Math.max(0, reflection.dir.dot(L)), triangle.shininess);
		color = color.addColor(rcolor, ratio);
		return (color);
	}

}

class RaytraceTask implements Callable {
	SDRaytracer tracer;
	int i;

	RaytraceTask(SDRaytracer t, int ii) {
		tracer = t;
		i = ii;
	}

	public RGB[] call() {
		RGB[] col = new RGB[tracer.height];
		for (int j = 0; j < tracer.height; j++) {
			tracer.image[i][j] = new RGB(0, 0, 0);
			for (int k = 0; k < tracer.rayPerPixel; k++) {
				double di = i + (Math.random() / 2 - 0.25);
				double dj = j + (Math.random() / 2 - 0.25);
				if (tracer.rayPerPixel == 1) {
					di = i;
					dj = j;
				}
				Ray eye_ray = new Ray();
				eye_ray.setStart(tracer.startX, tracer.startY, tracer.startZ); // ro
				eye_ray.setDir((float) (((0.5 + di) * tracer.tan_fovx * 2.0) / tracer.width - tracer.tan_fovx),
						(float) (((0.5 + dj) * tracer.tan_fovy * 2.0) / tracer.height - tracer.tan_fovy), (float) 1f); // rd
				eye_ray.normalize();
				col[j] = tracer.image[i][j].addColor(tracer.rayTrace(eye_ray, 0), 1.0f / tracer.rayPerPixel);
			}
		}
		return col;
	}
}
