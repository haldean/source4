#ifndef __CRENDER_CAMERA_H__
#define __CRENDER_CAMERA_H__

#include <Eigen/Dense>
#include "geometry.h"

class Camera {
  public:
    Camera();
    Camera(
        const Ray& view, const Vector3f& up, const float focalLength,
        const float imageWidth, const float imageHeight);
    Ray rayForH(float hx, float hy);

  private:
    Vector3f location;
    Vector3f f, u, v;
};

#endif
