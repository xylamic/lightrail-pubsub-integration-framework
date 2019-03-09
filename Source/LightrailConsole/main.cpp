#include <iostream>
#include "LightrailConsole.h"

using namespace std;

void main()
{
#ifdef _WIN32
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	{
		LightrailConsole console(L"name", L"desc");
		console.Run();
	}

	cin.get();
}