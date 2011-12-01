#ifndef __CRENDER_CAMERA_H__
#define __CRENDER_CAMERA_H__

#include "geometry.h"

class Camera {
  public:
    Camera(
        const Ray& view, const float focalLength,
        const float imageWidth, const float imageHeight,
        const float pixelWidth, const float pixelHeight);
    Ray rayForH(float hx, float hy);

  private:
    Ray& viewRay;
    float focal;
    float iw, ih;
    float pw, ph;
}
