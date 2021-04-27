#pragma once
#include "ServerSocket.h"
#include "Button.h"
#include "Picture.h"
#include <fstream>

#define $Log			"..\\Data\\Log\\newest.log"
#define $Font			"..\\Data\\ClearSans-Bold.ttf"
#define $Background		"..\\Data\\png\\background.png"
#define $Weather		"..\\Data\\png\\icon\\"
#define $Button_start	"..\\Data\\png\\icon\\B_start.png"
#define $Button_end		"..\\Data\\png\\icon\\B_end.png"

using namespace sf;
using namespace sock;

class ServerSFML {
	bool isStart;

	ServerSocket sock;

	RenderWindow* window;
	Font font;
	Texture weather_icon[7];
	Picture background;
	RectangleShape windowLog;
	Button B_start, B_end;

public:
	void Init(RenderWindow* _win);
	void start();

	void render();
};

