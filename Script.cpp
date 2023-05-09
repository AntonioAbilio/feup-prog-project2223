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
            if (command == "invert") {              // invert
                Script::invert();                               
                continue;
            }
            if (command == "to_gray_scale") {
                Script::to_gray_scale();      // to_gray_scale
                continue;
            }
            if (command == "replace"){
                // 3 - call to replace()            // MISSING - Vanessa
                continue;
            }
            if (command == "fill"){                 // fill  
                int x, y, w, h;
                input >> x >> y >> w >> h;
                Color c;
                input >> c;
                Script::fill(x, y, w, h, c.red(), c.green(), c.blue());     
                continue;
            }
            if (command == "h_mirror"){             // h_mirror
                Script::h_mirror();           
                continue;
            }
            if (command == "v_mirror"){
                // 6 - call to v_mirror()           // MISSING - Vanessa
                continue;
            }
            if (command == "add"){
                string file;
                input >> file;

                int red;
                input >> red;
                
                int green;
                input >> green;

                int blue;
                input >> blue;

                int wx;
                input >> wx;

                int wy;
                input >> wy;
                Image* altimage = loadFromPNG(file);
                //cout << file << "\n" ;
                Script::add(altimage, red, green, blue, wx, wy);                // add
                continue;
            }

            // Script commands for dimension-changing operations.
            if (command == "crop"){
                // 8 - call to crop()               // MISSING - Vanessa
                continue;
            }
            if (command == "rotate_left"){          // rotate_left
                Script::rotate_left();
                continue;
            }
            if (command == "rotate_right"){
                Script::rotate_right();      // rotate_right
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


    // invert
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
    }   


    // fill
    void Script::fill(int x, int y, int w, int h, rgb_value r, rgb_value g, rgb_value b){
        for (int yi = y; yi < y + h; yi++){
            for (int xi = x; xi < x + w; xi++){
                Color& current_pixel = image->at(xi, yi);
                current_pixel = Color(r, g, b);
            }
        }
    }

    // h_mirror
    void Script::h_mirror(){
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width()/2; x++){
                Color& current_pixel = image->at(x, y);
                Color& mirror_pixel = image->at(image->width() - x - 1, y);
                Color temp = current_pixel;
                current_pixel = mirror_pixel;
                mirror_pixel = temp;
            }
        }
    }


    // rotate_left
    void Script::rotate_left(){
        Image* image_rot = new Image(image->height(), image->width());
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                Color& current_pixel = image->at(x, y);
                Color& rot_pixel = image_rot->at(y, image->width() - x - 1);
                rot_pixel = current_pixel;
            }
        }
        *image = *image_rot;
        delete image_rot;
    }

    // to_gray_scale
    void Script::to_gray_scale(){
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                Color& pixel = image->at(x, y);
                rgb_value v = ((pixel.red() + pixel.green() + pixel.blue())/3);
                pixel.red() = v;
                pixel.green() = v;
                pixel.blue() = v;
            }
        }
    }

    // rotate_right
    void Script::rotate_right(){
        Image* rot_img = new Image(image->height(), image->width());
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                Color& orgC = image->at(x, y);
                Color& rotC = rot_img->at(image->height() - y - 1, x);
                rotC = orgC;
            }
        }
        *image = *rot_img;
        delete rot_img;
    }

    // add
    void Script::add(Image* altimg, rgb_value r, rgb_value g, rgb_value b, int x, int y){
        int tx = x;
        int ty = y;
        for(int ay = 0; ay < altimg->height(); ay++){
            for (int ax = 0; ax < altimg->width(); ax++){
                Color& altC = altimg->at(ax, ay);
                Color& C = image->at(tx, ty);
                tx++;
                if (altC.red() == r){
                    if (altC.green() == g){
                        if (altC.blue() == b){
                            continue;
                        } else {
                            C = altC;
                        }
                    } else {
                        C = altC;
                    }
                } else {
                    C = altC;
                }
            }
            tx = x;
            ty++;

        }
    delete altimg;
    }

}
