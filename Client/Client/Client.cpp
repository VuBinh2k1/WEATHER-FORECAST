#include "stdafx.h"
#include "ClientSFML.h"

int _tmain(int argc, _TCHAR* argv[]) {
	RenderWindow window(VideoMode(1366, 768), "Weather Forecast", Style::Titlebar | Style::Close);
	ClientSFML gui;

	gui.Init(&window);
	gui.start();
	

	/*/
	do {
		
	} while (Handle(sock));
	//*/
	
	return 0;
}