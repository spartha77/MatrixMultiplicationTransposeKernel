#include "Utils.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

bool Utils::is_number(const std::string& str_num)
{
	/*return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();*/


	int i = 0;
	size_t str_len = str_num.size();
	while (i < str_len && str_num[i] == ' ')
		i++;
	if (i < str_len && (str_num[i] == '+' || str_num[i] == '-'))
		i++;
	int digits = 0, dots = 0;
	while (i < str_len && ((str_num[i] >= '0' && str_num[i] <= '9') || (str_num[i] == '.'))) {
		if (str_num[i] >= '0' && str_num[i] <= '9')
			digits++;
		else if (str_num[i] == '.')
			dots++;
		i++;
	}
	if (digits == 0 || dots > 1)
		return false;
	if (i < str_len && str_num[i] == 'e') {
		if (++i < str_len && (str_num[i] == '+' || str_num[i] == '-'))
			i++;
		if (i == str_len || (str_num[i] < '0' || str_num[i]>'9'))
			return false;
		while (i < str_len && (str_num[i] >= '0' && str_num[i] <= '9'))
			i++;
	}
	while (i < str_len && str_num[i] == ' ')  //optional trailing spaces
		i++;
	return (i == str_len); //if at end of string then success
}

bool Utils::is_string_in_strings(const std::vector<std::string>& strings, const std::string& str)
{
	auto iter = std::find_if(strings.begin(), strings.end(), [&str](const std::string& thisStr) {if (thisStr == str) return true; return false; });
	if (iter != strings.end())
		return true;
	return false;
}

bool Utils::isClearStatement(const std::string& statement)
{
	auto tk = statement;
	tk.erase(std::remove_if(tk.begin(), tk.end(), ::isspace), tk.end());
	if (Utils::toLowerCase(tk) == "clear")
		return true;
	return false;
}
void Utils::removeCharFromString(std::string& str, const char& ch)
{
	str.erase(remove(str.begin(), str.end(), ch), str.end()); //remove A from string
}

bool Utils::isMultiplyStatement(const std::string& statement)
{
	return findDelimInString(statement, "*");
}

bool Utils::isTransposeStatement(const std::string& statement)
{
	return findDelimInString(statement, "^");
}

bool Utils::isAssignmentStatement(const std::string& statement)
{
	return findDelimInString(statement, "=");
}

bool Utils::isNoSymbolStatement(const std::string& statement)
{
	return (!isMultiplyStatement(statement) && !isTransposeStatement(statement) && !(isAssignmentStatement(statement)));
}



bool Utils::isExecutionStatement(const std::string& statement)
{
	if (isMultiplyStatement(statement) || isTransposeStatement(statement))
		return true;


	if (isAssignmentStatement(statement) && isTransposeStatement(statement))
		return true;

	if (isAssignmentStatement(statement))
	{
		auto tokens = Utils::getExclusiveInputStatementTokens(statement);
		if (tokens.size() == 3)
		{
			try {
				fs::path fp(tokens[2]);
				if (fs::exists(fp))
					return false; // not execution statement but read statement
			}
			catch (...)
			{
				return true;
			}
			return true;
		}

		return false;
		
	}


	if (isNoSymbolStatement(statement))
		return true;

	return false;
}
bool Utils::isInlineReadWithDataType(const std::string& statement)
{
	bool sofarsogood = true;

	//auto tokens = splitString(statement, " \t,()[]{}=");
	auto tokens = getInputStatementTokens(statement);

	for (size_t ii = 0; ii < tokens.size(); ii++)
	{
		if (sofarsogood == false)
			break;

		// Checking for inline
		switch (ii)
		{
		case 0:
			if (is_number(tokens[ii]))
				sofarsogood = false;
			break;
		case 1:
			if (tokens[ii] != "=" )
				sofarsogood = false;
			break;
		case 2:
			if (is_number(tokens[ii]))
				sofarsogood = false;
			break;
		default:
			if (!is_number(tokens[ii]))
				sofarsogood = false;
			break;
		}
	}

	return sofarsogood;
}

