#include "geometry.h"

Triangle::Triangle(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3)
: p1(p1), p2(p2), p3(p3), n1((p2-p1).cross(p3-p1)), n2(n1), n3(n1) {}

Triangle::Triangle(
    const Vector3f& p1, const Vector3f& p2, const Vector3f& p3,
    const Vector3f& n1, const Vector3f& n2, const Vector3f& n3)
: p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3) {}

Triangle::Triangle(const Triangle& other) 
: p1(other.p1), p2(other.p2), p3(other.p3),
  n1(other.n1), n2(other.n2), n3(other.n3) {}

Triangle::~Triangle() {}

/*
 * Uses the method put forward in "Fast, Minimum Storage Ray/Triangle
 * Intersection", Moller and Trumbone, '96
 */
Intersection Triangle::intersect(const Ray& ray) const {
  Intersection result;
  result.intersects = false;
  result.geom = this;

  Vector3f e1 = p2 - p1
         , e2 = p3 - p1
         , t  = ray.origin - p1
         , p  = ray.dir.cross(e2)
         , q  = t.cross(e1);

  float det = e1.dot(p);
  if (det > -1e-7 && det < 1e-7) {
    return result;
  }

  float scale = 1 / det;
  float u = scale * p.dot(t);
  if (u < 0 || u > 1) {
    return result;
  }

  float v = scale * q.dot(ray.dir);
  if (v < 0 || u + v > 1) {
    return result;
  }

  float s = scale * q.dot(e2);
  if (s < 0) {
    return result;
  }

  result.intersects = true;
  result.location = ray.origin + s * ray.dir;
  result.s = s;
  return result;
}

Vector3f Triangle::normal(const Vector3f& p) const {
  /* If the triangle doesn't have per-vertex normals, it's a big waste of time
   * to interpolate between three equal values. */
  if (n1 == n2 && n2 == n3) {
    return n1;
  }

  float triarea = (p2-p1).cross(p3-p1).norm()
      , lambda1 = (p2-p).cross(p3-p).norm() / triarea
      , lambda2 = (p1-p).cross(p3-p).norm() / triarea
      , lambda3 = (p1-p).cross(p2-p).norm() / triarea;

  assert(0 <= lambda1 && lambda1 <= 1);
  assert(0 <= lambda2 && lambda2 <= 1);
  assert(0 <= lambda3 && lambda3 <= 1);

  return lambda1 * n1 + lambda2 * n2 + lambda3 * n3;
}

ostream& operator<<(ostream& stream, const Triangle& tri) {
  cout << "triangle "
    << tri.p1[0] << ", " << tri.p1[1] << ", " << tri.p1[2] << " / "
    << tri.p2[0] << ", " << tri.p2[1] << ", " << tri.p2[2] << " / "
    << tri.p3[0] << ", " << tri.p3[1] << ", " << tri.p3[2] << endl;
  return stream;
}
