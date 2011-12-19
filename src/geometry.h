#ifndef __CRENDER_GEOMETRY_H__
#define __CRENDER_GEOMETRY_H__

#include <Eigen/Core>
#include <Eigen/Dense>
#include <ostream>
#include <iostream>
#include <vector>

#include "ray.h"

using namespace Eigen;
using namespace std;

ostream& operator<<(ostream&, const Ray&);

struct Intersection;
class Geometry;
class Scene;

struct Intersection {
  bool intersects;
  float s;
  Vector3f location;
  const Geometry* geom;
};

class GeometrySet {
  public:
    GeometrySet();
    void addGeometry(Geometry*);
    Intersection intersect(const Ray&) const;
    Intersection intersect(const Ray&, const Geometry* ignore) const;
    Intersection intersect(
        const Ray&, const float& smax, const Geometry* ignore) const;
    int size() const;
    vector<Geometry*> geom;
};

ostream& operator<<(ostream&, const GeometrySet&);

class Color {
  public:
    Color(float red, float green, float blue)
      : r(red), g(green), b(blue) {}
    bool zero() const;
    void operator+=(const Color&);
    Color operator*(double) const;
    Color operator*(const Color&) const;
    float r, g, b;
};

ostream& operator<<(ostream&, const Color&);

class PointLight {
  public:
    PointLight(Color spec, Color diff, Color amb, Vector3f loc)
      : specular(spec), diffuse(diff), ambient(amb),
        location(loc), directional(false) {}
    PointLight(Color spec, Color diff, Color amb, Vector3f loc, bool dir)
      : specular(spec), diffuse(diff), ambient(amb),
        location(loc), directional(dir) {}

    Color specular, diffuse, ambient;
    float phong;
    Vector3f location;
    bool directional;
};

class Material {
  public:
    virtual Color evaluateAt(
        const Ray& ray, const Vector3f& location, const Vector3f& normal,
        const Scene&, const Geometry* primitive, const int depth) const = 0;
};

class ColorMaterial : public Material {
  public:
    ColorMaterial(const Color& c) : color(c) {}
    Color evaluateAt(
        const Ray& ray, const Vector3f& location, const Vector3f& normal,
        const Scene&, const Geometry* primitive, const int depth) const;
  private:
    Color color;
};

class PhongMaterial : public Material {
  public:
    PhongMaterial(
        const Color& specular, const Color& diffuse,
        const Color& ambient, const Color& idealspec,
        const double phongExp);
    Color evaluateAt(
        const Ray& ray, const Vector3f& location, const Vector3f& normal,
        const Scene&, const Geometry* primitive, const int depth) const;

    const Color& specular;
    const Color& diffuse;
    const Color& ambient;
    const Color& idealspec;
    const double phongExp;
};

ostream& operator<<(ostream&, const PhongMaterial&);

class Geometry {
  public:
    virtual Intersection intersect(const Ray& ray) const = 0;
    virtual Vector3f normal(const Vector3f& location) const = 0;

    Color colorAt(
        const Vector3f& location, const Ray& incident,
        const Scene& scene, int depth) const;
    Material* material;
};

class Sphere : public Geometry {
  public:
    Sphere(const float r, const Vector3f& center);
    Sphere(const Sphere&);
    ~Sphere();

    Intersection intersect(const Ray& ray) const;
    Vector3f normal(const Vector3f& location) const;

  private:
    const float _r;
    const Vector3f _center;
};

class Triangle : public Geometry {
  public:
    Triangle(
        const Vector3f& p1, const Vector3f& p2, const Vector3f& p3);
    Triangle(
        const Vector3f& p1, const Vector3f& p2, const Vector3f& p3,
        const Vector3f& n1, const Vector3f& n2, const Vector3f& n3);
    Triangle(const Triangle&);
    ~Triangle();

    Intersection intersect(const Ray& ray) const;
    Vector3f normal(const Vector3f& location) const;

    friend ostream& operator<<(ostream&, const Triangle&);
  private:
    const Vector3f p1, p2, p3;
    const Vector3f n1, n2, n3;
};

ostream& operator<<(ostream&, const Triangle&);

class Plane : public Geometry {
  public:
    Plane(const Vector3f& normal, const float& d);
    Plane(const Plane&);
    ~Plane();

    Intersection intersect(const Ray& ray) const;
    Vector3f normal(const Vector3f& location) const;

  private:
    const Vector3f norm;
    const float& d;
};

#endif
