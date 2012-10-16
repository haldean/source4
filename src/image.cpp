#include "image.h"

using namespace png;

Rgba::Rgba() : r(0), g(0), b(0), a(0) {}
Rgba::Rgba(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

Image::Image(int width, int height) : w(width), h(height) {
  pixels = new Rgba*[height];
  for (int j = 0; j < height; j++) {
    pixels[j] = new Rgba[width];
    for (int i = 0; i < width; i++) {
      pixels[j][i] = Rgba(0, 0, 0, 1);
    }
  }
}

void Image::setPixel(int i, int j, const Color& color) {
  pixels[j][i] = Rgba(color.r, color.g, color.b, 1);
}

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
