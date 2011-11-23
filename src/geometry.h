#ifndef __CRENDER_GEOMETRY_H__
#define __CRENDER_GEOMETRY_H__

#include <Eigen/Dense>

using namespace Eigen;

struct Intersection {
  bool intersects;
  float s;
  Vector3f location;
};

class Ray {
  public:
    Ray(const Vector3f& origin, const Vector3f& direction)
      : origin(origin), dir(direction) {}
    Ray(const Ray& other)
      : origin(other.origin), dir(other.dir) {}
    ~Ray() {}

    Vector3f origin;
    Vector3f dir;
};

class Geometry {
  public:
    virtual Intersection intersect(const Ray& ray) const {
      Intersection i;
      i.intersects = false;
      return i;
    };
};

class Sphere : public Geometry {
  public:
    Sphere(const float r, const Vector3f& center);
    Sphere(const Sphere&);
    ~Sphere();

    Intersection intersect(const Ray& ray) const;

  private:
    const float _r;
    const Vector3f _center;
};

#endif
