// demo3_colorpicker.cpp
#include "../Include/akschitraDisplayV2.cpp"
#include <thread>
#include <chrono>

int main() {
    akschitraDisplay display;
    display.Initialise();
    display.Clear();

    // Display color palette
    int colors[][3] = {
        {255, 0, 0},    // Red
        {255, 127, 0},  // Orange
        {255, 255, 0},  // Yellow
        {0, 255, 0},    // Green
        {0, 0, 255},    // Blue
        {75, 0, 130},   // Indigo
        {148, 0, 211}   // Violet
    };
    
    const char* labels[] = {"RED", "ORNG", "YLLW", "GRN", "BLUE", "INDG", "VIOL"};
    
    // Draw color swatches
    for (int i = 0; i < 7; i++) {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 10; x++) {
                display.setByRaw(5 + y, 10 + i * 12 + x, 
                               colors[i][0], colors[i][1], colors[i][2]);
            }
        }
        
        // Label
        for (int j = 0; j < 4; j++) {
            display.setByHex(11, 12 + i * 12 + j, "#FFFFFF", std::string(1, labels[i][j]));
        }
    }
    
    // Show RGB values
    for (int i = 0; i < 7; i++) {
        std::string rgb = "R" + std::to_string(colors[i][0]) + 
                         " G" + std::to_string(colors[i][1]) + 
                         " B" + std::to_string(colors[i][2]);
        for (size_t j = 0; j < rgb.length(); j++) {
            display.setByHex(13, 12 + i * 12 + j, "#888888", std::string(1, rgb[j]));
        }
    }
    
    // Animate selection
    for (int selection = 0; selection < 14; selection++) {
        int idx = selection % 7;
        
        // Highlight selected swatch
        for (int y = 4; y < 11; y++) {
            for (int x = 9 + idx * 12; x < 21 + idx * 12; x++) {
                if (y == 4 || y == 10 || x == 9 + idx * 12 || x == 20 + idx * 12) {
                    display.setByRaw(y, x, 255, 255, 255);
                }
            }
        }
        
        // Display selected color in large format
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 20; x++) {
                display.setByRaw(20 + y, 30 + x, 
                               colors[idx][0], colors[idx][1], colors[idx][2]);
            }
        }
        
        display.Render();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    return 0;
}