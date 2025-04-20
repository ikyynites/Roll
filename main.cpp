#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <conio.h>
#include <random>
#include <cstdint>
#include <thread>
#include <windows.h>

#include "ansi.h"
#include "lists.h"
#include "win.h"

// global variables
short width;
short height;
short viewportStart = 0;
short viewportDepth = 0;
std::string selectedList = "NONE";
std::vector<std::string> selectedListContents;

std::mt19937 generator(std::random_device{}());

// keycodes for user input
enum Keycodes {
	ENTER = 13,
	ESC = 27,
	DIGIT = 48, // use: DIGIT + n; where n is the digit to be pressed (no offset = 0)
};

/**
 * Functions to print formatted text to the console.
 */
namespace Display
{
	/**
	 * Prints a string to the console at a specified depth from the top.
	 * @param depth How many lines from the top the text should be printed.
	 * @param header The text to be printed.
	 * @param colour Optional colour of the text.
	 */
	void header(const uint8_t& depth, const std::string& header, const char* const& colour = "")
	{
		setCursorPosition(0, depth);
		std::cout << Erase::LINE;

		short padding = width / 2 - header.length() / 2;
		std::cout << std::string(padding, ' ') << colour << header << Style::RESET << std::endl;
	}

	/**
	 * Prints a string to the console at a specified depth from the bottom.
	 * @param depth How many lines from the bottom the text should be printed.
	 * @param footer The text to be printed.
	 * @param colour Optional colour of the text.
	 */
	void footer(const uint8_t& depth, const std::string& footer, const char* const& colour = "")
	{
		setCursorPosition(0, depth - height);
		std::cout << Erase::LINE;

		short padding = width / 2 - footer.length() / 2;
		std::cout << std::string(padding, ' ') << colour << footer << Style::RESET << std::endl;
	}

	/**
	 * Prints out a list of options along the bottom of the console.
	 * @param depth How many lines from the bottom the text should be printed.
	 * @param options List of options to be printed.
	 */
	void options(const uint8_t& depth, const std::vector<std::string>& options)
	{
		if (options.size() >= 9) {
			std::cerr << "Too many... options? Rethink what you're doing." << std::endl;
			std::cerr << "Not enough key binds to select options." << std::endl;
			return;
		}

		setCursorPosition(0, height - depth);
		std::cout << Erase::LINE;

		std::ostringstream outputStream;
		std::ostringstream formattedStream;
		short index = 0;

		for (const std::string& option : options) {
			outputStream << "[" << std::to_string(index) << "] " << option;
			formattedStream << Fore::YELLOW << "[" << Style::UNDER << std::to_string(index) << Style::RESET <<
				Fore::YELLOW << "]" << Style::RESET << " " << option;
			index++;

			if (index != options.size()) {
				outputStream << " | ";
				formattedStream << Fore::BLACK << " | " << Style::RESET;
			}
		}

		std::string output = outputStream.str();
		std::string formatted = formattedStream.str();

		short padding = width / 2 - output.length() / 2;

		std::cout << std::string(padding, ' ') << formatted << Style::RESET << std::endl;
	}

	/**
	 * Prints the currently selected list to the top of the console.
	 */
	void currentList()
	{
		setCursorPosition(0, 4);
		std::cout << Erase::LINE;

		short padding = width / 2 - (selectedList.length() + 15) / 2;

		std::cout << std::string(padding, ' ') << Fore::BLUE << "selected list:" << " " <<
			(selectedList == "NONE" ? Fore::BLACK : Fore::DEFAULT) << selectedList << Style::RESET << std::endl;
	}

	/**
	 * Prints information about the program along the bottom row of the console.
	 */
	void info()
	{
		setCursorPosition(0, height - 1);
		std::cout << Erase::LINE;

		std::string esc = " press ESC to exit";
		std::string version = "C++ V" + std::to_string(__cplusplus);
		short padding = width - (esc.length() + version.length());

		std::cout << Fore::BLACK << esc << std::string(padding, ' ') << version << Style::RESET;
	}

	/**
	 * Cleans blank lines in the display.
	 */
	void clean()
	{
		setCursorPosition(0, 0);
		std::cout << Erase::LINE;

		setCursorPosition(0, 3);
		std::cout << Erase::LINE;

		setCursorPosition(0, 5);
		std::cout << Erase::LINE;

		setCursorPosition(0, height - 3);
		std::cout << Erase::LINE;

		setCursorPosition(0, height - 2);
		std::cout << Erase::LINE;

		setCursorPosition(0, height - 5);
		std::cout << Erase::LINE;
	}
}

