#include "Image.hpp"
#include <vector>

namespace prog
{
  Image::Image(int w, int h, const Color &fill){
    height_ = h;
    width_ = w;
    std::vector<std::vector <Color>> pixel_matrix(height_, std::vector<Color>(width_));
    // Lines of the matrix represent the height of the image
    // while the columns repesent the width
  }
  Image::~Image()
  {
  }
  int Image::width() const
  {
    return -1;
  }
  int Image::height() const
  {
    return -1;
  }

  // TODO: remove this DUMMY_color variable once you have appropriate fields for representing image pixels.
  Color DUMMY_color;

  Color& Image::at(int x, int y)
  {
    return DUMMY_color;
  }

  const Color& Image::at(int x, int y) const
  {
    return DUMMY_color;
  }
}
