#include "Interpreter.h"

Interpreter::Interpreter() 
{
	m_MatrixWrapperPtr = std::make_unique<MatrixWrapper>();
}

void Interpreter::parse(const std::string& statement) //throws
{
	m_Statement = statement;
	syntaxAnalyzer();

	if (m_StatementType == StatementType::None)
		throw std::invalid_argument("Wrong syntax! Exiting reading..\n");

	semanticAnalyzer();
}


void Interpreter::syntaxAnalyzer()
{
	m_StatementType = StatementType::None;
	if (Utils::isClearStatement(m_Statement)) {

		m_StatementType = StatementType::Execution;
		return;
	}

	if (Utils::isExecutionStatement(m_Statement)) {
		m_StatementType = StatementType::Execution;
		return;
	}

	auto validType = Utils::isInlineFileReadType(m_Statement);
	if (validType) {
		m_StatementType = StatementType::InlineFileReadType;

		readMatrixDataFromFile(m_Statement);
		return;
	}

	validType = Utils::isInlineReadWithDataType(m_Statement);
	if (validType) {
		m_StatementType = StatementType::InlineReadWithDataType;
		return;
	}

	validType = Utils::isInlineReadWithNoDataType(m_Statement);
	if (validType) {
		m_StatementType = StatementType::InlineReadWithNoDataType;
		return;
	}
}

void  Interpreter::readMatrixDataFromFile(const std::string& statement)
{

	bool sofarsogood = true;
	bool secondArgumentAFile = false;
	//auto tokens = splitString(statement, " \t,()[]{}=");
	auto tokens = Utils::getInputStatementTokens(statement);
	if (tokens.size() != 3)
	{
		m_StatementType = StatementType::None;
	}

	std::string fileName = tokens[2];

	fs::path fsPath(fileName);
	std::string line;
	std::vector<std::string> allLinesVec;
	std::string combinedFileReadString;
	std::fstream inFile;
	try {
		inFile.open(fsPath.string(), std::fstream::in);

		while (std::getline(inFile, line))
		{
			allLinesVec.push_back(line);
		}
	}
	catch (std::exception ex)
	{
		inFile.close();
		std::stringstream ss;
		ss << "Unable to open the file " << m_Filename << " Exiting.\n";
		throw std::runtime_error(ss.str());
	}
	if (!inFile.is_open())
	{
		std::stringstream ss;
		ss << "Unable to open the file " << m_Filename << " Exiting.\n";
		throw std::runtime_error(ss.str());
	}

	for (auto& thisLine : allLinesVec)
	{
		std::size_t found = thisLine.find(";");
		if (found != std::string::npos)
		{
			thisLine.erase(thisLine.begin() + found);
		}
		combinedFileReadString += " ";
		combinedFileReadString += thisLine;
	}
	combinedFileReadString = tokens[0] + " = " + combinedFileReadString;
	m_Statement = combinedFileReadString;
	syntaxAnalyzer();
	
}


