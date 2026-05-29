#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmath>

class Terminal_Display {
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
    }

    void Set(int row, int col, const std::string& ansi_code) {
        if (row < 0 || row >= rows ||
            col < 0 || col >= cols)
            return;

        display[row * cols + col] = ansi_code;
    }

    std::string Get(int row, int col) const {
        if (row < 0 || row >= rows ||
            col < 0 || col >= cols)
            return "";

        return display[row * cols + col];
    }

    void Render() {
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
    }

    ~Terminal_Display() {
        delete[] display;
    }

    void Clear() {
        for (int i = 0; i < rows * cols; i++) {
            display[i] = "\033[0m ";
        }
    }

private:
    int rows = 0;
    int cols = 0;

    std::string* display = nullptr;
};
