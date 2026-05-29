#include "Akschitra_Initiality_v(1).cpp"

Terminal_Display Display;

int main() {

    Display.Initialise();

    for (int row = 0; row < 40; row++) {

        for (int col = 0; col < 80; col++) {

            int r = (row * 255) / 40;
            int g = (col * 255) / 80;
            int b = 150;

            Display.Set(
                row,
                col,
                "\033[48;2;" +
                std::to_string(r) + ";" +
                std::to_string(g) + ";" +
                std::to_string(b) + "m "
            );
        }
    }

    Display.Render();
}
