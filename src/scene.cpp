#include <Eigen/Dense>
#include <png++/png.hpp>
#include <vector>

#include "scene.h"

using namespace png;
using namespace std;
using namespace Eigen;

Scene::Scene(GeometrySet& gs, Camera& c, vector<Light>& ls, int w, int h) 
: geom(gs), camera(c), lights(ls), width(w), height(h) {
  background_color(0, 0, 0);
}

rgb_pixel pixel_for(Color c) {
  return rgb_pixel(c.r * 255, c.g * 255, c.b * 255);
}

Color Scene::colorAtRay(Ray& ray) {
  if (geom.size() == 0) {
    return background_color;
  }

  uint i = 0;
  bool intersects;
  Intersection min;

  do {
    Intersection test = geom.intersect(ray);
    if (test.intersects && (minindex == -1 || min.s > test.s)) {
      min = test;
      intersects = true;
    }
    i++;
  } while (i < geom.size());

  if (intersects) {
    return min.geom->evaluateAt(ray, min.location);
  }

  return background_color;
}

void Scene::render(int width, int height) {
  for (float i = -0.5; i <= 0.5; i += 1. / (float) width) {
    for (float j = -0.5; j <= 0.5; j += 1. / (float) height) {
      Ray ray = camera.rayForH(i, j);
      Color pixel = color_at_ray(ray, geom);
      img.set_pixel((i + 0.5) * width, (j + 0.5) * height, pixel_for(pixel));
    }
  }

  img.write("test.png");
}

int main() {
  render(600, 300);
}
