#ifndef __CRENDER_SCENE_H__
#define __CRENDER_SCENE_H__

#include <vector>

#include "geometry.h"
#include "camera.h"
#include "shading.h"

using namespace std;

class Scene {
  public:
    Scene(GeometrySet&, Camera&, vector<Light>&, int, int);
    void render();

  private:
    Color colorAtRay(Ray&);

    GeometrySet geom;
    Camera camera;
    vector<Light> lights;

    Color background_color;

    int width;
    int height;
}

#endif
