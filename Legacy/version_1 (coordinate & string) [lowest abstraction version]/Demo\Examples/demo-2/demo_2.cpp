#include "akschitraDisplay.cpp"

Terminal_Display Display;

int main() {

    Display.Initialise();

    float t = 0;

    while(true)
    {
        Display.Clear();

        for(int row=0; row<40; row++)
        {
            for(int col=0; col<80; col++)
            {
                float v =
                    sin(col * 0.15f + t) +
                    sin(row * 0.15f + t) +
                    sin((col + row) * 0.10f + t);

                int c = (int)((v + 3.0f) * 42.0f);

                Display.Set(
                    row,
                    col,
                    "\033[48;5;" +
                    std::to_string(c) +
                    "m "
                );
            }
        }

        Display.Render();

        t += 0.1f;
    }
}
