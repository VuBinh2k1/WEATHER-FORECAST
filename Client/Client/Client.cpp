#include "stdafx.h"
/// window screen only: Proprerties > Linker > System [SubSystem: Windows]
#include <Windows.h>	
#include "ClientSFML.h"

//int _tmain(int argc, _TCHAR* argv[]) 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//*/
	RenderWindow window(VideoMode(1366, 768), "Weather Forecast", Style::Titlebar | Style::Close);
	ClientSFML gui;

	gui.Init(&window);
	gui.start();
	//*/
	return 0;
}