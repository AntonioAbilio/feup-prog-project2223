#ifndef __prog_Script_hpp__
#define __prog_Script_hpp__

#include <string>
#include <fstream>
#include "Image.hpp"

namespace prog
{
  class Script
  {
  public: 
    Script(const std::string &filename);
    ~Script();
    void run();
  private:
    // Current image.
    Image *image;
    // Input stream for reading script commands.
    std::ifstream input;
  private:
    // Private functions
    void clear_image_if_any();
    void open();
    void blank();
    void save();

    void invert();   // invert
    void fill(int x, int y, int w, int h, rgb_value r, rgb_value g, rgb_value b);  // fill
    void h_mirror();  // h_mirror
    void rotate_left(); // rotate_left
    void to_gray_scale(); // to_gray_scale
  };
}
#endif
