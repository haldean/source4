#include "camera.h"

Camera::Camera() {}

Camera::Camera(
    const Ray& view, const Vector3f& upVector, const float focalLength,
    const float imageWidth, const float imageHeight) {
  Vector3f up = upVector / upVector.norm();

  location = view.origin;
  f = view.dir * focalLength;
  u = up * imageHeight;
  v = imageWidth * up.cross(view.dir);
}

Ray Camera::rayForH(float hx, float hy) {
  return Ray(location, f - (hx * v) - (hy * u));
}
