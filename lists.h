#ifndef LISTS_H
#define LISTS_H

#include <string>
#include <vector>

class Lists {
public:
	static bool init();

	static bool save(const char* filename, const std::vector<std::string>& list);

	static bool load(const char* filename, std::vector<std::string>& list);

	static bool list();
};

#endif //LISTS_H
