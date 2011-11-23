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

  Intersection min = geom[0].geom.intersect(ray);
  TexturedGeometry minGeometry = geom[0];

  for (vector<TexturedGeometry>::const_iterator iter = geom.begin() + 1;
       iter != geom.end(); iter++) {
    Intersection test = iter->geom.intersect(ray);
    if (!test.intersects) continue;
    if (!min.intersects || min.s > test.s) {
      min = test;
      minGeometry = *iter;
    }
  }

  if (min.intersects) {
    cout << "Intersection!" << endl;
    return minGeometry.evaluateAt(ray, min.location);
  }
  return background_color();
}

void render() {
  image<rgb_pixel> img(400, 400);
  vector<TexturedGeometry> geom;
  geom.push_back(TexturedGeometry(
        Sphere(100, Vector3f(100, 0, 0)),
        ColorMaterial(Color(1., 0., 0.))));
  for (int i=0; i<400; i++) {
    for (int j=0; j<400; j++) {
      Ray ray(Vector3f(0, j-200, i-200), Vector3f(1, 0, 0));
      img.set_pixel(i, j, pixel_for(color_at_ray(ray, geom)));
    }
  }
  img.write("test.png");
}

int main() {
  render();
}
