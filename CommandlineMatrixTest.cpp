#include <iostream>
#include <complex>
#include <algorithm>
#include <regex>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "matrix.h"
#include "Interpreter.h"

namespace fs = std::filesystem;

// Standard input
int main()
{

	int nRectBrackets = 0;

	std::vector<std::string> lines;
	Interpreter interpreter;
	while (std::cin) {
		std::cout << "BrainCorp_Challenge >";
		std::string inputLine;
		getline(std::cin, inputLine);


		if (Utils::isWhitespace(inputLine))
			continue;

		auto lines = Utils::splitString(inputLine, ";");

		try {
			for (const auto& line : lines)
				interpreter.parse(line);
		}
		catch (std::exception ex)
		{
			std::cerr << ex.what();
		}
	};

	return 0;

}