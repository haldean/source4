#include <Eigen/Dense>
#include <png++/png.hpp>
#include <vector>

#include "scene.h"

using namespace png;
using namespace std;
using namespace Eigen;

Scene::Scene() : background_color(0., 0., 0.) { }

Scene::Scene(GeometrySet& gs, Camera& c, vector<PointLight>& ls, int w, int h) 
  : geom(gs), camera(c), lights(ls), background_color(0.f, 0.f, 0.f),
    width(w), height(h) {}

rgb_pixel pixel_for(Color c) {
  return rgb_pixel(c.r * 255, c.g * 255, c.b * 255);
}

Color Scene::colorAtRay(Ray& ray) {
  if (geom.size() == 0) {
    return background_color;
  }

  Intersection pt = geom.intersect(ray);
  if (pt.intersects) {
    return pt.geom->colorAt(pt.location, ray, lights, geom);
  }
  return background_color;
}

void Scene::render() {
  image<rgb_pixel> img(width, height);

  for (float i = -0.5; i <= 0.5; i += 1. / (float) width) {
    for (float j = -0.5; j <= 0.5; j += 1. / (float) height) {
      Ray ray = camera.rayForH(i, j);
      Color pixel = colorAtRay(ray);
      img.set_pixel((i + 0.5) * width, (j + 0.5) * height, pixel_for(pixel));
    }
  }

  img.write("test.png");
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
  Sphere* sphere = new Sphere(1, Vector3f(5, 0, 0));
  PhongMaterial* red = new PhongMaterial(
      Color(1, 1, 1), Color(.7, 0, 0), Color(.7, .0, .0), 8);
  sphere->material = red;

  GeometrySet geom;
  geom.addGeometry(sphere);

  Camera c(
      Ray(Vector3f(0, 0, 0), Vector3f(1, 0, 0)),
      Vector3f(0, 1, 0), 1, 1, 1);

  vector<PointLight> lights;
  lights.push_back(
      PointLight(
        Color(0.5, 0.5, 0.5), Color(0.5, 0.5, 0.5),
        Color(.3, .3, .3), Vector3f(0, 0, 5)));

  Scene(geom, c, lights, 600, 600).render();
}
#endif
