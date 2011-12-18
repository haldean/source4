#include <cassert>
#include <Eigen/Dense>
#include <iostream>

#include "geometry.h"

using namespace std;

void test_sphere_intersection() {
  cout << "test_sphere_intersection: ";

  Sphere sphere(1, Vector3f(4, 0, 0));

  { /* Two intersections */
    Ray ray(Vector3f(0, 0, 0), Vector3f(1, 0, 0));
    Intersection result = sphere.intersect(ray); 
    assert(result.intersects);
    assert(result.location == Vector3f(3, 0, 0));
  }

  { /* One intersection */
    Ray ray(Vector3f(0, 1, 0), Vector3f(1, 0, 0));
    Intersection result = sphere.intersect(ray); 
    assert(result.intersects);
    assert(result.location == Vector3f(4, 1, 0));
  }

  { /* No intersections */
    Ray ray(Vector3f(0, 0, 0), Vector3f(1, 1, 0));
    Intersection result = sphere.intersect(ray); 
    assert(!result.intersects);
  }

  cout << "OK.\n";
}

#ifdef TEST
int main() {
  test_sphere_intersection();
}
#endif
