#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmath>

class akschitraDisplay {
private:
    bool isInitialized=false;

    int rows = 0;
    int cols = 0;

    std::string* display = nullptr;

public:

    void Initialise() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        rows = w.ws_row;
        cols = w.ws_col;

        display = new std::string[rows * cols];

        for (int i = 0; i < rows * cols; i++) {
            display[i] = "\033[0m ";
        }

        isInitialized = true;
    }

    bool setByHex(int row, int col, const std::string& hex_code="#ffffff", std::string character="█") {
        if(!isInitialized)
            return false;

        if (row < 0 || row >= rows || col < 0 || col >= cols) return false;
        
        // Security: validate hex
        if (hex_code.length() != 7 || hex_code[0] != '#') return false;
        for (int i = 1; i < 7; i++) {
            if (!isxdigit(hex_code[i])) return false;
        }
        
        // Convert hex to ANSI
        int r = std::stoi(hex_code.substr(1, 2), nullptr, 16);
        int g = std::stoi(hex_code.substr(3, 2), nullptr, 16);
        int b = std::stoi(hex_code.substr(5, 2), nullptr, 16);
        
        std::string ansi = "\033[38;2;" + std::to_string(r) + ";" + 
                        std::to_string(g) + ";" + std::to_string(b) + "m" + character;
        
        display[row * cols + col] = ansi;

        return true;
    }

    bool setByRaw(int row, int col, int r=255, int g=255, int b=255, std::string character="█") {
        if(!isInitialized)
            return false;

        if (row < 0 || row >= rows || col < 0 || col >= cols) return false;
        
        // Security: validate RGB values
        if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) return false;
        
        // Convert RGB to ANSI
        std::string ansi = "\033[38;2;" + std::to_string(r) + ";" + 
                        std::to_string(g) + ";" + std::to_string(b) + "m" + character;
        
        display[row * cols + col] = ansi;

        return true;
    }

    std::string Get(int row, int col) const {
        if(!isInitialized)
            return "";

        if (row < 0 || row >= rows ||
            col < 0 || col >= cols)
            return "";

        return display[row * cols + col];
    }

    bool Render() {
        if(!isInitialized)
            return false;

        std::string buffer;

        buffer += "\033[H";

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                buffer += display[row * cols + col];
            }

            if (row != rows - 1)
                buffer += '\n';
        }

        write(STDOUT_FILENO,
              buffer.c_str(),
              buffer.size());

        return true;
    }

    ~akschitraDisplay() {
        delete[] display;
    }

    bool Clear() {
        if(!isInitialized)
            return false;

        for (int i = 0; i < rows * cols; i++) {
            display[i] = "\033[0m ";
        }

        std::cout << "\033[H\033[2J\033[3J" << std::flush;

        return true;
    }
};