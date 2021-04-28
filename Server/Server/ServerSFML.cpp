#include "stdafx.h"
#include "ServerSFML.h"

void ServerSFML::Init(RenderWindow* _win) {
	isStart = 0;

	window = _win;
	font.loadFromFile($Font);

	background.setTexture($Background);

	windowLog.setFillColor(Color(0, 0, 0, 230));
	windowLog.setPosition(Vector2f(120, 140));
	windowLog.setSize(Vector2f(556, 237));

	B_start.setTexture($Button_start);
	B_start.setPosition(std::pair<int,int>(11, 331));
	B_start.setSize(std::pair<int, int>(96, 25));

	B_end.setTexture($Button_end);
	B_end.setPosition(std::pair<int, int>(11, 331));
	B_end.setSize(std::pair<int, int>(96, 25));
}

void ServerSFML::start() {
	render();

	while (window->isOpen()) {
		if (sock::ServerSocket::newLog) {
			sock::ServerSocket::newLog = 0;
			render();
		}
		Event e;
		if (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window->close();
				return;
			}
			if (e.type == Event::MouseButtonReleased) {
				if (!isStart) {
					if (B_start.clicked(window)) {
						isStart = 1;
						sock.Start();
						render();
					}
				}
				else {
					if (B_end.clicked(window)) {
						isStart = 0;
						sock.End();
						render();
					}
				}
			}
		}
	}
}

void ServerSFML::render() {
	window->clear();
	background.draw(window);
	window->draw(windowLog);
	if (!isStart) {
		B_start.draw(window);
	}
	else {
		B_end.draw(window);
	}

	std::ifstream Log($Log);
	std::vector<std::string> DataLog;
	std::string str;
	while (std::getline(Log, str)) {
		DataLog.push_back(str);
	}
	Log.close();

	int start = DataLog.size() - min(10, DataLog.size());
	for (int i = start; i < DataLog.size() ; ++i) {
		Text textTime, textData;
		textTime.setFont(font);
		textData.setFont(font);
		textTime.setString(DataLog[i].substr(0, 23));
		textData.setString(DataLog[i].substr(24));
		textTime.setCharacterSize(15);
		textData.setCharacterSize(15);
		textTime.setFillColor(Color::Yellow);
		textData.setFillColor(Color::White);
		textTime.setPosition(130, 150 + 20 * (i - start));
		textData.setPosition(320, 150 + 20 * (i - start));
		window->draw(textTime);
		window->draw(textData);
	}

	window->display();
}
