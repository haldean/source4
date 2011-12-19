#include "image.h"
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>

Image::Image(int width, int height) : w(width), h(height) {
  pixels.resizeErase(height, width);
}

void Image::setPixel(int i, int j, const Color& color) {
  pixels[j][i] = Rgba(color.r, color.g, color.b, 1);
}

void Image::writeExr(const string& path) const {
  RgbaOutputFile file(path.c_str(), w, h, WRITE_RGBA);
  file.setFrameBuffer(&(pixels[0][0]), 1, w);
  file.writePixels(h);
}

#ifdef WRITE_PNG
#include <png++/png.hpp>
using namespace png;

void Image::writePng(const string& path) const {
  image<rgb_pixel> output(w, h);
  for (int i=0; i<w; i++) {
    for (int j=0; j<h; j++) {
      Rgba pixel = pixels[j][i];
      output[j][i] = rgb_pixel(pixel.r * 255, pixel.g * 255, pixel.b * 255);
    }
  }
  output.write(path);
}
#endif
