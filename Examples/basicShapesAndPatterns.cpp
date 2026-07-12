// demo1_basics.cpp
#include "../Include/akschitraDisplayV2.cpp"
#include <thread>
#include <chrono>

int main() {
    akschitraDisplay display;
    display.Initialise();
    display.Clear();

    // Draw a rectangle border
    for (int i = 0; i < 30; i++) {
        display.setByHex(5, 20 + i, "#FFD700", "═");
        display.setByHex(15, 20 + i, "#FFD700", "═");
    }
    for (int i = 0; i < 10; i++) {
        display.setByHex(5 + i, 20, "#FFD700", "║");
        display.setByHex(5 + i, 49, "#FFD700", "║");
    }
    // Corners
    display.setByHex(5, 20, "#FFD700", "╔");
    display.setByHex(5, 49, "#FFD700", "╗");
    display.setByHex(15, 20, "#FFD700", "╚");
    display.setByHex(15, 49, "#FFD700", "╝");

    // Draw a diagonal line
    for (int i = 0; i < 10; i++) {
        display.setByHex(6 + i, 25 + i, "#FF6B6B", "●");
    }

    // Draw a circle (approximate)
    int cx = 35, cy = 10, radius = 5;
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius && x*x + y*y >= (radius-1)*(radius-1)) {
                display.setByHex(cy + y, cx + x, "#4ECDC4", "○");
            }
        }
    }

    // Fill with gradient
    for (int i = 0; i < 8; i++) {
        int color = 100 + i * 20;
        for (int j = 0; j < 15; j++) {
            display.setByRaw(20 + i, 10 + j, color, 100, 255 - color/2);
        }
    }

    display.Render();
    
    return 0;
}