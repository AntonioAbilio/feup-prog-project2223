#include "XPM2.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
using namespace std;

namespace prog {

    Color hex_to_rgb(const string& r_hex, const string& g_hex, const string& b_hex) {
    map<char, int> hex_int = {
        {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
        {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}
    };

    int red = hex_int[r_hex[0]] * 16 + hex_int[r_hex[1]];
    int green = hex_int[g_hex[0]] * 16 + hex_int[g_hex[1]];
    int blue = hex_int[b_hex[0]] * 16 + hex_int[b_hex[1]];
    Color result((rgb_value)red, (rgb_value)green, (rgb_value)blue);
    return result;
}

    Image* loadFromXPM2(const std::string& file) {
        
        ifstream in;
        in.open(file); // Open the file using the provided path.

        string line; // Variable to store lines.
        getline(in, line); // Can be used to check if the content belongs to a xpm2 file, ignored.

        getline(in, line); // Parameters (Size, Color Count, Chars per pixel).
        istringstream iss(line);
        int wh; // Width.
        int ht; // Height.
        int colors; // Amount of colors to load.
        int chrpp; // Char per pixel -> const 1.
        iss >> wh >> ht >> colors >> chrpp;

        Image* img = new Image (wh, ht); // Create a new image with the given dimentions.
    
        char color_char;  // Variable to store the character that is going to be converted.
        string hex_color; // Variable to store the hexadecimal value of a given character.
        map<char, Color> color_map; // Create an empty map to hold the Color value of each character.
        
        for (int i = 0; i < colors; i++){
            // The lines that we get inside this for loop have parameters that we need to save to be able
            // to convert from a character to an actual color.
            getline(in, line); 

            istringstream clr(line);
            clr >> color_char >> hex_color >> hex_color; // 2x hex_color -> c is being ignored; -> Issue...
            hex_color = hex_color.substr(1, 6); // Remove the hastag.

            // Make sure every character is lowercase as
            // the dictionary used in hex_to_rgb function uses lowercase chars.
            for (auto& chr: hex_color) chr = tolower(chr);
            color_map[color_char] = hex_to_rgb(hex_color.substr(0,2), hex_color.substr(2,2), hex_color.substr(4,2));
        }
        
        int line_hw = 0; // We begin at the top of the image.
        while (getline(in, line)){
            for(size_t char_wd = 0; char_wd < line.size(); char_wd++){

                if (((int)char_wd >= img->width())||(line_hw >= img->height())) break; // Some XPM2 files contained way too many characters per line
                Color& replace = img->at(char_wd, line_hw); // Get the current pixel.
                replace = color_map[line[char_wd]]; // Replace the pixel with the color that the map returned.

            }
        line_hw++; // Increment the current y position.
        }
        return img;
    }

    string Color_HEX(Color clr){
        ostringstream ooo;
        map<int, char> hex_int = {
            {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'},
            {8, '8'}, {9, '9'}, {10, 'a'}, {11, 'b'}, {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'}
        };
        int red_int = (int)clr.red();
        int div_red = (int)floor(red_int/16); // Get the first hexadecimal value.
        int rem_red = red_int%16; // Get the second hexadecimal value.


        ooo << hex_int[div_red] << hex_int[rem_red];

        int green_int = (int)clr.green();
        int div_green = (int)floor(green_int/16);
        int rem_green = green_int%16;

        ooo << hex_int[div_green] << hex_int[rem_green];

        int blue_int = (int)clr.blue();
        int div_blue = (int)floor(blue_int/16);
        int rem_blue = blue_int%16;

        ooo << hex_int[div_blue] << hex_int[rem_blue];

        return ooo.str();

    }

    void saveToXPM2(const std::string& file, const Image* image) {
        ofstream out;
        out.open(file); // Create the output file
        out << "! XPM2\n"; // Header for XPM2 file.
        

        char first_color_char = '!'; // The first character is a ! because we have more characters to use if we start here.

        map <string, char> color_char; // Create a map that stores the hexadecimal value for a color along with its character
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){

                // Condition to check if the color is already inside of the map.
                if (color_char.count(Color_HEX(image->at(x,y))) == 0 ){ 
                    color_char[Color_HEX(image->at(x,y))] = first_color_char; 
                    // Place the color and its corresponding character inside the map object
                    // and use another character for a possible future color. 
                    first_color_char += 1;
                }
            }
        }

        // Count the total amount of colors.
        int color_count = 0;
        for (auto counter: color_char){
            color_count++;
        }

        // Save the current width and height of the image to the xpm2 file.
        out << image->width() << " " << image->height() << " " << color_count << " 1\n";

        // Save every color according ti the xpm2 standard.
        for (auto pairs: color_char){
            out << pairs.second << " " << "c" << " #" << pairs.first << endl;
        }

        // Run trough every pixel and store the value in the xpm2 file using the map
        // to convert from an rgb value to a character that represents that same value.
        for (int y = 0; y < image->height(); y++){
            for (int x = 0; x < image->width(); x++){
                out << color_char[Color_HEX(image->at(x,y))];
            }
            out << endl;    
        }
        // Close the previously created file.
        out.close();
        
    }
}
