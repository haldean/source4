#include "shading.h"

Color ColorMaterial::evaluateAt(
    const Ray& ray, const Vector3f& location, const Geometry* geom) const {
  return color;
}
