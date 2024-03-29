#include "geometry.h"

Sphere::Sphere(const float r, const Vector3f& center)
  : _r(r), _center(center) {}

Sphere::Sphere(const Sphere& other)
  : _r(other._r), _center(other._center) {}

Sphere::~Sphere() {}

Vector3f Sphere::normal(const Vector3f& location) const {
  Vector3f normal = location - _center;
  normal.normalize();
  return normal;
}

Intersection Sphere::intersect(const Ray& ray) const {
  Intersection result;
  result.geom = this;

  float a = ray.dir.dot(ray.dir)
    , b = 2 * (ray.dir.dot(ray.origin) - _center.dot(ray.dir))
    , c = ray.origin.dot(ray.origin) + _center.dot(_center)
          - 2 * ray.origin.dot(_center) - _r * _r;

  float disc = b * b - 4 * a * c;
  /* Both roots are complex */
  if (disc < 0) {
    result.intersects = false;
    return result;
  }

  result.intersects = true;

  float s1 = (-b + sqrt(disc)) / (2 * a);
  float s2 = (-b - sqrt(disc)) / (2 * a);

  if (s1 <= s2 && s1 > 0) {
    result.s = s1;
    result.location = ray.origin + s1 * ray.dir;
  } else if (s2 > 0) {
    result.s = s2;
    result.location = ray.origin + s2 * ray.dir;
  } else {
    result.intersects = false;
  }

  return result;
}
