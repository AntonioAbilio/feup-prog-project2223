#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <cmath>
#include <algorithm>
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

            // Script commands for simple image manipulations (image dimensions are not altered).
            if (command == "invert") {                      // invert
                Script::invert();                               
                continue;
            }
            if (command == "to_gray_scale") {               // to_gray_scale
                Script::to_gray_scale();        
                continue;
            }
            if (command == "replace"){                      // replace
                Script:: replace();
                continue;

            }
            if (command == "fill"){                         // fill  
                int x, y, w, h;
                input >> x >> y >> w >> h;
                Color c;
                input >> c;
                Script::fill(x, y, w, h, c.red(), c.green(), c.blue());     
                continue;
            }
            if (command == "h_mirror"){                     // h_mirror
                Script::h_mirror();           
                continue;
            }
            if (command == "v_mirror"){                     // v_mirror
                Script::v_mirror();
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
                int x, y, w, h;
                input >> x >> y >> w >> h;
                Script::crop(x, y, w, h);
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

            if (command == "median_filter"){
                int ws = 0;
                input >> ws;
                Script::median_filter(ws);     // median_filter
                continue;
            }

            if (command == "xpm2_open"){
                clear_image_if_any();
                string filename;
                input >> filename;
                image = loadFromXPM2(filename);     // xpm2_open
                continue;
            }

            if (command == "xpm2_save"){
                string output_path;
                input >> output_path;
                saveToXPM2(output_path, image);     // xpm2_save
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


    // v_mirror
    void Script::v_mirror(){
        for (int y = 0; y < image->height()/2; y++){
            for (int x = 0; x < image->width(); x++){
                Color& current_pixel = image->at(x, y);
                Color& mirror_pixel = image->at(x , image->height() - 1 - y);
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

        // Run through the pixels of the image.
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                Color& pixel = image->at(x, y); // Get the pixel.
                // The average is so we can go from 0 up to 255 on every pixel. This mean we will go from black values to whiter values.
                rgb_value v = ((pixel.red() + pixel.green() + pixel.blue())/3); // Get the average value.
                // Replace the current rbg value with the average.
                pixel.red() = v;
                pixel.green() = v;
                pixel.blue() = v;
            }
        }
    }

    //replace
    void Script::replace(){
        int r1, g1, b1, r2, g2, b2;
        input >> r1>> g1>> b1>> r2>> g2>> b2; // Get the values for this function.

        // Run through the pixels of the image.
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                Color& pixel= image->at(x, y); // Get the pixel.

                // If we find the desired color we wish to replace, replace it.
                if(pixel.red() == r1 && pixel.green() == g1 && pixel.blue() == b1){
                    pixel.red() = r2;
                    pixel.green() = g2;
                    pixel.blue() = b2;
                }

            }
        }
        

    }


    // rotate_right
    void Script::rotate_right(){
        Image* rot_img = new Image(image->height(), image->width());    // Create a new image with the height and width switched.

        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                Color& orgC = image->at(x, y);  // Get the pixel of the original image.
                // For every pixel we need to reposition it.
                // The x now is the y and the new x is calculated by the total height - y - 1.
                /* 
                
                 _ _ _ _           _ _ _
                |0 0 * 0          |0 0 0
                |0 0 0 0    -->   |0 0 0
                |0 0 0 0          |0 0 *
                                  |0 0 0
                 */
                Color& rotC = rot_img->at(image->height() - y - 1, x);
                rotC = orgC; // Copy the pixel from the original image to the new image
            }
        }
        *image = *rot_img; // Replace the image with the one that was rotated.
        delete rot_img;  // #freethedinamicmemory
    }


    //crop
    void Script::crop(int x, int y, int w, int h){
        Image* cropped = new Image(w, h);   // Create a new image that we'll use to copy the current image so that we can work on it.
        for (int y_img = y; y_img < y + h; y_img++){    // For each line in cropped image (indexing of original image)
            for (int x_img = x; x_img < x + w; x_img++){    // For each column in cropped image (indexing of original image)
                cropped->at(x_img - x, y_img - y) = image->at(x_img, y_img);    // Copy the pixel from the current image and place them in the new image
                // x_img and y_img begin in the place we want to begin the crop.
                // This means that we can use a single variable to run through the pixel_matrix of the cropped image.
            }
        }
        *image = *cropped; // Replace the current image.
        delete cropped; // #freethedinamicmemory
    }


    // add
    void Script::add(Image* altimg, rgb_value r, rgb_value g, rgb_value b, int x, int y){
        // Note: Provided image is altimg, the image to be added to the current image.

        int tx = x; // Save the original starting position of x
        int ty = y; // Save the original starting position of y

        // Run through the provided image.
        for(int ay = 0; ay < altimg->height(); ay++){
            for (int ax = 0; ax < altimg->width(); ax++){
                Color& altC = altimg->at(ax, ay); // Copy the pixel of the provided image.
                Color& C = image->at(tx, ty);   // Get the pixel from the current image.
                tx++; // Increment the x position of the provided image

                // Start the comparison between colors
                // if red, green and blue are the same then we must not replace the color
                if (altC.red() == r){
                    if (altC.green() == g){
                        if (altC.blue() == b){
                            continue; // Give control back to the second for loop.
                        } else {
                            C = altC; // Blue is different which means that the color is different.
                        }
                    } else {
                        C = altC; // Green is different which means that the color is different.
                    }
                } else {
                    C = altC; // Red is different which means that the color is different.
                }
            }
            tx = x; // Set tx back to the origin.
            ty++; // Incremenet the y of the provided image.

        }
    delete altimg; // #freethedinamicmemory
    }


    // median_filter

    bool compare(const rgb_value& a, const rgb_value& b){     // auxiliary function
        return (a < b);
    }   

    rgb_value median(vector<rgb_value> window){     // auxiliary function
        sort(window.begin(), window.end(), compare);    
        int size = window.size();                       
        if (size % 2 == 0) return (window[size/2 - 1] + window[size/2]) / 2;  // window has even size
        else return window[size/2];                                           // window has odd size
    }

    Color median_calc(const int& x, const int& y, const int& ws, const Image& img_copy){         // auxiliary function
        vector<rgb_value> window_r;    // vector of red values of pixels in neighbourhood
        vector<rgb_value> window_g;    // vector of green values of pixels in neighbourhood
        vector<rgb_value> window_b;    // vector of blue values of pixels in neighbourhood

        for (int nx = max(0, x - ws / 2); nx <= min(img_copy.width() - 1, x + ws / 2); nx++){
            for (int ny = max(0, y - ws / 2); ny <= min(img_copy.height() - 1, y + ws / 2); ny++){
                window_r.push_back(img_copy.at(nx, ny).red());
                window_g.push_back(img_copy.at(nx, ny).green());
                window_b.push_back(img_copy.at(nx, ny).blue());
            }
        }

        if (window_r.empty()) return img_copy.at(x, y);     // if the neighbourhood is empty, return the pixel itself

        rgb_value mr = median(window_r);
        rgb_value mg = median(window_g);
        rgb_value mb = median(window_b);
        return {mr, mg, mb};        // return median of each window separately
    }
    void Script::median_filter(int ws){
        if (ws % 2 == 0 || ws < 3) return;        // ws can't be even or less than 3
        Image img_copy = *image;                  // the original image will be altered, so we need a copy
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                image->at(x, y) = median_calc(x, y, ws, img_copy);      // calculates median value for each pixel and applies to image
            }
        }
    
    }

}
