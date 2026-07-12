#include <windows.h>
#include <iostream>
#include <string>

class Terminal_Display {
public:

    void Initialise() {

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        DWORD mode = 0;
        GetConsoleMode(hOut, &mode);

        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, mode);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);

        cols =
            csbi.srWindow.Right -
            csbi.srWindow.Left + 1;

        rows =
            csbi.srWindow.Bottom -
            csbi.srWindow.Top + 1;

        display = new std::string[rows * cols];

        Clear();
    }

    void Set(int row,int col,const std::string& ansi_code) {

        if(row < 0 || row >= rows)
            return;

        if(col < 0 || col >= cols)
            return;

        display[row * cols + col] = ansi_code;
    }

    std::string Get(int row,int col) const {

        if(row < 0 || row >= rows)
            return "";

        if(col < 0 || col >= cols)
            return "";

        return display[row * cols + col];
    }

    void Clear() {

        for(int i=0;i<rows*cols;i++)
            display[i] = "\033[0m ";
    }

    void Render() {

        std::string buffer;

        buffer += "\033[H";

        for(int row=0;row<rows;row++) {

            for(int col=0;col<cols;col++) {

                buffer += display[row * cols + col];
            }

            if(row != rows-1)
                buffer += '\n';
        }

        DWORD written;

        WriteConsoleA(
            GetStdHandle(STD_OUTPUT_HANDLE),
            buffer.c_str(),
            static_cast<DWORD>(buffer.size()),
            &written,
            nullptr
        );
    }

    ~Terminal_Display() {

        delete[] display;
    }

private:

    int rows = 0;
    int cols = 0;

    std::string* display = nullptr;
};
