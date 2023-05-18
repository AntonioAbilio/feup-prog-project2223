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
            if (command == "invert") {
                Script::invert();                               
                continue;
            }
            if (command == "to_gray_scale") {
                Script::to_gray_scale();        
                continue;
            }
            if (command == "replace") {
                int r1, g1, b1, r2, g2, b2;
                input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
                Script::replace(r1, g1, b1, r2, g2, b2);
                continue;
            }
            if (command == "fill") {                        
                int x, y, w, h;
                input >> x >> y >> w >> h;
                Color c;
                input >> c;
                Script::fill(x, y, w, h, c.red(), c.green(), c.blue());     
                continue;
            }
            if (command == "h_mirror") {
                Script::h_mirror();           
                continue;
            }
            if (command == "v_mirror") {                     
                Script::v_mirror();
                continue;
            }
            if (command == "add") {
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
                Script::add(altimage, red, green, blue, wx, wy);                
                continue;
            }

            // Script commands for dimension-changing operations.
            if (command == "crop") {
                int x, y, w, h;
                input >> x >> y >> w >> h;
                Script::crop(x, y, w, h);
                continue;
            }
            if (command == "rotate_left") {          
                Script::rotate_left();
                continue;
            }
            if (command == "rotate_right") {
                Script::rotate_right();      
                continue;
            }
            if (command == "median_filter") {
                int ws = 0;
                input >> ws;
                Script::median_filter(ws);     
                continue;
            }
            if (command == "xpm2_open") {
                clear_image_if_any();
                string filename;
                input >> filename;
                image = loadFromXPM2(filename);     
                continue;
            }
            if (command == "xpm2_save") {
                string output_path;
                input >> output_path;
                saveToXPM2(output_path, image);
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

    void Script::invert() {     
        // Inverts colors of current image.
        for (int y = 0; y < image->height(); y++){    // y - line of image.
            for (int x = 0; x < image->width(); x++){   // x - column of image.
                Color& current_pixel = image->at(x, y); // Get pixel of image at (x, y).
                rgb_value& r = current_pixel.red();     // Get a reference to each Color value. 
                rgb_value& g = current_pixel.green();
                rgb_value& b = current_pixel.blue();
                r = 255 - r;    // Invert every value of pixel directly (by reference).
                g = 255 - g;
                b = 255 - b;
            }
        }
    }   

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

    void Script::replace(int r1, int g1, int b1, int r2, int g2, int b2){
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

    void Script::fill(int x, int y, int w, int h, rgb_value r, rgb_value g, rgb_value b){   
        // Replaces a defined part of image with color fill.
        for (int yi = y; yi < y + h; yi++){     // y - line of image.
            for (int xi = x; xi < x + w; xi++){     // x - column of image.
                Color& current_pixel = image->at(xi, yi);   // Get pixel of image at (x, y).
                current_pixel = Color(r, g, b);     // Replace pixel with color fill.
            }
        }
    }

    void Script::h_mirror(){    
        // Mirrors image horizontally (vertical axis).
        for (int y = 0; y < image->height(); y++){   // y - line of image.
            for (int x = 0; x < image->width()/2; x++){     // x - column of image (until half width).
                Color& current_pixel = image->at(x, y);     // Get pixel of image at (x, y).
                Color& mirror_pixel = image->at(image->width() - x - 1, y); // Get mirrored pixel of image.
                Color temp = current_pixel;     // Temporary copy of pixel at (x,y).
                current_pixel = mirror_pixel;   // Swap pixel and mirrored pixel.
                mirror_pixel = temp;
            }
        }
    }

    void Script::v_mirror(){    
        // Mirrors image vertically (horizontal axis).
        for (int y = 0; y < image->height()/2; y++){    // y - line of image (until half height).
            for (int x = 0; x < image->width(); x++){   // x - column of image.
                Color& current_pixel = image->at(x, y);     // Get pixel of image at (x, y).
                Color& mirror_pixel = image->at(x , image->height() - 1 - y);   // Get mirrored pixel of image.
                Color temp = current_pixel;     // Temporary copy of pixel at (x, y).
                current_pixel = mirror_pixel;   // Swap pixel and mirrored pixel.
                mirror_pixel = temp;
            }
        }
    }

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
    delete altimg; // Free the dynamic memory.
    }

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
        delete cropped; // Free the dynamic memory.
    }

    void Script::rotate_left(){    
         // Rotate image left by 90 degrees.
        Image* image_rot = new Image(image->height(), image->width());  // Create copy of image (with width and height switched).
        for (int y = 0; y < image->height(); y++){     // y - line of image.
            for (int x = 0; x < image->width(); x++){   // x - column of image.
                Color& current_pixel = image->at(x, y);     // Get pixel of image at (x, y).
                Color& rot_pixel = image_rot->at(y, image->width() - x - 1);    // Get pixel of rotated image at relative position.
                rot_pixel = current_pixel;  // Assign color of pixel to rotated pixel.
            }
        }
        *image = *image_rot;    // Rotated image is now assigned to the original image.
        delete image_rot;   // Delete unused rotated image.
    }

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
        delete rot_img;  // Free the dynamic memory.
    }

    // Median filter (functions below).
    bool compare(const rgb_value& a, const rgb_value& b){   // Auxiliary function.
        // Compare function for sort.
        return (a < b);
    }

    rgb_value median(vector<rgb_value> window){     // Auxiliary function.
        // Returns median of a vector.
        sort(window.begin(), window.end(), compare);    
        int size = window.size();                       
        if (size % 2 == 0) return (window[size/2 - 1] + window[size/2]) / 2;  // Window has even size. -> Even means there is no "middle" value.
        else return window[size/2];                                           // Window has odd size.
    }

    Color median_calc(const int& x, const int& y, const int& ws, const Image& img_copy){    // Auxiliary function.
        vector<rgb_value> window_r;    // Vector of red values of pixels in neighbourhood.
        vector<rgb_value> window_g;    // Vector of green values of pixels in neighbourhood.
        vector<rgb_value> window_b;    // Vector of blue values of pixels in neighbourhood.

        // Iterate through the values of the neighbourhood of the pixel and copies the values to the previously created vectors.
        for (int nx = max(0, x - ws / 2); nx <= min(img_copy.width() - 1, x + ws / 2); nx++){
            for (int ny = max(0, y - ws / 2); ny <= min(img_copy.height() - 1, y + ws / 2); ny++){
                window_r.push_back(img_copy.at(nx, ny).red());
                window_g.push_back(img_copy.at(nx, ny).green());
                window_b.push_back(img_copy.at(nx, ny).blue());
            }
        }

        if (window_r.empty()) return img_copy.at(x, y);     // If the neighbourhood is empty, return the pixel itself.

        rgb_value mr = median(window_r);
        rgb_value mg = median(window_g);
        rgb_value mb = median(window_b);
        return {mr, mg, mb};        // Return median of each window separately.
    }

    void Script::median_filter(int ws){
        if (ws % 2 == 0 || ws < 3) return;        // ws can't be even or less than 3
        Image img_copy = *image;                  // The original image will be altered, so we need a copy.
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                image->at(x, y) = median_calc(x, y, ws, img_copy);      // Calculates median value for each pixel and applies to image.
            }
        }
    }


}

