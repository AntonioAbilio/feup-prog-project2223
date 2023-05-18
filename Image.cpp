#include "Image.hpp"
#include <vector>

namespace prog
{ 
  Image::Image(int w, int h, const Color &fill){
    width_ = w;
    height_ = h;
    
    pixel_matrix_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_)); // lines -> height of the matrix; columns -> width of the matrix
    
    for (int y = 0; y < height_; y++){    // y - line (0 <= y < height_)
      for (int x = 0; x < width_; x++){   // x - column (0 <= x < width_)
        pixel_matrix_[y][x] = fill;       // Every pixel of pixel_matrix_ has color fill.
      }
    }
  }

  Image::~Image()
  {
      // Can be empty if no dynamically allocated memory is used explicitly.
  }

  int Image::width() const
  {
    return width_;
  }

  int Image::height() const
  {
    return height_;
  }

  Color& Image::at(int x, int y)      // Pixel at (x, y) can be altered.
  {
    return pixel_matrix_[y][x];
  }

  const Color& Image::at(int x, int y) const    
  {
    return pixel_matrix_[y][x];
  }
}