bool Utils::isInlineReadWithNoDataType(const std::string& statement)
{
	bool sofarsogood = true;
	auto tokens = getInputStatementTokens(statement);
	for (size_t ii = 0; ii < tokens.size(); ii++)
	{
		if (sofarsogood == false)
			break;

		// Checking for inline
		switch (ii)
		{
		case 0:
			if (is_number(tokens[ii]))
				sofarsogood = false;
			break;
		case 1:
			if (tokens[ii] != "=")
				sofarsogood = false;
			break;
		default:
			if (!is_number(tokens[ii]))
				sofarsogood = false;
			break;
		}
	}

	return sofarsogood;
}

bool Utils::isInlineFileReadType(const std::string& statement)
{
	bool sofarsogood = true;
	bool secondArgumentAFile = false;
	//auto tokens = splitString(statement, " \t,()[]{}=");
	auto tokens = getInputStatementTokens(statement);
	if (tokens.size() != 3)
		return false;
	{
		fs::path fsPath(tokens[2]);
		std::vector<std::string> allLinesVec;
		std::string combinedFileReadString;
		std::fstream inFile;
		try {
			inFile.open(fsPath.string(), std::fstream::in);
		}
		catch (std::exception ex)
		{
			inFile.close();
			/*std::stringstream ss;
			ss << "Unable to open the file " << tokens[2] << " Exiting.\n";
			throw std::runtime_error(ss.str());*/
			//inFile.close();
		}
		inFile.close();
		secondArgumentAFile = true;
	}
	for (size_t ii = 0; ii < tokens.size(); ii++)
	{
		if (sofarsogood == false)
			break;

		// Checking for inline
		switch (ii)
		{
		case 0:
			if (is_number(tokens[ii]))
				sofarsogood = false;
			break;
		case 1:
			if ( tokens[ii] != "=")
				sofarsogood = false;
			break;
		case 2:
			if (is_number(tokens[ii]) || !secondArgumentAFile )
				sofarsogood = false;
			break;
		default:
			sofarsogood = false;
			break;
		}
	}

	return sofarsogood;
}

unsigned Utils::stou(std::string const& str, size_t* idx, int base) {
	unsigned long result = std::stoul(str, idx, base);
	if (result > std::numeric_limits<unsigned>::max()) {
		throw std::out_of_range("stou");
	}
	return result;
}

bool Utils::isFileReadWithDataType(const std::string& statement)
{
	bool sofarsogood = true;
	auto tokens = splitString(statement, " \t,()[]{}=");
	for (size_t ii = 0; ii < tokens.size(); ii++)
	{
		if (sofarsogood == false)
			break;

		// Checking for inline
		switch (ii)
		{
		case 0:
			if (is_number(tokens[ii]))
				sofarsogood = false;
			break;
		default:
			if (!is_number(tokens[ii]))
				sofarsogood = false;
			break;
		}
	}

	return sofarsogood;
}

bool Utils::isFileReadWithNoDataType(const std::string& statement)
{
	bool sofarsogood = true;
	auto tokens = splitString(statement, " \t,()[]{}=");
	for (size_t ii = 0; ii < tokens.size(); ii++)
	{
		if (sofarsogood == false)
			break;

		// Checking for inline

		if (!is_number(tokens[ii]))
			sofarsogood = false;
	}

	return sofarsogood;
}

