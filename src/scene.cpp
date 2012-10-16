#include <Eigen/Dense>
#include <vector>

#include "scene.h"
#include "image.h"

using namespace std;
using namespace Eigen;

/* Taken from http://stackoverflow.com/questions/686353/c-random-float */
float randFloat() {
  return (float)(((rand() << 15) + rand()) & ((1 << 24) - 1)) / (1 << 24);
}

Scene::Scene() : background_color(0., 0., 0.) {
  setDefaults();
}

Scene::Scene(GeometrySet& gs, Camera& c, vector<PointLight>& ls, int w, int h) 
: geom(gs), camera(c), lights(ls), background_color(0.f, 0.f, 0.f),
width(w), height(h) {
  setDefaults();
}

void Scene::setDefaults() {
  msaa = 1;
  dofSamples = 0;
  aperture = .5;
  focaldist = 15;
}

Color Scene::colorAtRay(
    Ray& ray, const Geometry* ignore, const int depth) const {
  if (geom.size() == 0) {
    return background_color;
  }

  Intersection pt = geom.intersect(ray, ignore);
  if (pt.intersects) {
    return pt.geom->colorAt(pt.location, ray, *this, depth);
  }
  return background_color;
}

Color Scene::colorAtRay(Ray& ray) const {
  return colorAtRay(ray, NULL, 0);
}

Color Scene::colorAtRayDof(Ray& ray) const {
  if (dofSamples == 0) {
    return colorAtRay(ray);
  }

  float r = 0, g = 0, b = 0;
  Vector3f focalpt = ray.origin + focaldist * ray.dir;
  for (int i=0; i<dofSamples; i++) {
    float x_offset = aperture * randFloat();
    float y_offset = aperture * randFloat();
    float z_offset = aperture * randFloat();

    Vector3f origin = ray.origin + Vector3f(x_offset, y_offset, z_offset);
    Ray testRay(origin, focalpt - origin);
    Color c = colorAtRay(testRay);
    r += c.r; g += c.g; b += c.b;
  }

  Color result(
      r / (float) dofSamples,
      g / (float) dofSamples,
      b / (float) dofSamples);
  return result;
}

void Scene::render() {
  Image img(width, height);

  for (float i = -0.5; i <= 0.5; i += 1. / (float) width) {
    for (float j = -0.5; j <= 0.5; j += 1. / (float) height) {
      int pi = (i + .5) * width
        , pj = (j + .5) * height;

      if (msaa == 1) {
        Ray ray = camera.rayForH(i, j);
        Color pixel = colorAtRayDof(ray);
        img.setPixel(pi, pj, pixel);
      } else {
        float r = 0, g = 0, b = 0;

        for (int k=0; k<msaa; k++) {
          float i_offset = ((float) rand() / RAND_MAX);
          i_offset *= .5 / (float) width;

          float j_offset = ((float) rand() / RAND_MAX);
          j_offset *= .5 / (float) height;

          Ray ray = camera.rayForH(i+i_offset, j+j_offset);
          Color pixel = colorAtRayDof(ray);
          r += pixel.r;
          g += pixel.g;
          b += pixel.b;
        }

        r /= msaa;
        g /= msaa;
        b /= msaa;

        img.setPixel(pi, pj, Color(r, g, b));
      }
    }
  }

  img.writePng("test.png");
}

ostream& operator<<(ostream& stream, const Scene& scene) {
  stream << "scene " << scene.width << "x" << scene.height << endl;
  stream << scene.geom << endl;
  //stream << scene.camera << endl;
  //stream << scene.lights << endl;
  //stream << scene.background_color << endl;
  return stream;
}

#ifdef SCENE_TEST
int main() {
#else
int scene_test() {
#endif
  Sphere* sphere = new Sphere(1, Vector3f(5, -1, -1));
  Sphere* sphere2 = new Sphere(.5, Vector3f(4, 0, 0));
  Triangle* triangle = new Triangle(
      Vector3f(5, 0, 0), Vector3f(5, 1, 0), Vector3f(5, 0, 1));
  Plane* plane = new Plane(Vector3f(-1, 0, 0), 8);

  PhongMaterial* red = new PhongMaterial(
      Color(1, 1, 1), Color(.7, 0, 0), Color(.7, 0, 0), Color(0, 0, 0), 8);
  PhongMaterial* green = new PhongMaterial(
      Color(1, 1, 1), Color(0, .7, 0), Color(0, .7, 0), Color(0, 0, 0), 8);

  sphere->material = red;
  sphere2->material = green;
  triangle->material = red;
  plane->material = green;

  GeometrySet geom;
  geom.addGeometry(sphere);
  geom.addGeometry(sphere2);
  geom.addGeometry(triangle);
  geom.addGeometry(plane);

  Camera c(
      Ray(Vector3f(0, 0, 0), Vector3f(1, 0, 0)),
      Vector3f(0, 1, 0), 1, 1, 1);

  vector<PointLight> lights;
  lights.push_back(
      PointLight(
        Color(0.5, 0.5, 0.5), Color(0.5, 0.5, 0.5),
        Color(.3, .3, .3), Vector3f(0, 0, 5)));

  Scene scene(geom, c, lights, 600, 600);
  scene.msaa = 16;
  scene.render();
  return 0;
}
