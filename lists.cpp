#include "lists.h"

#include <cstdint>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include "ansi.h"
#include "win.h"

/**
 * Initialises file structure for loading, and saving lists.
 * Ensures lists folder is generated.
 * @return true if successful, false otherwise
 * @note To be called before any other methods in the class.
 */
bool Lists::init()
{
	std::filesystem::path listsPath("./Lists");

	if (std::filesystem::is_directory(listsPath)) {
		return true;
	}

	if (std::filesystem::is_regular_file(listsPath)) {
		if (!std::filesystem::remove(listsPath)) return false;
	}

	return std::filesystem::create_directory(listsPath);
}

/**
 * Serialises and saves a list of strings to a file.
 * @param filename Name of the file to save the list to.
 * @param list A reference to the list to be saved.
 * @return true if successful, false otherwise
 */
bool Lists::save(const char* filename, const std::vector<std::string>& list)
{
	if (std::filesystem::exists(filename)) return false;

	std::ofstream outFile = std::ofstream(filename, std::ios::out | std::ios::binary);
	if (!outFile) return false;

	for (const std::string& string : list) {
		size_t length = string.length();
		outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
		outFile.write(string.c_str(), length);
	}

	outFile.close();
	return true;
}

/**
 * Deserialises and loads a list of strings from a file.
 * @param filename Name of the file to load the list from.
 * @param list A reference to the list to be saved to.
 * @return true if successful, false otherwise
 */
bool Lists::load(const char* filename, std::vector<std::string>& list)
{
	if (!std::filesystem::exists(filename)) return false;

	std::ifstream inFile = std::ifstream(filename, std::ios::in | std::ios::binary);
	if (!inFile) return false;

	list.clear();

	while (inFile.peek() != EOF) {
		size_t length;
		inFile.read(reinterpret_cast<char*>(&length), sizeof(length));

		std::string string(length, '\0');
		inFile.read(&string[0], length);

		list.push_back(string);
	}

	inFile.close();
	return true;
}

/**
 * Lists all files in the lists directory.
 * @return true if successful, false otherwise
 */
bool Lists::list()
{
	std::filesystem::path listsPath("./Lists");

	std::ostringstream formattedStream;
	short index = 0;

	std::vector<std::string> listOfLists;

	for (const std::filesystem::directory_entry& list : std::filesystem::directory_iterator(listsPath)) {
		if (!list.is_regular_file()) continue;
		if (list.path().extension() != ".bin") continue;

		formattedStream << Fore::YELLOW << "[" << Style::UNDER << std::to_string(index) << Style::RESET <<
			Fore::YELLOW << "]" << Style::RESET << " " << list.path().stem().string();

		listOfLists.push_back(formattedStream.str());

		formattedStream.str("");
		formattedStream.clear();

		index++;
	}

	index = 0;

	for (const std::string& string : listOfLists) {
		short padding = 8;

		if (index + 3 == viewportDepth) {
			short extras = listOfLists.size() - index;
			std::cout << Erase::LINE << std::string(padding, ' ') << Fore::BLACK << "... (+" <<
				std::to_string(extras) << (extras == 1 ? " extra)" : " extras)") << Style::RESET << std::endl;

			break;
		}

		std::cout << Erase::LINE << std::string(padding, ' ') << string << std::endl;
		index++;
	}

	return true;
}
