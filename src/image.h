#ifndef __CRENDER_IMAGE_H__
#define __CRENDER_IMAGE_H__

#include <ImfArray.h>
#include <ImfRgbaFile.h>
#include "geometry.h"

using namespace Imf;

class Image {
  public:
    Image(int width, int height);
    void setPixel(int i, int j, const Color&);
    void writeExr(const string& path) const;
#ifdef WRITE_PNG
    void writePng(const string& path) const;
#endif

  private:
    Array2D<Rgba> pixels;
    int w, h;
};

#endif
