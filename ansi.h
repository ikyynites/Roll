#ifndef ANSI_H
#define ANSI_H

/**
 * Holds ANSI codes to modify foreground (text) colour.
 */
namespace Fore {
	constexpr const char* DEFAULT = "\x1b[39m";
	constexpr const char* WHITE = "\x1b[97m";
	constexpr const char* BLACK = "\x1b[90m";
	constexpr const char* RED = "\x1b[91m";
	constexpr const char* GREEN = "\x1b[92m";
	constexpr const char* YELLOW = "\x1b[93m";
	constexpr const char* BLUE = "\x1b[94m";
	constexpr const char* MAGENTA = "\x1b[95m";
	constexpr const char* CYAN = "\x1b[96m";
}

/**
 * Holds ANSI codes to modify background colour.
 */
namespace Back {
	constexpr const char* DEFAULT = "\x1b[49m";
	constexpr const char* WHITE = "\x1b[107m";
	constexpr const char* BLACK = "\x1b[100m";
	constexpr const char* RED = "\x1b[101m";
	constexpr const char* GREEN = "\x1b[102m";
	constexpr const char* YELLOW = "\x1b[103m";
	constexpr const char* BLUE = "\x1b[104m";
	constexpr const char* MAGENTA = "\x1b[105m";
	constexpr const char* CYAN = "\x1b[106m";
}

/**
 * Holds ANSI codes to modify style of text printed to console.
 */
namespace Style {
	constexpr const char* RESET = "\x1b[0m";
	constexpr const char* BOLD = "\x1b[1m";
	constexpr const char* DIM = "\x1b[2m";
	constexpr const char* ITALIC = "\x1b[3m";
	constexpr const char* UNDER = "\x1b[4m";
	constexpr const char* DUNDER = "\x1b[5m";
	constexpr const char* BLINKING = "\x1b[6m";
	constexpr const char* REVERSE = "\x1b[7m";
	constexpr const char* HIDDEN = "\x1b[8m";
	constexpr const char* STRIKE = "\x1b[9m";
}

/**
 * Holds ANSI codes to erase text in console.
 */
namespace Erase {
	constexpr const char* LINE = "\x1b[2K";
}

#endif //ANSI_H
