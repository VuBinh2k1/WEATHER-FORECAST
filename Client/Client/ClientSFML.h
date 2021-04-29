#pragma once
#include "ClientSocket.h"
#include "Tokenizer.h"
#include "Button.h"
#include "InputTextBox.h"
#include "Picture.h"

#define $Font			"..\\Data\\ClearSans-Bold.ttf"
#define $Weather		"..\\Data\\png\\icon\\"
#define $Background		"..\\Data\\png\\background.png"
#define $BG_Login		"..\\Data\\png\\login.png"
#define $BG_Regis		"..\\Data\\png\\register.png"
#define $BG_Users		"..\\Data\\png\\user.png"
#define $BG_Admin		"..\\Data\\png\\admin.png"

using namespace sf;
using namespace sock;

class LoginSFML;
class RegisterSFML;
class UserSFML;

/// Main Window
class ClientSFML {
public:
	ClientSocket sock;

	RenderWindow* window;
	Font font;
	//Texture weather_icon[7];
	Picture background;

public:
	static int Log_Reg;
	LoginSFML* win01;
	RegisterSFML* win02;
	UserSFML* win03;

public:
	ClientSFML();
	~ClientSFML();

	void Init(RenderWindow* _win);
	void start();

	void render();
};

/// Member Window
class LoginSFML {
	RenderWindow* window;
	Font font;

	Picture background, LoginBG;
	InputTextBox m_username, m_password;
	Button B_Log, B_Reg;

public:
	LoginSFML();

	void Init(ClientSFML* main);
	void start();

	void render();
};

class RegisterSFML {
	RenderWindow* window;
	Font font;

	Picture background, RegisBG;
	InputTextBox m_username, m_password, m_password2;
	Button B_Log, B_Reg;

public:
	RegisterSFML();

	void Init(ClientSFML* main);
	void start();

	void render();
};

class UserSFML {
	RenderWindow* window;
	Font font;

	Picture background;
	InputTextBox m_city_code, m_date;
	Button B_Search;

	int show_row;
	Text t_city, t_date, t_temp, t_humi, t_stat;
	std::vector<std::string> show;

public:
	UserSFML();

	void Init(ClientSFML* main);
	void start();

	void render();
};
