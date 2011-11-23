#include "geometry.h"

Sphere::Sphere(const float r, const Vector3f& center)
  : _r(r), _center(center) {}

Sphere::Sphere(const Sphere& other)
  : _r(other._r), _center(other._center) {}

Sphere::~Sphere() {}

Intersection Sphere::intersect(const Ray& ray) const {
  Intersection result;

  Vector3f dir = ray.dir, pos = ray.origin;

  float a = dir.dot(dir)
    , b = 2 * (dir.dot(pos) - _center.dot(dir))
    , c = pos.dot(pos) + _center.dot(_center) - 2 * pos.dot(_center) - _r * _r;

  float disc = b * b - 4 * a * c;
  /* Both roots are complex */
  if (disc < 0) {
    result.intersects = false;
    return result;
  }

  result.intersects = true;

  float s1 = (-b + sqrt(disc)) / (2 * a);
  float s2 = (-b - sqrt(disc)) / (2 * a);

  if (s1 <= s2) {
    result.location = pos + s1 * dir;
  } else {
    result.location = pos + s2 * dir;
  }

  return result;
}
