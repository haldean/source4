#include <png++/png.hpp>

using namespace png;

int test_image() {
  image<rgb_pixel> img(400, 400);

  for (int i=0; i<400; i++) {
    for (int j=0; j<400; j++) {
      img.set_pixel(i, j, rgb_pixel(0, 255, 0));
    }
  }

  img.write("test.png");
  return 0;
}
