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
    const vector<PointLight>& lights, const GeometrySet& geom) const {
  if (material == NULL) {
    cerr << "Warning: intersection with geometry without material." << endl;
    return Color(0, 0, 0);
  }

  return material->evaluateAt(
      incident, location, normal(location), lights, geom);
}

GeometrySet::GeometrySet() : geom() {}

void GeometrySet::addGeometry(Geometry* g) {
  geom.push_back(g);
}

Intersection GeometrySet::intersect(const Ray& ray) const {
  uint i = 0;
  Intersection min = noIntersection();

  do {
    Intersection test = geom[i]->intersect(ray);
    if (test.intersects && (!min.intersects || min.s > test.s)) {
      min = test;
    }

    i++;
  } while (i < geom.size());

  return min;
}

int GeometrySet::size() const {
  return geom.size();
}

ostream& operator<<(ostream& stream, const GeometrySet& geom) {
  stream << "GeometrySet size=" << geom.size();
  return stream;
}
