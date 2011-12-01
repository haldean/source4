#ifndef __CRENDER_SHADING_H__
#define __CRENDER_SHADING_H__

#include "geometry.h"

class Color {
  public:
    float r, g, b;
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

class Material {
  public:
    virtual Color evaluateAt(
        const Ray& ray, const Vector3f& location,
        const Geometry* geom) const = 0;
};

class ColorMaterial : public Material {
  public:
    ColorMaterial(const Color& c) : color(c) {}
    Color evaluateAt(
        const Ray& ray, const Vector3f& location, const Geometry* geom) const;
    Color color;
};

class TexturedGeometry {
  public:
    Geometry* geom;
    Material* material;

    TexturedGeometry(Geometry* geom, Material* mat)
      : geom(geom), material(mat) {}
    Color evaluateAt(const Ray& ray, const Vector3f& location) const {
      return material->evaluateAt(ray, location, geom);
    }
};

#endif
