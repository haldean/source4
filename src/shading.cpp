#include "shading.h"

ostream& operator<<(ostream& stream, const Color& color) {
  stream << "(color r:" << color.r << " g:" << color.g
    << " b:" << color.b << ")";
  return stream;
}

Color ColorMaterial::evaluateAt(
    const Ray& ray, const Vector3f& location, const Geometry* geom) const {
  return color;
}
