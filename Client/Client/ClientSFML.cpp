#include "stdafx.h"
#include "ClientSFML.h"

/// Main Window
int ClientSFML::Log_Reg = 1;

ClientSFML::ClientSFML() {
	win01 = new LoginSFML;
	win02 = new RegisterSFML;
}

ClientSFML::~ClientSFML() {
	delete win01;
	delete win02;
}

void ClientSFML::Init(RenderWindow* _win) {
	window = _win;
	font.loadFromFile($Font);

	for (int i = 0; i < 7; ++i) {
		std::string path = $Weather;
		path += (i + '0');
		path += ".png";
		weather_icon[i].loadFromFile(path);
	}
	background.setTexture($Background);

	// Init Member window
	win01->Init(this);
	win02->Init(this);
}

void ClientSFML::start() {
	render();

	while (window->isOpen()) {
		Event e;
		if (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window->close();
				return;
			}
		}
		if (sock::ClientSocket::cmd_to_gui == SERV_LOGIN) {
			sock::ClientSocket::cmd_to_gui.clear();
			while(1) {
				if (Log_Reg ==  1) win01->start();
				if (Log_Reg == -1) win02->start();

				if (Log_Reg == 2 || Log_Reg == -2)
					Log_Reg /= 2;
				else break;
			}
			render();
		}

	}
}

void ClientSFML::render() {
	window->clear();
	background.draw(window);

	window->display();
}

/// Member Window
LoginSFML::LoginSFML() : m_username(23, 30), m_password(33, 30, true) {
	LoginBG.setTexture($BG_Login);
	LoginBG.setPosition(std::pair<int, int>(230, 180));
	
	m_username.setPosition(525, 315);
	m_username.setSize(446, 45);
	m_password.setPosition(525, 379);
	m_password.setSize(446, 45);

	B_Log.setPosition(540, 535);
	B_Log.setSize(113, 39);
	B_Reg.setPosition(713, 535);
	B_Reg.setSize(113, 39);
}

void LoginSFML::Init(ClientSFML* main) {
	window = (main->window);
	font = main->font;
	background = main->background;
}

void LoginSFML::start() {
	ClientSFML::Log_Reg = 1;
	m_username.clear();
	m_password.clear();
	render();

	int selectedBOX = 0;
	while (window->isOpen()) {
		Event e;
		if (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window->close();
				return;
			}
			if (e.type == Event::MouseButtonReleased) {
				selectedBOX = 0;
				if (m_username.start(window)) {
					selectedBOX = 1;
				}
				if (m_password.start(window)) {
					selectedBOX = 2;
				}
				if (B_Log.clicked(window)) {
					printf("Log\n");

					if (m_username.size() == 0 || m_password.size() == 0) {
						continue;
					}

					std::string cmd = "LOG\t";
					cmd += m_username.getText() + '\t';
					cmd += m_password.getText();

					sock::ClientSocket::cmd_from_gui = cmd;
					return;
				}
				if (B_Reg.clicked(window)) {
					ClientSFML::Log_Reg = -2;
					return;
				}
				render();
			}
			if (e.type == Event::TextEntered) {
				printf("%d\n", static_cast<char>(e.text.unicode));
				if (e.text.unicode == 13) {
					// Key.Enter
					selectedBOX = 0;
				}
				if (selectedBOX == 1) {
					m_username.input(window, e);
				}
				if (selectedBOX == 2) {
					m_password.input(window, e);
				}
				render();
			}
		}
	}
}

void LoginSFML::render() {
	window->clear();
	background.draw(window);
	LoginBG.draw(window);
	m_username.draw(window);
	m_password.draw(window);
	window->display();
}


RegisterSFML::RegisterSFML() : m_username(23, 30), m_password(33, 30, true), m_password2(33, 30, true) {
	RegisBG.setTexture($BG_Regis);
	RegisBG.setPosition(std::pair<int, int>(230, 180));

	m_username.setPosition(525, 315);
	m_username.setSize(446, 45);
	m_password.setPosition(525, 379);
	m_password.setSize(446, 45);
	m_password2.setPosition(525, 444);
	m_password2.setSize(446, 45);

	B_Log.setPosition(540, 535);
	B_Log.setSize(113, 39);
	B_Reg.setPosition(713, 535);
	B_Reg.setSize(113, 39);
}

void RegisterSFML::Init(ClientSFML* main) {
	window = (main->window);
	font = main->font;
	background = main->background;
}

void RegisterSFML::start() {
	ClientSFML::Log_Reg = -1;
	m_username.clear();
	m_password.clear();
	m_password2.clear();
	render();

	int selectedBOX = 0;
	while (window->isOpen()) {
		Event e;
		if (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window->close();
				return;
			}
			if (e.type == Event::MouseButtonReleased) {
				selectedBOX = 0;
				if (m_username.start(window)) {
					selectedBOX = 1;
				}
				if (m_password.start(window)) {
					selectedBOX = 2;
				}
				if (m_password2.start(window)) {
					selectedBOX = 3;
				}
				if (B_Log.clicked(window)) {
					ClientSFML::Log_Reg = 2;
					return;
				}
				if (B_Reg.clicked(window)) {
					printf("Reg\n");

					if (m_username.size() == 0 || m_password.size() == 0 || m_password2.size() == 0) {
						continue;
					}

					std::string cmd = "REG\t";
					cmd += m_username.getText() + '\t';
					cmd += m_password.getText() + '\t';
					cmd += m_password2.getText();
					
					sock::ClientSocket::cmd_from_gui = cmd;
					return;
				}
				render();
			}
			if (e.type == Event::TextEntered) {
				printf("%d\n", static_cast<char>(e.text.unicode));
				if (e.text.unicode == 13) {
					// Key.Enter
					selectedBOX = 0;
				}
				if (selectedBOX == 1) {
					m_username.input(window, e);
				}
				if (selectedBOX == 2) {
					m_password.input(window, e);
				}
				if (selectedBOX == 3) {
					m_password2.input(window, e);
				}
				render();
			}
		}
	}
}

void RegisterSFML::render() {
	window->clear();
	background.draw(window);
	RegisBG.draw(window);
	m_username.draw(window);
	m_password.draw(window);
	m_password2.draw(window);
	window->display();
}
