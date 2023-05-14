#include "XPM2.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

/* 
    Planning for loadFromXPM2:

        First way:
            Read all colors, store them in a vector or a map and check for every pixel the value inside of the map.
        Second way:
            Read one color, apply changes to every character that represents that color and repeat.
            It looks like the efficiency would be gratly reduced...
         
    Checks for loadFromXPM2:
        cout << file << "\n";
        cout << " width: " << wh << " height: " << ht << " colors: " << colors << " chrpp: " << chrpp << "\n";
        cout << "Current color: " << color_char << " Char is set to color: " << hex_color << "\n";
        cout << " red: " << r_hex << " green: " << g_hex  << " blue: " << b_hex << endl;


 */


namespace prog {

    Color hex_to_rgb(string r_hex, string g_hex, string b_hex){

    }

    Image* loadFromXPM2(const std::string& file) {
        
        ifstream in;
        
        in.open(file);
        string line;

        getline(in, line); // Can be used to check if the content belongs to a xpm2 file, ignored.

        getline(in, line); // Parameters (Size, Color Count, Chars per pixel).
        istringstream iss(line);
        int wh; // Width.
        int ht; // Height.
        int colors; // Amount of colors to load.
        int chrpp; // Char per pixel -> const 1.
        iss >> wh >> ht >> colors >> chrpp;
        Image* img = new Image (wh, ht);

        // First way

        string colr; 
        string color_char;
        string hex_color;
        map<string, Color> color_map;
        Color temp;
        
        for (int i = 0; i < colors; i++){
            getline(in, colr); // Line with color.
            istringstream prc(colr);
            prc >> color_char >> hex_color >> hex_color; // 2x hex_color because we know that c is const, it can be ignored;
            hex_color = hex_color.substr(1, 6);
            temp = hex_to_rgb(hex_color.substr(0,2), hex_color.substr(2,2), hex_color.substr(4,2));
            
          // UNFINISHED :(  
        }
        return img;
    }

    void saveToXPM2(const std::string& file, const Image* image) {

    }
}
