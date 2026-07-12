#include "akschitraDisplay.cpp"
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

Terminal_Display Display;


int main() {
    std::srand(std::time(nullptr));
    
    Display.Initialise();
    
    std::cout << "\033[2J\033[?25l";
    std::cout.flush();
    
    float time = 0;
    int mode = 0;
    bool running = true;
    
    while(running) {
        Display.Clear();
        
        int rows = 54;  // Default
        int cols = 210;  // Default
        
        // Plasma effect with rotating wireframe cube
        for(int y = 0; y < rows; y++) {
            for(int x = 0; x < cols; x++) {
                float v1 = sin(x * 0.15 + time);
                float v2 = cos(y * 0.1 + time * 1.3);
                float v3 = sin((x + y) * 0.12 + time * 0.7);
                float v4 = cos(sqrt((x-cols/2)*(x-cols/2) + (y-rows/2)*(y-rows/2)) * 0.3 - time * 2);
                
                float val = (v1 + v2 + v3 + v4) / 4.0;
                int color = 31 + ((int)((val + 1) * 3.5)) % 7;
                
                std::string ch;
                if(val > 0.6) ch = "█";
                else if(val > 0.3) ch = "▓";
                else if(val > 0.0) ch = "▒";
                else if(val > -0.4) ch = "░";
                else ch = " ";
                
                std::stringstream ss;
                ss << "\033[1;" << color << "m" << ch;
                Display.Set(y, x, ss.str());
            }
        }
        
        // 3D Rotating cube wireframe
        float size = 8;
        float cx = cols/2, cy = rows/2;
        
        // Cube vertices
        float verts[8][3] = {
            {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
            {-1,-1,1}, {1,-1,1}, {1,1,1}, {-1,1,1}
        };
        
        // Rotate vertices
        float rotX = time * 0.7, rotY = time * 1.1;
        float rotated[8][3];
        
        for(int i = 0; i < 8; i++) {
            float x = verts[i][0], y = verts[i][1], z = verts[i][2];
            
            // Rotate Y
            float cosY = cos(rotY), sinY = sin(rotY);
            float x1 = x * cosY + z * sinY;
            float z1 = -x * sinY + z * cosY;
            
            // Rotate X
            float cosX = cos(rotX), sinX = sin(rotX);
            float y1 = y * cosX - z1 * sinX;
            float z2 = y * sinX + z1 * cosX;
            
            rotated[i][0] = x1;
            rotated[i][1] = y1;
            rotated[i][2] = z2 + 3; // Push back
        }
        
        // Project and draw edges
        int edges[12][2] = {
            {0,1},{1,2},{2,3},{3,0},
            {4,5},{5,6},{6,7},{7,4},
            {0,4},{1,5},{2,6},{3,7}
        };
        
        for(int e = 0; e < 12; e++) {
            int a = edges[e][0], b = edges[e][1];
            
            float z1 = rotated[a][2], z2 = rotated[b][2];
            if(z1 < 0.1 || z2 < 0.1) continue;
            
            int x1 = cx + rotated[a][0] * size * 2.5 / z1;
            int y1 = cy + rotated[a][1] * size * 1.5 / z1;
            int x2 = cx + rotated[b][0] * size * 2.5 / z2;
            int y2 = cy + rotated[b][1] * size * 1.5 / z2;
            
            // Draw line
            int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
            int dy = -abs(y2-y1), sy = y1<y2 ? 1 : -1;
            int err = dx+dy;
            
            while(true) {
                if(x1 >= 0 && x1 < cols && y1 >= 0 && y1 < rows) {
                    std::stringstream ss;
                    ss << "\033[1;97m█";
                    Display.Set(y1, x1, ss.str());
                }
                if(x1==x2 && y1==y2) break;
                int e2 = 2*err;
                if(e2 >= dy) { err += dy; x1 += sx; }
                if(e2 <= dx) { err += dx; y1 += sy; }
            }
        }
        
        // HUD
        std::string title = " AKSHITRA CROSS-PLATFORM DEMO ";
        for(int i = 0; title[i] && i < cols; i++) {
            std::stringstream ss;
            ss << "\033[1;37;44m" << title[i];
            Display.Set(0, i, ss.str());
        }
        
        std::string bottom = " FPS:50 | MODE:PLASMA+CUBE | PURE C++ | Q TO QUIT ";
        for(int i = 0; bottom[i] && i < cols; i++) {
            std::stringstream ss;
            ss << "\033[1;37;44m" << bottom[i];
            Display.Set(rows-1, i, ss.str());
        }
        
        Display.Render();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        time += 0.02;
        
        // Simple quit after 30 seconds
        if(time > 30) running = false;
    }
    
    std::cout << "\033[2J\033[H\033[?25h";
    std::cout.flush();
    
    return 0;
}
