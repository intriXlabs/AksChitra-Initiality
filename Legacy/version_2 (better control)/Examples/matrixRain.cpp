// demo4_matrix.cpp
#include "../Include/akschitraDisplayV2.cpp"
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    akschitraDisplay display;
    display.Initialise();
    display.Clear();
    
    int numCols = 50;
    int numRows = 25;
    int* drops = new int[numCols];
    int* speeds = new int[numCols];
    
    // Initialize drops
    for (int i = 0; i < numCols; i++) {
        drops[i] = rand() % 30 - 10;
        speeds[i] = 1 + rand() % 3;
    }
    
    // Matrix rain animation
    for (int frame = 0; frame < 100; frame++) {
        display.Clear();
        
        // Update and draw drops
        for (int col = 0; col < numCols; col++) {
            drops[col] += speeds[col];
            
            // Reset drop if it goes off screen
            if (drops[col] > numRows) {
                drops[col] = rand() % 5 - 10;
                speeds[col] = 1 + rand() % 3;
            }
            
            // Draw the drop trail
            for (int row = 0; row < 8; row++) {
                int pos = drops[col] - row;
                if (pos >= 0 && pos < numRows) {
                    int brightness = 255 - (row * 30);
                    if (brightness < 0) brightness = 0;
                    
                    if (row == 0) {
                        // Head of drop - bright white
                        display.setByRaw(pos, col + 10, 255, 255, 255, "█");
                    } else {
                        // Trail - green fading with different characters
                        std::string chars[] = {"█", "▓", "▒", "░"};
                        int charIdx = row % 4;
                        display.setByRaw(pos, col + 10, 0, brightness, 0, chars[charIdx]);
                    }
                }
            }
        }
        
        // Random bright sparks
        for (int i = 0; i < 3; i++) {
            int col = rand() % numCols;
            int row = rand() % numRows;
            if (rand() % 10 < 3) {
                display.setByRaw(row, col + 10, 255, 255, 200, "✦");
            }
        }
        
        display.Render();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    delete[] drops;
    delete[] speeds;
    
    // Clean exit message
    display.Clear();
    const char* message = "MATRIX COMPLETE!";
    for (int i = 0; i < 16; i++) {
        display.setByHex(12, 25 + i, "#00FF00", std::string(1, message[i]));
    }
    
    for (int i = 0; i < 10; i++) {
        display.setByHex(12, 42 + i, "#00FF00", "=");
    }
    display.setByHex(12, 53, "#00FF00", "✓");
    
    display.Render();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    return 0;
}