void Interpreter::handleExecutionStatement()
{
	if (Utils::isMultiplyStatement(m_Statement))
		m_ExecutionSubType = ExecutionSubType::Multiply;
	else if (Utils::isAssignmentStatement(m_Statement) && Utils::isTransposeStatement(m_Statement))
	{
		m_ExecutionSubType = ExecutionSubType::TransposeAndAssignment;
	}
	else if (!Utils::isAssignmentStatement(m_Statement) && Utils::isTransposeStatement(m_Statement))
	{
		m_ExecutionSubType = ExecutionSubType::DisplayTranspose;
	}
	else if (Utils::isAssignmentStatement(m_Statement))
	{
		m_ExecutionSubType = ExecutionSubType::SimpleAssignment;
	}
	else if (Utils::isNoSymbolStatement(m_Statement))
	{
		m_ExecutionSubType = ExecutionSubType::Display;
	}

	std::vector<std::string> statementTokens;
	size_t startIndex = 0;
	std::string lhs = "res";
	std::vector<std::string> args;

	std::vector<std::string> assignmentTokens;
	std::vector<std::string> transposeTokens;
	std::string rhs;
	switch (m_ExecutionSubType)
	{
	case ExecutionSubType::Multiply:
		statementTokens = Utils::getExecutionStatementTokens(m_Statement);

		if (statementTokens[statementTokens.size() - 1] == "*")
		{
			throw std::invalid_argument("Invalid statement. Exiting\n");
		}
		startIndex = 0;
		if (statementTokens[1] == "=")
		{
			lhs = statementTokens[0];
			startIndex = 2;
		}
		for (size_t ii = startIndex; ii < statementTokens.size(); ii++)
		{
			if (statementTokens[ii] != "*")
				args.push_back(statementTokens[ii]);
		}

		// Multiply matrices
		m_MatrixWrapperPtr->multiply(args);


		// display
		m_MatrixWrapperPtr->displayResult();

		break;
	case ExecutionSubType::Display:
		m_MatrixWrapperPtr->displayMatrix(m_Statement);
		break;
	case ExecutionSubType::DisplayTranspose:
		m_MatrixWrapperPtr->displayMatrix(m_Statement, true);
		break;
	case ExecutionSubType::TransposeAndAssignment:
		assignmentTokens = Utils::getExclusiveInputStatementTokens(m_Statement);
		lhs = assignmentTokens[0];
		transposeTokens = Utils::getInputStatementTokensWithTransposeDelimeters(assignmentTokens[2]);
		rhs = transposeTokens[0];
		m_MatrixWrapperPtr->assign2New(lhs, rhs, true);
		break;
	case ExecutionSubType::SimpleAssignment:
		assignmentTokens = Utils::getExclusiveInputStatementTokens(m_Statement);
		lhs = assignmentTokens[0];
		transposeTokens = Utils::getInputStatementTokensWithTransposeDelimeters(assignmentTokens[2]);
		rhs = transposeTokens[0];
		m_MatrixWrapperPtr->assign2New(lhs, rhs, false);
		break;
	default:
		throw std::runtime_error("Unidentified execution statement. Exiting\n");
		break;
	}
}


void Interpreter::handleInlineDataWithType()
{
	std::vector<std::string> statementTokens;
	size_t startIndex = 0;
	std::string lhs = "res";
	std::vector<std::string> args;

	statementTokens = Utils::getInputStatementTokens(m_Statement);
	if (statementTokens.empty())
		throw std::invalid_argument("Statement invalid. Exiting\n");

	std::string matrixTag = statementTokens[0];
	NumberType nType = NumberType::double_type;
	size_t rows = 0;
	size_t columns = 0;

	nType = Utils::getNumberType(statementTokens[2]);
	rows = Utils::stou(statementTokens[3]);
	columns = Utils::stou(statementTokens[4]);

	m_MatrixWrapperPtr->setMatrixWrapper(nType, matrixTag, rows, columns);

	size_t nElements = rows * columns;
	if (Utils::isComplexType(nType))
	{
		nElements = rows * columns * 2;
	}
	std::vector<int> v_int;
	std::vector<float> v_float;
	std::vector<double> v_double;
	std::vector<long int> v_long_int;
	std::vector<long double> v_long_double;
	std::vector<std::complex<int>> v_complex_int;
	std::vector<std::complex<float>> v_complex_float;
	std::vector<std::complex<double>> v_complex_double;
	std::vector<std::complex<long int>> v_complex_long_int;
	std::vector<std::complex<long double>> v_complex_long_double;
	try {
		for (size_t ii = 5; ii < statementTokens.size(); (Utils::isComplexType(nType) ? (ii += 2) : (ii++)))
		{
			switch (nType)
			{
			case NumberType::int_type:
				v_int.push_back(std::stoi(statementTokens[ii]));
				break;
			case NumberType::float_type:
				v_float.push_back(std::stof(statementTokens[ii]));
				break;
			case NumberType::double_type:
				v_double.push_back(std::stod(statementTokens[ii]));
				break;
			case NumberType::long_int_type:
				v_long_int.push_back(std::stol(statementTokens[ii]));
				break;
			case NumberType::long_double_type:
				v_long_double.push_back(std::stold(statementTokens[ii]));
				break;
			case NumberType::complex_int_type:
				v_complex_int.push_back({ std::stoi(statementTokens[ii]), std::stoi(statementTokens[ii + 1]) });
				break;
			case NumberType::complex_float_type:
				v_complex_float.push_back({ std::stof(statementTokens[ii]), std::stof(statementTokens[ii + 1]) });
				break;
			case NumberType::complex_double_type:
				v_complex_double.push_back({ std::stod(statementTokens[ii]), std::stod(statementTokens[ii + 1]) });
				break;
			case NumberType::complex_long_int_type:
				v_complex_long_int.push_back({ std::stol(statementTokens[ii]), std::stol(statementTokens[ii + 1]) });
				break;
			case NumberType::complex_long_double_type:
				v_complex_long_double.push_back({ std::stold(statementTokens[ii]), std::stold(statementTokens[ii + 1]) });
				break;
			default:
				throw std::invalid_argument("Data for the matrix type invalid. Exiting\n");
			}
		}
	}
	catch (...)
	{
		//m_MatrixWrapperPtr.reset();
		throw std::invalid_argument("Data for the matrix type invalid. Exiting\n");
	}

	try {
		switch (nType)
		{
		case NumberType::int_type:
			if (nElements != v_int.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_int);
			break;
		case NumberType::float_type:
			if (nElements != v_float.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_float);
			break;
		case NumberType::double_type:
			if (nElements != v_double.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_double);
			break;
		case NumberType::long_int_type:
			if (nElements != v_long_int.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_long_int);
			break;
		case NumberType::long_double_type:
			if (nElements != v_long_double.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_long_double);
			break;
		case NumberType::complex_int_type:
			if (nElements != 2u * v_complex_int.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_complex_int);
			break;
		case NumberType::complex_float_type:
			if (nElements != 2u * v_complex_float.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_complex_float);
			break;
		case NumberType::complex_double_type:
			if (nElements != 2u * v_complex_double.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_complex_double);
			break;
		case NumberType::complex_long_int_type:
			if (nElements != 2u * v_complex_long_int.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_complex_long_int);
			break;
		case NumberType::complex_long_double_type:
			if (nElements != 2u * v_complex_long_double.size())
				throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
			m_MatrixWrapperPtr->addData(matrixTag, v_complex_long_double);
			break;
		}
	}
	catch (std::exception ex)
	{
		std::string error = ex.what() + std::string(" Error while setting value to the matrix. Exiting\n");
		throw std::runtime_error(error);
	}
}