std::string Utils::toLowerCase(const std::string& token)
{
	std::string tk = token;
	std::transform(tk.begin(), tk.end(), tk.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return tk;
}

NumberType Utils::getNumberType(const std::string& token)
{
	std::string tk = token;
	tk.erase(std::remove_if(tk.begin(), tk.end(), ::isspace), tk.end());
	NumberType nType = NumberType::double_type;
	std::string lower_case_token = toLowerCase(tk);
	if (tk == "int")
		nType = NumberType::int_type;
	else if (tk == "long_int")
		nType = NumberType::long_int_type;
	else if (tk == "float")
		nType = NumberType::float_type;
	else if (tk == "double")
		nType = NumberType::double_type;
	else if (tk == "long_double")
		nType = NumberType::long_double_type;
	else if (tk == "complex_int")
		nType = NumberType::complex_int_type;
	else if (tk == "complex_float")
		nType = NumberType::complex_float_type;
	else if (tk == "complex_double")
		nType = NumberType::complex_double_type;
	else if (tk == "complex_long_int")
		nType = NumberType::complex_long_int_type;
	else if (tk == "complex_long_double>")
		nType = NumberType::complex_long_double_type;

	return nType;
}

bool Utils::isComplexType(const NumberType& nt)
{
	if (nt == NumberType::complex_double_type ||
		nt == NumberType::complex_float_type ||
		nt == NumberType::complex_int_type ||
		nt == NumberType::complex_long_double_type ||
		nt == NumberType::complex_long_int_type)
		return true;
	return false;
}

std::vector<std::string> Utils::split2tokens(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

//bool Utils::is_number(const std::string& str_num)
//{
//	int i = 0;
//	size_t str_len = str_num.size();
//	while (i < str_len && str_num[i] == ' ')
//		i++;
//	if (i < str_len && (str_num[i] == '+' || str_num[i] == '-'))
//		i++;
//	int digits = 0, dots = 0;
//	while (i < str_len && ((str_num[i] >= '0' && str_num[i] <= '9') || (str_num[i] == '.'))) {
//		if (str_num[i] >= '0' && str_num[i] <= '9')
//			digits++;
//		else if (str_num[i] == '.')
//			dots++;
//		i++;
//	}
//	if (digits == 0 || dots > 1)
//		return false;
//	if (i < str_len && str_num[i] == 'e') {
//		if (++i < str_len && (str_num[i] == '+' || str_num[i] == '-'))
//			i++;
//		if (i == str_len || (str_num[i] < '0' || str_num[i]>'9'))
//			return false;
//		while (i < str_len && (str_num[i] >= '0' && str_num[i] <= '9'))
//			i++;
//	}
//	while (i < str_len && str_num[i] == ' ')  //optional trailing spaces
//		i++;
//	return (i == str_len); //if at end of string then success
//}

bool Utils::findDelimInString(const std::string& str, const std::string& pattern, size_t& pos)
{
	for (const auto& ch : pattern)
	{
		pos = str.find(ch);
		if (pos != std::string::npos)
			return true;
	}
	return false;
}

bool Utils::findDelimInString(const std::string& str, const std::string& pattern)
{
	size_t pos = 0u;
	for (const auto& ch : pattern)
	{
		pos = str.find(ch);
		if (pos != std::string::npos)
			return true;
	}
	return false;
}

std::vector<std::string> Utils::splitString(const std::string& inputString, const std::string& delims)
{
	std::vector<std::string> wordVector;
	std::stringstream stringStream(inputString);
	std::string line;
	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(delims, prev)) != std::string::npos)
		{
			if (pos > prev)
				wordVector.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
			wordVector.push_back(line.substr(prev, std::string::npos));
	}

	return wordVector;
}

bool Utils::isWhitespace(std::string s) {
	for (int index = 0; index < s.length(); index++) {
		if (!std::isspace(s[index]))
			return false;
	}
	return true;
}

std::vector<std::string> Utils::tokenizeStatement(const std::string& strInput, const std::string& strDelims)
{
	std::vector<std::string> outputVec;

	std::string strOne = strInput;
	std::string delimiters = strDelims;

	size_t startpos = 0;
	size_t pos = strOne.find_first_of(delimiters, startpos);
	if (startpos == std::string::npos)
	{
		outputVec.push_back(strInput);
	}
	while (std::string::npos != pos && std::string::npos != startpos)
	{
		if (strOne.substr(startpos, pos - startpos) != "")
			outputVec.push_back(strOne.substr(startpos, pos - startpos));

		// if delimiter is a new line (\n) then addt new line
		if (strOne.substr(pos, 1) == "\n")
			outputVec.push_back("\\n");
		// else if the delimiter is not a space
		else if (strOne.substr(pos, 1) != " ")
			outputVec.push_back(strOne.substr(pos, 1));

		if (std::string::npos == strOne.find_first_not_of(delimiters, pos))
			startpos = strOne.find_first_not_of(delimiters, pos);
		else
			startpos = pos + 1;

		pos = strOne.find_first_of(delimiters, startpos);

		if (std::string::npos == pos && startpos != std::string::npos)
		{
			std::string last_token;
			outputVec.push_back(strOne.substr(startpos, strOne.length() - 1));
		}
	}

	return outputVec;
}

