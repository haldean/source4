#include "parser.h"
#include "geometry.h"

#include <cstdio>

Material* currentMaterial = NULL;

void parseSphere(string& line, Scene& scene) {
  Vector3f center;
  float r;

  sscanf(line.c_str(), "s %f %f %f %f",
      &center[0], &center[1], &center[2], &r);

  Sphere* sphere = new Sphere(r, center);
  sphere->material = currentMaterial;
  scene.geom.addGeometry(sphere);
}

void parseTriangle(string& line, Scene& scene) {
  Vector3f p1, p2, p3;
  sscanf(line.c_str(), "t %f %f %f %f %f %f %f %f %f",
      &p1[0], &p1[1], &p1[2],
      &p2[0], &p2[1], &p2[2],
      &p3[0], &p3[1], &p3[2]);

  Triangle* triangle = new Triangle(p1, p2, p3);
  triangle->material = currentMaterial;
  scene.geom.addGeometry(triangle);
}

void parsePlane(string& line, Scene& scene) {
  Vector3f n;
  float d;

  sscanf(line.c_str(), "p %f %f %f %f", &n[0], &n[1], &n[2], &d);
  Plane* plane = new Plane(n, d);
  plane->material = currentMaterial;
  scene.geom.addGeometry(plane);
}

void parseCamera(string& line, Scene& scene) {
  Vector3f up(0, 1, 0), direction, position;
  float focalLength, iw, ih;

  sscanf(line.c_str(), "c %f %f %f %f %f %f %f %f %f %d %d",
      &position[0], &position[1], &position[2],
      &direction[0], &direction[1], &direction[2],
      &focalLength, &iw, &ih, &scene.width, &scene.height);

  scene.camera = Camera(Ray(position, direction), up, focalLength, iw, ih);
}

void parseLight(string& line, Scene& scene) {
  if (line[2] == 'a') {
    // Simulate ambient lights with a light with only an ambient contribution
    // whose position is at the origin.
    Color amb(0, 0, 0);
    sscanf(line.c_str(), "l a %f %f %f", &(amb.r), &(amb.g), &(amb.b));
    scene.lights.push_back(PointLight(
          Color(0, 0, 0), Color(0, 0, 0), amb, Vector3f(0, 0, 0)));
  } else if (line[2] == 'p') {
    Vector3f location;
    Color color(0, 0, 0);

    sscanf(line.c_str(), "l p %f %f %f %f %f %f",
        &location[0], &location[1], &location[2], &color.r, &color.g, &color.b);
    scene.lights.push_back(PointLight(color, color, Color(0, 0, 0), location));
  } else {
    cerr << "directional lights unsupported" << endl;
  }
}

void parseMaterial(string& line) {
  Color *diff = new Color(0, 0, 0)
      , *spec = new Color(0, 0, 0)
      , *idealspec = new Color(0, 0, 0);
  float phexp;

  sscanf(line.c_str(), "m %f %f %f %f %f %f %f %f %f %f",
      &(diff->r), &(diff->g), &(diff->b), &(spec->r), &(spec->g), &(spec->b),
      &phexp, &(idealspec->r), &(idealspec->g), &(idealspec->b));

  currentMaterial = 
    new PhongMaterial(*spec, *diff, *diff, *idealspec, phexp);
}

void parseLine(string& line, Scene& scene) {
  if (line[0] == '/' || line.length() == 0) return;

  string cmd = line.substr(0, line.find(' '));
  if (cmd == "s") {
    parseSphere(line, scene);
  } else if (cmd == "c") {
    parseCamera(line, scene);
  } else if (cmd == "t") {
    parseTriangle(line, scene);
  } else if (cmd == "m") {
    parseMaterial(line);
  } else if (cmd == "l") {
    parseLight(line, scene);
  } else if (cmd == "o") {
    cout << "option unsupported" << endl;
  } else {
    cout << "unrecognized command: " << cmd << endl;
  }
}

Scene parseStream(istream& in) {
  Scene scene;

  string line;
  while (in.good()) {
    getline(in, line);
    parseLine(line, scene);
  }

  cout << endl << scene << endl;
  return scene;
}
