#include <Eigen/Dense>
#include <png++/png.hpp>
#include <vector>

#include "shading.h"
#include "geometry.h"

using namespace png;
using namespace std;
using namespace Eigen;

Color background_color() {
  return Color(0, 0, 0);
}

rgb_pixel pixel_for(Color c) {
  return rgb_pixel(c.r * 255, c.g * 255, c.b * 255);
}

Color color_at_ray(Ray& ray, vector<TexturedGeometry>& geom) {
  if (geom.size() == 0) {
    return background_color();
  }

  Intersection min = geom[0].geom->intersect(ray);
  TexturedGeometry minGeometry = geom[0];

  for (uint i=1; i<geom.size(); i++) {
    Intersection test = geom[i].geom->intersect(ray);
    if (!test.intersects) continue;
    if (!min.intersects || min.s > test.s) {
      min = test;
      minGeometry = geom[i];
    }
  }

  if (min.intersects) {
    return minGeometry.evaluateAt(ray, min.location);
  }
  return background_color();
}

void render(int width, int height) {
  image<rgb_pixel> img(width, height);
  vector<TexturedGeometry> geom;

  {
    Sphere* s = new Sphere(1, Vector3f(3, 0, 0));
    ColorMaterial* cm = new ColorMaterial(Color(1., 0., 0.));
    geom.push_back(TexturedGeometry(s, cm));
  }

  for (float i=0; i<1; i += 1. / (float) width) {
    for (float j=0; j<1; j += 1. / (float) height) {
      Ray ray(Vector3f(0, 0, 0), Vector3f(1, i, j));
      img.set_pixel(i, j, pixel_for(color_at_ray(ray, geom)));
    }
  }
  img.write("test.png");
}

int main() {
  render(600, 300);
}
