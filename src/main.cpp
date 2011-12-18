#include "parser.h"
#include "scene.h"
#include <fstream>

int main(int argc, char* argv[]) {
  return scene_test();

  if (argc != 2) {
    cerr << "Must provide input file" << endl;
    return -1;
  }

  ifstream in(argv[1]);
  Scene scene = parseStream(in);
  scene.render();
  return 0;
}
