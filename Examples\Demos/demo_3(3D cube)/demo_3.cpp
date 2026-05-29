#include "Akschitra_Initiality_v(1).cpp"
#include <cmath>
#include <chrono>
#include <thread>

// -----------------------------
// Line Drawing (Bresenham)
// -----------------------------

void DrawLine(
    Terminal_Display& Display,
    int r0, int c0,
    int r1, int c1,
    const std::string& color)
{
    int dr = abs(r1 - r0);
    int dc = abs(c1 - c0);

    int sr = (r0 < r1) ? 1 : -1;
    int sc = (c0 < c1) ? 1 : -1;

    int err = dc - dr;

    while (true)
    {
        Display.Set(r0, c0, color);

        if (r0 == r1 && c0 == c1)
            break;

        int e2 = err * 2;

        if (e2 > -dr)
        {
            err -= dr;
            c0 += sc;
        }

        if (e2 < dc)
        {
            err += dc;
            r0 += sr;
        }
    }
}

// -----------------------------
// 3D
// -----------------------------

struct Vec3
{
    float x;
    float y;
    float z;
};

struct Point2D
{
    int row;
    int col;
};

int main()
{
    Terminal_Display Display;

    Display.Initialise();

    Vec3 cube[8] =
    {
        {-1,-1,-1},
        { 1,-1,-1},
        { 1, 1,-1},
        {-1, 1,-1},

        {-1,-1, 1},
        { 1,-1, 1},
        { 1, 1, 1},
        {-1, 1, 1}
    };

    int edges[12][2] =
    {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    float angle = 0.0f;

    while (true)
    {
        Display.Clear();

        Point2D projected[8];

        for (int i = 0; i < 8; i++)
        {
            Vec3 v = cube[i];

            // rotate Y
            float x1 =
                v.x * cos(angle)
              - v.z * sin(angle);

            float z1 =
                v.x * sin(angle)
              + v.z * cos(angle);

            // rotate X
            float y2 =
                v.y * cos(angle * 0.7f)
              - z1 * sin(angle * 0.7f);

            float z2 =
                v.y * sin(angle * 0.7f)
              + z1 * cos(angle * 0.7f);

            float distance = 4.0f;

            float factor =
                distance /
                (z2 + distance);

            int col =
                40 +
                (int)(x1 * factor * 20);

            int row =
                20 +
                (int)(y2 * factor * 10);

            projected[i] = {row, col};
        }

        for (int i = 0; i < 12; i++)
        {
            Point2D a = projected[edges[i][0]];
            Point2D b = projected[edges[i][1]];

            DrawLine(
                Display,
                a.row, a.col,
                b.row, b.col,
                "\033[46m "
            );
        }

        Display.Render();

        angle += 0.05f;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(16)
        );
    }
}