void roll()
{
	short centerWidth = width / 2;
	short viewportCenter = viewportStart + (short)(viewportDepth / 2);

	if (selectedList == "NONE") {
		setCursorPosition(centerWidth - 13, viewportCenter);
		std::cout << Erase::LINE << Fore::RED << "please select a list first!" << Style::RESET;

		return;
	}

	std::size_t index = 0;
	std::size_t length = selectedListContents.size();
	std::uniform_int_distribution<std::size_t> distribution(0, length - 1);

	std::size_t randomIndex = distribution(generator);
	std::size_t iterations = randomIndex + (length * 5);

	for (int i = 0; i < iterations; i++) {
		PlaySound("./click.wav", nullptr, SND_FILENAME | SND_ASYNC);

		index = (index + 1) % length;
		std::size_t prevIndex = (index - 1 + length) % length;
		std::size_t nextIndex = (index + 1) % length;

		setCursorPosition(centerWidth - selectedListContents[prevIndex].length() / 2, viewportCenter - 2);
		std::cout << Erase::LINE << Fore::BLACK << selectedListContents[prevIndex] << Style::RESET;

		setCursorPosition(centerWidth - selectedListContents[nextIndex].length() / 2, viewportCenter + 2);
		std::cout << Erase::LINE << Fore::BLACK << selectedListContents[nextIndex] << Style::RESET;

		setCursorPosition(centerWidth - selectedListContents[index].length() / 2 - 3, viewportCenter);
		std::cout << Erase::LINE << " > " << selectedListContents[index] << " < ";

		std::this_thread::sleep_for(std::chrono::milliseconds(100 + i * 30));
	}

	PlaySound("./ding.wav", nullptr, SND_FILENAME | SND_ASYNC);

	setCursorPosition(centerWidth - 9, viewportCenter + 5);
	std::cout << Erase::LINE << Fore::BLACK << "(press 'F' to continue)" << Style::RESET;

	while (!(GetKeyState('F') & 0x8000)) {
		setCursorPosition(centerWidth - selectedListContents[index].length() / 2 - 3, viewportCenter);
		std::cout << Erase::LINE << " > " << selectedListContents[index] << " < ";

		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		setCursorPosition(centerWidth - selectedListContents[index].length() / 2 - 3, viewportCenter);
		std::cout << Erase::LINE << ">  " << selectedListContents[index] << "  <";

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
}

int main()
{
	// configure console
	if (!(setTitle("Roll") && lockSize(width, height) && setCursor(false) && enableAnsi() && clearScreen())) {
		std::cerr << "Failed to configure console." << std::endl;
		return 1;
	}
	if (height < 21) {
		std::cerr << "Console height is too small." << std::endl;
        return 1;
	}

	viewportStart = 6;
	viewportDepth = height - 5 - viewportStart;

	// initialise classes
	if (!Lists::init()) {
		std::cerr << "Failed to initialise lists." << std::endl;
		return 1;
	}

	std::vector<std::string> options = {"roll", "new list", "select list"};
	std::vector<std::string> names = {"callum", "miesha", "olivia", "eleanor"};

	// main loop
	while (true) {
		Display::header(1, "welcome to roll.", Fore::YELLOW);
		Display::header(2, "by callum wakefield", Fore::BLACK);
		Display::currentList();
		Display::info();
		Display::options(4, options);

		Display::clean();
		clearViewport();

		int key = _getch();

		if (key == ESC) {
			break;
		}
		if (key == DIGIT) { // roll
			roll();

			continue;
		}
		if (key == DIGIT + 1) { // new list
			continue;
		}
		if (key == DIGIT + 2) { // select list
            Lists::list();

			std::filesystem::path listsPath("./Lists");
			short padding = 8;

			setCursorPosition(0, height - 4);
			std::cout << Erase::LINE << std::endl << Erase::LINE << std::endl;

			setCursorPosition(padding, height - 4);

			std::string input;
			std::cout << "list: ";

			setCursor(true);
			std::getline(std::cin, input);
			setCursor(false);

			while (!std::filesystem::exists(listsPath / (input + ".bin"))) {
				std::cout << Erase::LINE << std::string(padding, ' ') << Fore::RED << "list \"" << input << "\" does not exist." << Style::RESET;
				std::cout << Cursor::PREV << Erase::LINE << std::string(padding, ' ') << "list: ";

				setCursor(true);
				std::getline(std::cin, input);
				setCursor(false);
			}

			selectedList = input;
			Lists::load((listsPath / (input + ".bin")).string().c_str(), selectedListContents);
        }
	}

	return 0;
}
