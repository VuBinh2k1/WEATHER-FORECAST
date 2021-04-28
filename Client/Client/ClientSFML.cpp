#include "stdafx.h"
#include "ClientSFML.h"

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


	}
}

void ClientSFML::render() {
	window->clear();
	background.draw(window);

	window->display();
}

LoginSFML::LoginSFML() : m_username(40, 30), m_password(40, 30) {
	LoginBG.setTexture($BG_Login);
	LoginBG.setPosition(std::pair<int, int>(230, 150));
	
	m_username.setPosition(525, 315);
	m_username.setSize(446, 45);

	m_password.setPosition(525, 379);
	m_password.setSize(446, 45);
}

void LoginSFML::render() {
	window->clear();
	background.draw(window);
	LoginBG.draw(window);

	window->display();
}
