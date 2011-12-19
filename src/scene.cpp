#include <Eigen/Dense>
#include <png++/png.hpp>
#include <vector>

#include "scene.h"

using namespace png;
using namespace std;
using namespace Eigen;

Scene::Scene() : background_color(0., 0., 0.), msaa(1) { }

Scene::Scene(GeometrySet& gs, Camera& c, vector<PointLight>& ls, int w, int h) 
: geom(gs), camera(c), lights(ls), background_color(0.f, 0.f, 0.f),
width(w), height(h), msaa(1) {}

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
      int pi = (i + .5) * width
        , pj = (j + .5) * height;

      if (msaa == 1) {
        Ray ray = camera.rayForH(i, j);
        Color pixel = colorAtRay(ray);
        img.set_pixel(pi, pj, pixel_for(pixel));
      } else {
        float r = 0, g = 0, b = 0;

        for (int k=0; k<msaa; k++) {
          float i_offset = ((float) rand() / RAND_MAX);
          i_offset *= 1. / (float) width;

          float j_offset = ((float) rand() / RAND_MAX);
          j_offset *= 1. / (float) height;

          Ray ray = camera.rayForH(i+i_offset, j+j_offset);
          Color pixel = colorAtRay(ray);
          r += pixel.r;
          g += pixel.g;
          b += pixel.b;
        }

        r /= msaa;
        g /= msaa;
        b /= msaa;

        img.set_pixel(pi, pj, pixel_for(Color(r, g, b)));
      }
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
