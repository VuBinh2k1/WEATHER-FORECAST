#pragma once
#include "ServerSocket.h"
#include "Button.h"
#include "Picture.h"
#include <fstream>

#define $Font			"..\\Data\\ClearSans-Bold.ttf"
#define $Background		"..\\Data\\png\\background.png"
#define $Button_start	"..\\Data\\png\\icon\\B_start.png"
#define $Button_end		"..\\Data\\png\\icon\\B_end.png"

using namespace sf;
using namespace sock;

class ServerSFML {
	bool isStart;

	ServerSocket sock;

	RenderWindow* window;
	Font font;
	Picture background;
	RectangleShape windowLog;
	Button B_start, B_end;

public:
	void Init(RenderWindow* _win);
	void start();

	void render();
};

