// demo2_animation.cpp
#include "../Include/akschitraDisplayV2.cpp"
#include <thread>
#include <chrono>

int main() {
    akschitraDisplay display;
    display.Initialise();
    display.Clear();

    const char* spinner[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    const char* colors[] = {"#FF0000", "#FF7F00", "#FFFF00", "#00FF00", "#0000FF", "#4B0082", "#8B00FF"};
    
    // Animate spinner with rainbow colors
    for (int frame = 0; frame < 60; frame++) {
        display.Clear();
        
        // Title
        for (int i = 0; i < 20; i++) {
            display.setByHex(5, 15 + i, "#FFFFFF", "=");
        }
        display.setByHex(5, 13, "#FFFFFF", "►");
        display.setByHex(5, 36, "#FFFFFF", "◄");
        
        // Loading text
        for (int i = 0; i < 10; i++) {
            display.setByHex(7, 20 + i, "#AAAAAA", "L");
        }
        display.setByHex(7, 30, "#AAAAAA", "O");
        for (int i = 0; i < 7; i++) {
            display.setByHex(7, 32 + i, "#AAAAAA", "A");
        }
        display.setByHex(7, 39, "#AAAAAA", "D");
        for (int i = 0; i < 7; i++) {
            display.setByHex(7, 41 + i, "#AAAAAA", "I");
        }
        display.setByHex(7, 48, "#AAAAAA", "G");
        
        // Spinner with rainbow colors
        int colorIdx = (frame / 8) % 7;
        display.setByHex(10, 25, colors[colorIdx], spinner[frame % 10]);
        
        // Progress bar
        int progress = (frame * 100) / 60;
        for (int i = 0; i < 40; i++) {
            if (i < (progress * 40) / 100) {
                display.setByHex(12, 15 + i, "#00FF88", "█");
            } else {
                display.setByHex(12, 15 + i, "#333333", "░");
            }
        }
        
        // Percentage
        std::string pct = std::to_string(progress) + "%";
        for (size_t i = 0; i < pct.length(); i++) {
            display.setByHex(12, 56 + i, "#FFFFFF", std::string(1, pct[i]));
        }
        
        display.Render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Final message
    display.Clear();
    display.setByHex(10, 25, "#00FF00", "✓");
    for (int i = 0; i < 20; i++) {
        display.setByHex(10, 27 + i, "#00FF00", "=");
    }
    display.setByHex(10, 48, "#00FF00", "DONE!");
    display.Render();
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}