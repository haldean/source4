#include "geometry.h"

Plane::Plane(const Vector3f& normal, const float& d) : norm(normal), d(d) {}

Plane::Plane(const Plane& other) : norm(other.norm), d(d) {}

Plane::~Plane() {}

Intersection Plane::intersect(const Ray& ray) const {
  Intersection result;
  result.geom = this;

  float s = (-d - ray.origin.dot(norm)) / ray.dir.dot(norm);

  if (s > 0) {
    result.s = s;
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
