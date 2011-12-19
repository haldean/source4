#include "geometry.h"

Intersection noIntersection() {
  Intersection min;

  min.intersects = false;
  min.geom = NULL;
  min.s = -1;
  min.location = Vector3f(0, 0, 0);

  return min;
}

Color Geometry::colorAt(
    const Vector3f& location, const Ray& incident,
    const Scene& scene, int depth) const {
  if (material == NULL) {
    cerr << "Warning: intersection with geometry without material." << endl;
    return Color(0, 0, 0);
  }

  return material->evaluateAt(
      incident, location, normal(location), scene, this, depth);
}

GeometrySet::GeometrySet() : geom() {}

void GeometrySet::addGeometry(Geometry* g) {
  geom.push_back(g);
}

Intersection GeometrySet::intersect(
    const Ray& ray, const Geometry* ignore) const {
  uint i = 0;
  Intersection min = noIntersection();

  do {
    if (geom[i] == ignore) {
      i++;
      continue;
    }

    Intersection test = geom[i]->intersect(ray);
    if (test.intersects && (!min.intersects || min.s > test.s)) {
      min = test;
    }

    i++;
  } while (i < geom.size());

  return min;
}

Intersection GeometrySet::intersect(const Ray& ray) const {
  return intersect(ray, NULL);
}

Intersection GeometrySet::intersect(
    const Ray& ray, const float& smax, const Geometry* ignore) const {
  Intersection result = intersect(ray, ignore);
  result.intersects &= result.s < smax;
  return result;
}

int GeometrySet::size() const {
  return geom.size();
}

ostream& operator<<(ostream& stream, const GeometrySet& geom) {
  stream << "GeometrySet size=" << geom.size();
  return stream;
}