void Interpreter::handleInlineDataWithoutType()
{
	std::vector<std::string> statementTokens;
	size_t startIndex = 0;
	std::string lhs = "res";
	std::vector<std::string> args;

	statementTokens = Utils::getInputStatementTokens(m_Statement);
	if (statementTokens.empty())
		throw std::invalid_argument("Statement invalid. Exiting\n");

	std::string matrixTag = statementTokens[0];
	NumberType nType = NumberType::double_type;
	size_t rows = 0;
	size_t columns = 0;

	rows = Utils::stou(statementTokens[2]);
	columns = Utils::stou(statementTokens[3]);

	m_MatrixWrapperPtr->setMatrixWrapper(nType, matrixTag, rows, columns);
	size_t nElements = rows * columns;
	if (Utils::isComplexType(nType))
	{
		nElements = rows * columns * 2;
	}
	std::vector<double> v_double;
	std::vector<std::complex<double>> v_complex_double;
	try {
		for (size_t ii = 4; ii < statementTokens.size(); (Utils::isComplexType(nType) ? (ii += 2) : (ii++)))
		{
			switch (nType)
			{

			case NumberType::double_type:
				v_double.push_back(std::stod(statementTokens[ii]));
				break;
			default:
				throw std::invalid_argument("Data for the matrix type invalid. Exiting\n");
			}
		}
	}
	catch (...)
	{	
		throw std::invalid_argument("Data for the matrix type invalid. Exiting\n");
	}

	switch (nType)
	{
	case NumberType::double_type:
		if (nElements != v_double.size())
			throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
		break;
	case NumberType::complex_double_type:
		if (nElements != v_complex_double.size())
			throw std::runtime_error("matrix elements size not equal to prescribed. Exiting\n");
		break;
	}
}

void Interpreter::semanticAnalyzer() // throws
{
	switch (m_StatementType)
	{
	case StatementType::InlineReadWithDataType:
		handleInlineDataWithType();
		break;
	case StatementType::InlineReadWithNoDataType:
		handleInlineDataWithoutType();
		break;
	case StatementType::Execution:
		handleExecutionStatement();
	default:
		break;
	}
}

