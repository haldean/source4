#include "geometry.h"

Ray::Ray(const Vector3f& position, const Vector3f& direction)
: origin(position), dir(direction) {
  dir.normalize();
}

ostream& operator<<(ostream& stream, const Ray& ray) {
  stream << "ray ["
    << ray.origin[0] << ", "
    << ray.origin[1] << ", "
    << ray.origin[2]
    << " / "
    << ray.dir[0] << ", "
    << ray.dir[1] << ", "
    << ray.dir[2]
    << "]";
  return stream;
}

