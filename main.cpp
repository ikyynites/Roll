#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <cstdint>

#include "ansi.h"
#include "lists.h"
#include "win.h"

short width;
short height;

enum Keycodes {
    ENTER = 13,
    ESC = 27,
	DIGIT = 48, // use: DIGIT + n; where n is the digit to be pressed (no offset = 0)
};

/**
 * Prints a string to the console at a specified depth from the top.
 * @param depth How many lines from the top the text should be printed.
 * @param header The text to be printed.
 * @param colour Optional colour of the text.
 */
void header(const uint8_t& depth, const std::string& header, const char* const& colour)
{
	setCursorPosition(0, 0);
	for (int i = 0; i < depth; i++) {
		std::cout << std::endl;
	}
	std::cout << Erase::LINE;

	short padding = width / 2 - header.length() / 2;
	std::cout <<  std::string(padding, ' ') << colour << header << Style::RESET << std::endl;
}

/**
 * Prints a string to the console at a specified depth from the bottom.
 * @param depth How many lines from the bottom the text should be printed.
 * @param footer The text to be printed.
 * @param colour Optional colour of the text.
 */
void footer(const uint8_t& depth, const std::string& footer, const char* const& colour = "")
{
	setCursorPosition(0, 0);
	for (int i = 0; i < height - depth; i++) {
        std::cout << std::endl;
    }
	std::cout << Erase::LINE;

	short padding = width / 2 - footer.length() / 2;
	std::cout << std::string(padding, ' ') << colour << footer << Style::RESET << std::endl;
}

/**
* Prints out a list of options along the bottom of the console.
* @param options List of options to be printed.
*/
void options(const std::vector<std::string>& options) {
	setCursorPosition(0, 0);
	for (int i = 0; i < height - 4; i++) {
		std::cout << std::endl;
	}
	std::cout << Erase::LINE;

	std::ostringstream outputStream;
	std::ostringstream formattedStream;
	short index = 0;

	for (const std::string& option : options) {
		outputStream << "[" + std::to_string(index) + "] " + option;
		formattedStream << Fore::YELLOW + "[" + std::to_string(index) + "]" + Style::RESET + " " + option;
		index++;

		if (index != options.size()) {
			outputStream << " | ";
			formattedStream << Fore::BLACK + " | " + Style::RESET;
		}
	}

	std::string output = outputStream.str();
	std::string formatted = formattedStream.str();

	short padding = width / 2 - output.length() / 2;

	std::cout << std::string(padding, ' ') << formatted << Style::RESET << std::endl;
}

/**
 * Prints information about the program along the bottom row of the console.
 */
void info()
{
	setCursorPosition(0, 0);
	for (int i = 0; i < height; i++) {
        std::cout << std::endl;
    }
	std::cout << Erase::LINE;

	std::string esc = "press ESC to exit";
	std::string version = "C++ V" + std::to_string(__cplusplus);
	short padding = width - (esc.length() + version.length());

	std::cout << Fore::BLACK << esc << std::string(padding, ' ') << version << Style::RESET;
}

int main()
{
	// configure console
	if (!(setTitle("Roll") && lockSize(width, height) && hideCursor() && enableAnsi() && clearScreen())) {
		std::cerr << "Failed to configure console." << std::endl;
		return 1;
	}

	// initialise classes
	if (!Lists::init()) {
		std::cerr << "Failed to initialise lists." << std::endl;
		return 1;
	}

	// main loop
	while (true) {
		header(2, "welcome to roll.", Fore::YELLOW);
		header(3, "by callum wakefield", Fore::BLACK);
		info();

		int key = _getch();

		if (key == ESC) {
			break;
		}
		if (key == DIGIT + 1) {
			break;
		}
	}

	return 0;
}
