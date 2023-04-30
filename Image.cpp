#include "Image.hpp"
#include <vector>

namespace prog
{                                             // TODO: add white as default value !!! 
  Image::Image(int w, int h, const Color &fill){
    width_ = w;
    height_ = h;
    
    pixel_matrix_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_)); // lines -> height of the matrix; columns -> width of the matrix
    
    for (int y = 0; y < height_; y++){    // y - line (0 <= y < height_)
      for (int x = 0; x < width_; x++){   // x - column (0 <= x < width_)
        pixel_matrix_[y][x] = fill;       // every pixel of pixel_matrix_ has color fill
      }
    }

    //std::vector<std::vector<Color>> pixel_matrix(height_, std::vector<Color>(width_));  // OUTDATED - vanessa
    // See if y matches the height or width (Lines / Columns)   - antonio
  }

  Image::~Image()
  {
      // Can be empty if no dynamically allocated memory is used explicitly - vanessa
  }

  int Image::width() const
  {
    return width_;
  }

  int Image::height() const
  {
    return height_;
  }

  // TODO: remove this DUMMY_color variable once you have appropriate fields for representing image pixels.
  //Color DUMMY_color;

  Color& Image::at(int x, int y)
  {
    // return DUMMY_color;
    return pixel_matrix_[y][x];
  }

  const Color& Image::at(int x, int y) const
  {
    //return DUMMY_color;
    return pixel_matrix_[y][x];
  }
}
