#include "nwpwin.h"

class STATIC : public vsite::nwp::window
{
public:
	std::string class_name() override { return "STATIC"; }
};

static const int size = 17;
static const DWORD style1 = WS_CHILD | WS_VISIBLE | SS_CENTER;

class main_window : public vsite::nwp::window
{
protected:
	void on_left_button_down(POINT p) override { 
		position = p;
		if (!ship)
			ship.create(*this, style1, "X", 0, p.x, p.y, size, size);
		else
			SetWindowPos(ship, 0, p.x, p.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	void on_key_up(int vk) override {
		DWORD style = GetWindowLong(ship, GWL_STYLE);
		style &= ~WS_BORDER;
		SetWindowLong(ship, GWL_STYLE, style);
		SetWindowPos(ship, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE);
	}
	void on_key_down(int vk) override {
		RECT window_border;
		GetClientRect(*this, &window_border);
		int speed = 20;
		if (GetAsyncKeyState(VK_CONTROL))
			speed = 50;
		if (ship)
		{
			switch (vk)
			{
			case VK_UP:
				position.y = max(position.y - speed, window_border.top);
				break;
			case VK_DOWN:
				position.y = min(position.y + speed, window_border.bottom - size);
				break;
			case VK_LEFT:
				position.x = max(position.x - speed, window_border.left);
				break;
			case VK_RIGHT:
				position.x = min(position.x + speed, window_border.right - size);
				break;
			default:
				return;
			}
			DWORD style = GetWindowLong(ship, GWL_STYLE);
			style |= WS_BORDER;
			SetWindowLong(ship, GWL_STYLE, style);
			SetWindowPos(ship, 0, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}
	void on_destroy() override {
		::PostQuitMessage(0);
	}
private:
	STATIC ship;
	POINT position;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window w;
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 4");
	return app.run();
}
