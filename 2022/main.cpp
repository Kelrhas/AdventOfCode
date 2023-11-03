#include "stdafx.h"
HANDLE hConsole;

#include <assert.h>
#include "stl_includes.h"
#include "Types.h"
#include "Util.h"

namespace Day1	{ void Solve(); }
namespace Day2	{ void Solve(); }
namespace Day3	{ void Solve(); }
namespace Day4	{ void Solve(); }
namespace Day5	{ void Solve(); }
namespace Day6	{ void Solve(); }
namespace Day7	{ void Solve(); }
namespace Day8	{ void Solve(); }
namespace Day9	{ void Solve(); }
namespace Day10 { void Solve(); }
namespace Day11 { void Solve(); }
namespace Day12 { void Solve(); }
namespace Day13 { void Solve(); }
namespace Day14 { void Solve(); }
namespace Day15 { void Solve(); }
namespace Day16 { void Solve(); }
namespace Day17 { void Solve(); }
namespace Day18 { void Solve(); }
namespace Day19 { void Solve(); }
namespace Day20 { void Solve(); }
namespace Day21 { void Solve(); }
namespace Day22 { void Solve(); }
namespace Day23 { void Solve(); }
namespace Day24 { void Solve(); }
namespace Day25 { void Solve(); }



int main(int argc, char **argv)
{
	int day = -1;
	if (argc > 1)
	{
		day = atoi(argv[1]);
	}

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
	cout << "Hello" << endl << endl;
	SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);

	if (day < 0 || day > 25)
	{
		cout << "Enter which day to test, 0 for all" << endl << ">";
		cin >> day;
	}

	LARGE_INTEGER freq, timeStart, timeEnd;
	QueryPerformanceCounter(&timeStart);
	switch (day)
	{
		case 0:
		case 1:		Day1::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 2:		Day2::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 3:		Day3::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 4:		Day4::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 5:		Day5::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 6:		Day6::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 7:		Day7::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 8:		Day8::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 9:		Day9::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 10:	Day10::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 11:	Day11::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 12:	Day12::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 13:	Day13::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 14:	Day14::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 15:	Day15::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 16:	Day16::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 17:	Day17::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 18:	Day18::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 19:	Day19::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 20:	Day20::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 21:	Day21::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 22:	Day22::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 23:	Day23::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 24:	Day24::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		case 25:	Day25::Solve();	if (day != 0) break; else cout << endl; [[fallthrough]];
		default: assert(day == 0); break;
	}


	QueryPerformanceCounter(&timeEnd);
	QueryPerformanceFrequency(&freq);
	cout << endl << "Found in ";
	DisplayTime((timeEnd.QuadPart - timeStart.QuadPart) / (double)freq.QuadPart);
#ifdef _DEBUG
	cout << " in Debug";
#else
	cout << " in Release";
#endif
	cout << endl;
}