#ifndef __CRENDER_IMAGE_H__
#define __CRENDER_IMAGE_H__

#include "geometry.h"
#include <png++/png.hpp>

class Rgba {
  public:
    Rgba();
    Rgba(float r, float g, float b, float a);
    float r, g, b, a;
};

class Image {
  public:
    Image(int width, int height);
    void setPixel(int i, int j, const Color&);
    void writePng(const string& path) const;

  private:
    Rgba **pixels;
    int w, h;
};

#endif
