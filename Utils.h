#pragma once
#include <string>

#include <optional>
#include <type_traits>
#include <sstream>
#include <filesystem>
#include <map>
#include <variant>

namespace fs = std::filesystem;

#include "matrix.h"
//
//
//namespace std {
//	template <typename _CharT, typename _Traits>
//	inline basic_ostream<_CharT, _Traits>&
//		tab(basic_ostream<_CharT, _Traits>& __os) {
//		return __os.put(__os.widen('\t'));
//	}
//}
static int __matrixNum = 0;
static std::string getMatrixName()
{
	std::string matName("Mult_Mat");
	std::stringstream ss;
	ss << matName << "_" << ++__matrixNum;
	return std::string(ss.str());
}



enum class StatementType
{
	InlineReadWithDataType, // a = int, 2,3,4,5,6,7,8,9
	InlineReadWithNoDataType, // a = 2,3,4,5,6,7,8,9
	InlineFileReadType, // a = InfultFileWithPath
	FileReadWithDataType, // int,2,3,4,5,6,7,8,9
	FileReadWithNoDataType, //2,3,4,5,6,7,8,9
	Execution,
	None
};

enum class ExecutionSubType
{
	Multiply,
	DisplayTranspose,
	TransposeAndAssignment,
	Clear,
	SimpleAssignment,
	Display,
	None
};



enum class NumberType
{
	int_type,
	float_type,
	double_type,
	long_double_type,
	long_int_type,
	complex_int_type,
	complex_float_type,
	complex_double_type,
	complex_long_double_type,
	complex_long_int_type,
	none
};


struct Utils
{
	static bool is_number(const std::string& str_num);
	static bool is_string_in_strings(const std::vector<std::string>& strings, const std::string& str);

	static unsigned stou(std::string const& str, size_t* idx = 0, int base = 10);
	static bool isWhitespace(std::string s);
	static std::vector<std::string> splitString(const std::string& inputString, const std::string& delims);
	static bool findDelimInString(const std::string& str, const std::string& pattern, size_t& pos);
	static bool findDelimInString(const std::string& str, const std::string& pattern);
	static std::vector<std::string> split2tokens(std::string s, std::string delimiter);
	static bool isClearStatement(const std::string& statement);
	static void removeCharFromString(std::string& str, const char& ch);
	static bool isExecutionStatement(const std::string& statement);
	static bool isInlineReadWithDataType(const std::string& statement);
	static bool isInlineReadWithNoDataType(const std::string& statement);
	static bool isInlineFileReadType(const std::string& statement);
	static bool isFileReadWithDataType(const std::string& statement);
	static bool isFileReadWithNoDataType(const std::string& statement);
	static std::string toLowerCase(const std::string& token);
	static NumberType getNumberType(const std::string& token);
	static bool isComplexType(const NumberType& nt);
	static std::vector<std::string> tokenizeStatement(const std::string& strInput, const std::string& strDelims);
	static std::vector<std::string> getExecutionStatementTokens(const std::string& statement);
	static std::vector<std::string> getInputStatementTokens(const std::string& statement);
	static std::vector<std::string> getInputStatementTokensWithTransposeDelimeters(const std::string& statement);
	static bool isMultiplyStatement(const std::string& statement);
	static bool isTransposeStatement(const std::string& statement);
	static bool isAssignmentStatement(const std::string& statement);
	static bool isNoSymbolStatement(const std::string& statement);
	static std::vector<std::string> getExclusiveInputStatementTokens(const std::string& statement);
};