#include "stdafx.h"
#include "ServerSFML.h"

int _tmain(int argc, _TCHAR* argv[]) {
	std::ofstream log($Log);
	log.close();
	 
	RenderWindow window(VideoMode(683, 384), "Weather Forecast Server", Style::Titlebar | Style::Close);
	ServerSFML gui;

	gui.Init(&window);
	gui.start();
	

	return 0;
}
