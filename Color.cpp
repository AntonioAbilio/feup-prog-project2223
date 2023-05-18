#include "Color.hpp"

namespace prog {
    // Default constructor.
    Color::Color() {
        red_ = 0;
        green_ = 0;
        blue_ = 0;
    }
    // Copy constructor.
    Color::Color(const Color& other) {
        red_ = other.red();
        green_ = other.green();
        blue_ = other.blue();
    }
    // Constructor.
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        red_ = red;
        green_ = green;
        blue_ = blue;

    }
    // Methods to access the values without modifying.
    rgb_value Color::red() const {
        return red_;
    }
    rgb_value Color::green() const {
        return green_;
    }
    rgb_value Color::blue() const {
        return blue_;
    }
    // Methods to be able to modify the individual pixeis.
    rgb_value& Color::red() {
        return red_;
    }
    rgb_value& Color::green() {
        return green_;
    }
    rgb_value& Color::blue() {
        return blue_;
    }
}
