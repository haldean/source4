#include "geometry.h"

Plane::Plane(const Vector3f& location, const Vector3f& normal)
: location(location), norm(normal) {}

Plane::Plane(const Plane& other)
: location(other.location), norm(other.norm) {}

Plane::~Plane() {}

Intersection Plane::intersect(const Ray& ray) const {
  Intersection result;
  result.geom = this;

  float s =
    (location.dot(norm) - ray.origin.dot(norm)) / ray.dir.dot(norm);

  if (s > 0) {
    result.intersects = true;
    result.location = ray.origin + s * ray.dir;
  } else {
    result.intersects = false;
  }

  return result;
}

Vector3f Plane::normal(const Vector3f& location) const {
  return norm;
}
