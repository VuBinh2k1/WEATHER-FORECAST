#include "stdafx.h"
/// window screen only: Proprerties > Linker > System [SubSystem: Windows]
#include <Windows.h>
#include "ServerSFML.h"


//int _tmain(int argc, _TCHAR* argv[])
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//*/
	std::ofstream log($Log);
	log.close();
	 
	RenderWindow window(VideoMode(683, 384), "Weather Forecast Server", Style::Titlebar | Style::Close);
	ServerSFML gui;

	gui.Init(&window);
	gui.start();
	//*/

	return 0;
}
