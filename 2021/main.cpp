#include <windows.h>
HANDLE hConsole;

#include "stl_includes.h"
#include "Types.h"
#include "Util.h"

#include "Day1.h"

int main()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
	cout << "Hello" << endl << endl;
	SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);

	auto timeStart = clock();

	Solve();

	auto timeEnd = clock();
	cout << endl << "Found in ";
	DisplayTime(timeEnd - timeStart);
#ifdef _DEBUG
	cout << " in Debug";
#else
	cout << " in Release";
#endif
	cout << endl;
}