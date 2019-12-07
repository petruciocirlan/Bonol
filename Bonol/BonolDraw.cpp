#include "Bonol.h"
/*
	add all necessary graphical libraries
	set compiler/linker options to include them properly
*/

#define KEY_ESC 27
void Bonol::BonolDraw(Bonol* game)
{
    const int WIDTH = 800, HEIGHT = 600;
    const int LEFT = (getmaxwidth() - WIDTH) / 2;
    const int TOP = (getmaxheight() - HEIGHT) / 2;
    initwindow(WIDTH, HEIGHT, "Mouse Drag Test", LEFT, TOP);

    bgiout <<
        "Draw by holding down the mouse button\n"
        "Exit by pressing the ESCAPE key\n";
    outstreamxy(10, HEIGHT - 50);

    bool Running = true;

    int x_old = 0, y_old = 0;
    bool mouse_down = false;
    int brush_size = 5, brush_color_selected = 0, brush_color_count = 3;
    int brush_color_palette[] = { WHITE, WHITE, WHITE };///{ RED, GREEN, BLUE };
    const int MAX_BRUSH_SIZE = 50;
    const int MIN_BRUSH_SIZE = 1;

    while (Running)
    {
        //swapbuffers();
        if (kbhit())
        {
            switch (getch())
            {
            case KEY_ESC:
            {
                Running = false;
                break;
            }
            case KEY_UP:
            {
                ++brush_size;
                if (brush_size > MAX_BRUSH_SIZE)
                    brush_size = MAX_BRUSH_SIZE;
                break;
            }
            case KEY_DOWN:
            {
                --brush_size;
                if (brush_size < MIN_BRUSH_SIZE)
                    brush_size = MIN_BRUSH_SIZE;
                break;
            }
            case KEY_LEFT:
            {
                --brush_color_selected;
                if (brush_color_selected < 0)
                    brush_color_selected += brush_color_count;
                break;
            }
            case KEY_RIGHT:
            {
                ++brush_color_selected;
                if (brush_color_selected >= brush_color_count)
                    brush_color_selected = 0;
                break;
            }
            }
        }

        if (!Running) break;

        if (mouse_down)
        {
            int x_new = mousex();
            int y_new = mousey();

            /// HOW TO GET RID OF ANNOYING WHITE BORDER?
            /// IDEA: MAKE OWN DRAW FUNCTION
            int color = brush_color_palette[brush_color_selected];
            long long pattern = -1;
            setfillpattern((char*)&pattern, color);
            fillellipse(x_new, y_new, brush_size, brush_size);
            //line(x_old, y_old, x_new, y_new);
            //swapbuffers();
            //line(x_old, y_old, x_new, y_new);

            x_old = x_new, y_old = y_new;

            if (ismouseclick(WM_LBUTTONUP))
            {
                getmouseclick(WM_LBUTTONUP, x_old, y_old);
                mouse_down = false;
            }
        }
        else
        {
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                getmouseclick(WM_LBUTTONDOWN, x_old, y_old);
                mouse_down = true;
            }
        }
    }
	/// draw game state to screen
}