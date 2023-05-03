#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__
#include "Color.hpp"
#include <vector> 

namespace prog
{
  class Image
  {
  private:
    int height_;
    int width_;
    // There may be something missing - Antonio. 
    std::vector<std::vector<Color>> pixel_matrix_; // added - Vanessa
  public:
    Image(int w, int h, const Color &fill = {255, 255, 255}); // = Color::White()); -> Not necessary - Vanessa
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;
  };
}
#endif
