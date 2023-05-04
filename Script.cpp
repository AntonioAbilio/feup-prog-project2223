#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;

            // Script commands for initialization and I/O.
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            }

            // TODO
            // Script commands for simple image manipulations (image dimensions are not altered).
            if (command == "invert") {
                invert();
                continue;
            }
            if (command == "to_gray_scale") {
                // 2 - call to to_gray_scale()          // MISSING - Vanessa
                continue;
            }
            if (command == "replace"){
                // 3 - call to replace()                // MISSING - Vanessa
                continue;
            }
            if (command == "fill"){
                // 4 - call to fill()                   // MISSING - Vanessa
                continue;
            }
            if (command == "h_mirror"){
                // 5 - call to h_mirror()               // MISSING - Vanessa
                continue;
            }
            if (command == "v_mirror"){
                // 6 - call to v_mirror()               // MISSING - Vanessa
                continue;
            }
            if (command == "add"){
                // 7 - call to add()                    // MISSING - Vanessa
                continue;
            }

            // Script commands for dimension-changing operations.
            if (command == "crop"){
                // 8 - call to crop()                   // MISSING - Vanessa
                continue;
            }
            if (command == "rotate_left"){
                // 9 - call to rotate_left()            // MISSING - Vanessa
                continue;
            }
            if (command == "rotate_right"){
                // 10 - call to rotate_right()          // MISSING - Vanessa
                continue;
            }

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }


    // 1 
    void Script::invert() {     // Invert colors of current image.
        for (int y = 0; y < image->height(); y++){    // y - line (0 <= y < height_)
            for (int x = 0; x < image->width(); x++){   // x - column (0 <= x < width_)
                Color& current_pixel = image->at(x, y);
                rgb_value& r = current_pixel.red();
                rgb_value& g = current_pixel.green();
                rgb_value& b = current_pixel.blue();
                r = 255 - r;
                g = 255 - g;
                b = 255 - b;
            }
        }
    }   // THIS FUNCTION IS WORKING :))))))


}
