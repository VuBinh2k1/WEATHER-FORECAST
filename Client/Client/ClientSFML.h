#pragma once
#include "ClientSocket.h"
#include "Button.h"
#include "InputTextBox.h"
#include "Picture.h"

#define $Font			"..\\Data\\ClearSans-Bold.ttf"
#define $Weather		"..\\Data\\png\\icon\\"
#define $Background		"..\\Data\\png\\background.png"
#define $BG_Login		"..\\Data\\png\\login.png"

using namespace sf;
using namespace sock;

class ClientSFML {
public:
	ClientSocket sock;

	RenderWindow* window;
	Font font;
	Texture weather_icon[7];
	Picture background;

public:
	void Init(RenderWindow* _win);
	void start();

	void render();
};

class LoginSFML : public ClientSFML {
	Picture LoginBG;
	InputTextBox m_username, m_password;

public:
	LoginSFML();

	void render();
};

class UserSFML : public ClientSFML {

};

class AdminSFML : public ClientSFML {

};
