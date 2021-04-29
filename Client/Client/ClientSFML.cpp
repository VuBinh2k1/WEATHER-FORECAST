#include "stdafx.h"
#include "ClientSFML.h"

/// Main Window
int ClientSFML::Log_Reg = 1;

ClientSFML::ClientSFML() {
	win01 = new LoginSFML;
	win02 = new RegisterSFML;
	win03 = new UserSFML;
}

ClientSFML::~ClientSFML() {
	delete win01;
	delete win02;
	delete win03;
}

void ClientSFML::Init(RenderWindow* _win) {
	window = _win;
	font.loadFromFile($Font);
	/*/
	for (int i = 0; i < 7; ++i) {
		std::string path = $Weather;
		path += (i + '0');
		path += ".png";
		weather_icon[i].loadFromFile(path);
	}//*/
	background.setTexture($Background);

	// Init Member window
	win01->Init(this);
	win02->Init(this);
	win03->Init(this);
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
		if (sock::ClientSocket::cmd_to_gui == SERV_USER) {
			sock::ClientSocket::cmd_to_gui.clear();
			win03->start();
			break;
		}
		if (sock::ClientSocket::cmd_to_gui == SERV_ADMIN) {
			sock::ClientSocket::cmd_to_gui.clear();
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
				//printf("%d\n", static_cast<char>(e.text.unicode));
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
				//printf("%d\n", static_cast<char>(e.text.unicode));
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


UserSFML::UserSFML() : m_city_code(23, 20), m_date(33, 20) {
	background.setTexture($BG_Users);

	m_city_code.setPosition(22, 345);
	m_city_code.setSize(193, 42);
	m_date.setPosition(22, 429);
	m_date.setSize(192, 42);

	B_Search.setPosition(55, 637);
	B_Search.setSize(127, 39);

	// Row[0]: CITY
	show_row = 1;
	t_city.setFont(font);
	t_date.setFont(font);
	t_temp.setFont(font);
	t_humi.setFont(font);
	t_stat.setFont(font);

	t_city.setCharacterSize(30);
	t_date.setCharacterSize(30);
	t_temp.setCharacterSize(30);
	t_humi.setCharacterSize(30);
	t_stat.setCharacterSize(30);

	t_city.setFillColor(Color::White);
	t_date.setFillColor(Color::White);
	t_temp.setFillColor(Color::White);
	t_humi.setFillColor(Color::White);
	t_stat.setFillColor(Color::White);
}

void UserSFML::Init(ClientSFML* main) {
	window = (main->window);
	font = main->font;
}

std::string curDay() {
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);

	std::string data;
	if (now->tm_mday < 10)
		data = (std::string)"0" + std::to_string(now->tm_mday) + "/";
	else data = std::to_string(now->tm_mday) + "/";

	
	if (now->tm_mon < 9)
		data += (std::string)"0" + std::to_string(now->tm_mon + 1) + "/";
	else data += std::to_string(now->tm_mon + 1) + "/";

	data += std::to_string(now->tm_year + 1900);

	return data;
}

void UserSFML::start() {
	ClientSFML::Log_Reg = -1;
	m_city_code.setText("ALL");
	m_date.setText(curDay());
	render();

	int selectedBOX = 0;
	while (window->isOpen()) {
		Event e;
		if (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window->close();
				return;
			}
			
			if (sock::is(sock::ClientSocket::cmd_to_gui.c_str(), "CITY")) {
				std::string data = sock::ClientSocket::cmd_to_gui;
				
				show.clear();
				show = Tokenizer::split(data, SEP);

				printf("Data: %s\n", data.c_str());
				render();

				sock::ClientSocket::cmd_to_gui.clear();
				sock::ClientSocket::cmd_from_gui = "OK";
			}

			if (sock::ClientSocket::cmd_to_gui == SERV_WAIT) {
				if (e.type == Event::MouseButtonReleased) {
					if (m_city_code.size() == 0) {
						m_city_code.setText("ALL");
					}
					if (m_date.size() == 0) {
						m_date.setText("ALL");
					}
					if (m_city_code.getText() == "ALL" && m_date.getText() == "ALL") {
						m_date.setText(curDay());
					}

					selectedBOX = 0;
					if (m_city_code.start(window)) {
						selectedBOX = 1;
					}
					if (m_date.start(window)) {
						selectedBOX = 2;
					}
					if (B_Search.clicked(window)) {
						std::string cmd = "CITY\t";
						cmd += m_city_code.getText() + '\t';
						cmd += m_date.getText();

						sock::ClientSocket::cmd_to_gui.clear();
						sock::ClientSocket::cmd_from_gui = cmd;
						continue;
						//std::cout << sock::ClientSocket::cmd_from_gui;
					}
					render();
				}
				if (e.type == Event::TextEntered) {
					//printf("%d\n", static_cast<char>(e.text.unicode));
					if (e.text.unicode == 13) {
						// Key.Enter
						selectedBOX = 0;
					}
					if (selectedBOX == 1) {
						m_city_code.input(window, e);
					}
					if (selectedBOX == 2) {
						m_date.input(window, e);
					}
					render();
				}
			}
		}
	}
}

void UserSFML::render() {
	window->clear();
	background.draw(window);
	background.draw(window);
	m_city_code.draw(window);
	m_date.draw(window);

	for (int i = show_row; i < min(show_row + 16, show.size()); ++i) {
		std::vector<std::string> tmp = Tokenizer::split(show[i], ",");
		t_city.setString(tmp[0]);
		t_date.setString(tmp[1]);
		t_temp.setString(tmp[2]);
		t_humi.setString(tmp[3]);
		t_stat.setString(tmp[4]);

		t_city.setPosition(247, 121 + 35 * (i - show_row));
		t_date.setPosition(477, 121 + 35 * (i - show_row));
		t_temp.setPosition(760, 121 + 35 * (i - show_row));
		t_humi.setPosition(995, 121 + 35 * (i - show_row));
		t_stat.setPosition(1149, 121 + 35 * (i - show_row));

		window->draw(t_city);
		window->draw(t_date);
		window->draw(t_temp);
		window->draw(t_humi);
		window->draw(t_stat);
	}

	window->display();
}
