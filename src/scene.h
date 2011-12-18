#ifndef __CRENDER_SCENE_H__
#define __CRENDER_SCENE_H__

#include <vector>
#include <ostream>

#include "geometry.h"
#include "camera.h"

using namespace std;

class Scene {
  public:
    Scene();
    Scene(GeometrySet&, Camera&, vector<PointLight>&, int, int);
    void render();

    GeometrySet geom;
    Camera camera;
    vector<PointLight> lights;

    Color background_color;

    int width;
    int height;

  private:
    Color colorAtRay(Ray&);
};

ostream& operator<<(ostream&, const Scene&);

#endif
