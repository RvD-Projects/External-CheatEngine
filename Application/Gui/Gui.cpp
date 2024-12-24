#include "Gui.h"

bool Gui::init(HWND hwnd)
{
	return true;
}

void Gui::frame()
{
	handle_events();
}

void Gui::destroy()
{
	
}

void Gui::handle_events()
{
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
}

