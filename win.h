#ifndef WIN_H
#define WIN_H

#include <windows.h>

extern short viewportStart;
extern short viewportDepth;

/**
 * Sets the cursor position to a specified coordinate in the console.
 * @param column The column to set the cursor to.
 * @param row The row to set the cursor to.
 * @return true if successful, false otherwise
 */
static bool setCursorPosition(const short column, const short row)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return false;

    COORD coord = {column, row};
    if (!SetConsoleCursorPosition(hConsole, coord)) return false;

    return true;
}

/**
 * Clears the console screen by filling it with spaces and resetting cursor position.
 * Also resets text attributes to default.
 * @return true if successful, false otherwise
 */
static bool clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return false;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return false;

	COORD coordScreen = {static_cast<short>(0), static_cast<short>(0)};
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD cCharsWritten;

	if (!FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten)) return false;

	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)) return false;

	if (!SetConsoleCursorPosition(hConsole, coordScreen)) return false;

	return true;
}

/**
 * Clears the viewport area of the console screen by filling it with spaces and resetting cursor position.
 * @return true if successful, false otherwise
 */
static bool clearViewport()
{
	if (viewportStart == 0 && viewportDepth == 0) return false;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return false;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return false;

	COORD coordScreen = {static_cast<short>(0), viewportStart};
	DWORD dwConSize = csbi.dwSize.X * viewportDepth;
	DWORD cCharsWritten;

	if (!FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten)) return false;

	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)) return false;

	if (!SetConsoleCursorPosition(hConsole, coordScreen)) return false;

	return true;

}

/**
 * Locks the console screen buffer size to the current window size, preventing scrolling.
 * @return true if successful, false otherwise
 */
static bool lockSize(short& width, short& height)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return false;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return false;

	width = static_cast<short>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
	height = static_cast<short>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

	COORD bufferSize = {width, height};
	if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) return false;

	return true;
}

/**
 * Sets the console window title.
 * @param title The new title for the console window.
 * @return true if successful, false otherwise
 */
static bool setTitle(const char* title)
{
	if (!SetConsoleTitleA(title)) return false;

	return true;
}

/**
 * Set visibility of the console cursor.
 * @param visibility true to show the cursor, false to hide it.
 * @return true if successful, false otherwise
 */
static bool setCursor(const bool& visibility)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return false;

	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(hConsole, &cursorInfo)) return false;
	cursorInfo.bVisible = visibility;

	if (!SetConsoleCursorInfo(hConsole, &cursorInfo)) return false;

	return true;
}

/**
 * Enables ANSI escape codes for output in the console.
 * @return true if successful, false otherwise
 */
static bool enableAnsi()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return false;

	DWORD mode;
	if (!GetConsoleMode(hConsole, &mode)) return false;

	if (!SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) return false;

	return true;
}

#endif //WIN_H
