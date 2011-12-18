#include "geometry.h"

Triangle::Triangle(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3)
: p1(p1), p2(p2), p3(p3) {
  norm = (p2-p1).cross(p3-p1);
}

Triangle::Triangle(const Triangle& other) 
: p1(other.p1), p2(other.p2), p3(other.p3), norm(other.norm) {}

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
  return result;
}

Vector3f Triangle::normal(const Vector3f& location) const {
  return norm;
}
