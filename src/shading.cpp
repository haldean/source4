#include "geometry.h"

ostream& operator<<(ostream& stream, const Color& color) {
  stream << "(color r:" << color.r << " g:" << color.g
    << " b:" << color.b << ")";
  return stream;
}

ostream& operator<<(ostream& stream, const PhongMaterial& mat) {
  stream << "spec: " << mat.specular << endl
    << "diff: " << mat.diffuse << endl
    << "ideal: " << mat.idealspec << endl
    << "exp: " << mat.phongExp << endl;
  return stream;
}

bool Color::zero() const {
  return r == 0 && g == 0 && b == 0;
}

void Color::operator+=(const Color& other) {
  r += other.r;
  g += other.g;
  b += other.b;

  r = r < 0 ? 0 : r > 1 ? 1 : r;
  g = g < 0 ? 0 : g > 1 ? 1 : g;
  b = b < 0 ? 0 : b > 1 ? 1 : b;
}

Color Color::operator*(const Color& other) const {
  Color result(0, 0, 0);
  result.r = r * other.r;
  result.g = g * other.g;
  result.b = b * other.b;
  return result;
}

Color Color::operator*(double scale) const {
  Color result(0, 0, 0);
  result.r = r * scale;
  result.g = g * scale;
  result.b = b * scale;
  return result;
}

Color ColorMaterial::evaluateAt(
    const Ray& ray, const Vector3f& location,
    const Vector3f& normal, const vector<PointLight>& lights,
    const GeometrySet& geom) const {
  return color;
}

PhongMaterial::PhongMaterial(
    const Color& specular, const Color& diffuse, const Color& ambient,
    const Color& idealspec, const double phongExp)
  : specular(specular), diffuse(diffuse), ambient(ambient),
    idealspec(idealspec), phongExp(phongExp) {};

Color PhongMaterial::evaluateAt(
    const Ray& ray, const Vector3f& location,
    const Vector3f& n_hat, const vector<PointLight>& lights,
    const GeometrySet& geom) const {
  Color result(0, 0, 0);

  for (vector<PointLight>::const_iterator light = lights.begin();
       light != lights.end(); light++) {
    result += ambient * light->ambient;

    if (!light->diffuse.zero() || !light->ambient.zero()) {
      Vector3f l_hat = light->location - location;
      l_hat.normalize();
      double ln = l_hat.dot(n_hat);
      result += (diffuse * light->diffuse) * max(0., ln);

      if (!light->specular.zero()) {
        Vector3f r_hat = 2 * l_hat.dot(n_hat) * n_hat - l_hat;
        r_hat.normalize();
        double rd = r_hat.dot(-ray.dir);
        result += (specular * light->specular) * pow(max(0., rd), phongExp);
      }
    }
  }

  return result;
}