std::vector<std::string> Utils::getExecutionStatementTokens(const std::string& statement)
{
	std::vector<std::string> statementTokens;
	if (!findDelimInString(statement, "*"))
		return statementTokens;
	std::string modifiedTokens;
	std::vector<std::string> tokens = Utils::splitString(statement, " \t,()[]{}");
	for (const auto& token : tokens)
		modifiedTokens += token;
	statementTokens = Utils::tokenizeStatement(modifiedTokens, "*=");
	return statementTokens;
}

std::vector<std::string> Utils::getInputStatementTokens(const std::string& statement)
{
	std::vector<std::string> statementTokens;
	if (findDelimInString(statement, "*^"))
		return statementTokens;

	std::vector<std::string> tokens = Utils::splitString(statement, " \t,()[]{};");
	
	bool vecOutOfBounds = false;
	for (auto iter = tokens.begin(); iter != tokens.end(); iter++)
	{
		auto tknsAttachedWithEQ = Utils::tokenizeStatement(*iter, "=");
		if (!tknsAttachedWithEQ.empty())
		{
			// Erase iter, add the new tokens
			iter = tokens.erase(iter);
			for (const auto tkn : tknsAttachedWithEQ)
			{
				if (vecOutOfBounds)
				{
					tokens.push_back(tkn);
					iter = std::prev(tokens.end());
				}
				else
				{
					iter = tokens.insert(iter, tkn);
					iter++;
				}
				if (iter == tokens.end())
				{
					vecOutOfBounds = true;
				}
			}
		}
	}

	return tokens;
}

std::vector<std::string> Utils::getExclusiveInputStatementTokens(const std::string& statement)
{
	std::vector<std::string> tokens = Utils::splitString(statement, " \t,()[]{};");

	bool vecOutOfBounds = false;
	for (auto iter = tokens.begin(); iter != tokens.end(); iter++)
	{
		auto tknsAttachedWithEQ = Utils::tokenizeStatement(*iter, "=");
		if (!tknsAttachedWithEQ.empty())
		{
			// Erase iter, add the new tokens
			iter = tokens.erase(iter);
			for (const auto tkn : tknsAttachedWithEQ)
			{
				if (vecOutOfBounds)
				{
					tokens.push_back(tkn);
					iter = std::prev(tokens.end());
				}
				else
				{
					iter = tokens.insert(iter, tkn);
					iter++;
				}
				if (iter == tokens.end())
				{
					vecOutOfBounds = true;
				}
			}
		}
	}

	return tokens;
}

std::vector<std::string> Utils::getInputStatementTokensWithTransposeDelimeters(const std::string& statement)
{
	std::vector<std::string> statementTokens;
	if (findDelimInString(statement, "*"))
		return statementTokens;

	std::vector<std::string> tokens = Utils::splitString(statement, " \t,()[]{};");

	bool vecOutOfBounds = false;
	for (auto iter = tokens.begin(); iter != tokens.end(); iter++)
	{
		auto tknsAttachedWithEQ = Utils::tokenizeStatement(*iter, "^");
		if (!tknsAttachedWithEQ.empty())
		{
			// Erase iter, add the new tokens
			iter = tokens.erase(iter);
			for (const auto tkn : tknsAttachedWithEQ)
			{
				if (vecOutOfBounds)
				{
					tokens.push_back(tkn);
					iter = std::prev(tokens.end());
				}
				else
				{
					iter = tokens.insert(iter, tkn);
					iter++;
				}
				if (iter == tokens.end())
				{
					vecOutOfBounds = true;
				}
			}
		}
	}

	return tokens;
}