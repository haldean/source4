#ifndef __CRENDER_RAY_H__
#define __CRENDER_RAY_H__

#include <Eigen/Core>

using namespace Eigen;

class Ray {
  public:
    Ray(const Vector3f& origin, const Vector3f& direction);
    Ray(const Ray& other)
      : origin(other.origin), dir(other.dir) {}
    ~Ray() {}

    Vector3f origin;
    Vector3f dir;
};

#endif
