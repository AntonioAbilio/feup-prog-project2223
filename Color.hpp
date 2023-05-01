#ifndef __prog_Color_hpp__
#define __prog_Color_hpp__

namespace prog
{
  typedef unsigned char rgb_value;
  class Color
  {
    private:
      rgb_value red_;
      rgb_value green_;
      rgb_value blue_; 
    public:
      Color();
      Color(const Color& c);
      Color(rgb_value r, rgb_value g, rgb_value b);
      static const Color White(); // <- static method
      rgb_value red() const;      // this means that we can access the color white
      rgb_value& red();           // using the class directly, without creating an object - Antonio.
      rgb_value green() const;
      rgb_value& green();
      rgb_value blue() const;
      rgb_value& blue();
  };
}
#endif
