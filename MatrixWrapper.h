#pragma once

#include <string>
#include <vector>
#include <complex>
#include <algorithm>
#include <optional>
#include <type_traits>
#include <sstream>
#include <filesystem>
#include <map>
#include <variant>

namespace fs = std::filesystem;

#include "Utils.h"



template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>; // line not needed in



struct MatrixWrapper
{
	using VariantMatrix = std::variant< Matrix<int>,
		Matrix<float>,
		Matrix<double>,
		Matrix<long int>,
		Matrix<long double>,
		Matrix<std::complex<int>>,
		Matrix<std::complex<float>>,
		Matrix<std::complex<double>>,
		Matrix<std::complex<long int>>,
		Matrix<std::complex<long double>> >;
	
	VariantMatrix m_ResultMatrix;

	std::map<std::string, VariantMatrix> m_VariantMatricesMap;
	std::vector< VariantMatrix> m_Matrices2Multiply;
	
	MatrixWrapper()
	{	
	}

	MatrixWrapper(const NumberType& numberType, const std::string& tag, const size_t& rows, const size_t& columns)
	{
		setMatrixWrapper(numberType, tag, rows, columns);
	}

	void setMatrixWrapper(const NumberType& numberType, const std::string& tag, const size_t& rows, const size_t& columns)
	{
		VariantMatrix varMatrix;
		switch (numberType)
		{
		case NumberType::int_type:
			varMatrix = Matrix<int>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::float_type:
			varMatrix = Matrix<float>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::double_type:
			varMatrix = Matrix<double>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::long_double_type:
			varMatrix = Matrix<long double>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::long_int_type:
			varMatrix = Matrix<long int>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::complex_int_type:
			varMatrix = Matrix<std::complex<int>>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::complex_float_type:
			varMatrix = Matrix<std::complex<float>>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::complex_double_type:
			varMatrix = Matrix<std::complex<double>>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::complex_long_double_type:
			varMatrix = Matrix<std::complex<long double>>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		case NumberType::complex_long_int_type:
			varMatrix = Matrix<std::complex<long int>>(tag, rows, columns);
			m_VariantMatricesMap[tag] = varMatrix;
			break;
		default:
			throw std::invalid_argument("Unsupported type in matrix creation. Exiting\n");
			break;
		}
	}

	template<typename U>
	void addData( const std::string& tag, const U& val)
	{
		std::visit(overloaded{
			[&val](Matrix<int > &mat)
			{
				if constexpr (std::is_same_v<const int&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<int>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<float>& mat)
			{
				if constexpr (std::is_same_v<const float&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<float>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<double>& mat)
			{
				if constexpr (std::is_same_v<const double&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<double>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<long int>& mat)
			{
				if constexpr (std::is_same_v<const long int&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<long int>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<long double>& mat)
			{
				if constexpr (std::is_same_v<const long double&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<long double>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<std::complex<int>>& mat)
			{
				if constexpr (std::is_same_v<const std::complex<int>&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<std::complex<int>>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<std::complex<float>>& mat)
			{
				if constexpr (std::is_same_v<const std::complex<float>&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<std::complex<float>>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<std::complex<double>>& mat)
			{
				if constexpr (std::is_same_v<const std::complex<double>&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<std::complex<double>>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<std::complex<long int>>& mat)
			{
				if constexpr (std::is_same_v<const std::complex<long int>&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<std::complex<long int>>&, decltype(val)>)
					mat.data() = val;
			},
			[&val](Matrix<std::complex<long double>>& mat)
			{
				if constexpr (std::is_same_v<const std::complex<long double>&, decltype(val)>)
					mat.data().push_back(val);
				else if constexpr (std::is_same_v<const std::vector<std::complex<long double>>&, decltype(val)>)
					mat.data() = val;
			}
		},m_VariantMatricesMap[tag]);
	}


	template<typename T>
	auto getData(const std::string& tag)
	{
		if (auto* ptr = std::get_if<Matrix<int>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<float>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<double>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<long int>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<long double>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<int>>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<float>>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<double>>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<long int>>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<long double>>>(&m_VariantMatricesMap[tag]))
		{
			return ptr->data();
		}
		else
			throw std::invalid_argument("Matrix of specified name/type does not exist. Exiting\n");
	}
	

	template<typename T>
	void transpose(const std::string& tag)
	{
		if (auto* ptr = std::get_if<Matrix<int>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<float>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<double>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<long int>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<long double>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<int>>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<float>>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<double>>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<long int>>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else if (auto* ptr = std::get_if<Matrix<std::complex<long double>>>(&m_VariantMatricesMap[tag]))
		{
			ptr->transpose();
		}
		else
			throw std::invalid_argument("Matrix of specified name/type does not exist. Exiting\n");
	}

	void assign2New(const std::string& lhs, const std::string& rhs, bool need2Transpose)
	{
		if (m_VariantMatricesMap.find(rhs) == m_VariantMatricesMap.end())
		{
			std::stringstream ss;
			ss << "No matrix with name " << rhs << " exists. Exiting" << std::endl;
			throw std::runtime_error(ss.str());
		}
			
		std::visit(overloaded{
			[this , &need2Transpose, lhs, rhs](Matrix<int >& mat)
			{
				if ( need2Transpose )
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<float >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<double >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<long int >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<long double >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<std::complex<int> >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<std::complex<float> >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<std::complex<double> >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<std::complex<long int> >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			},
			[this , &need2Transpose, lhs, rhs](Matrix<std::complex<long double> >& mat)
			{
				if (need2Transpose)
					this->m_VariantMatricesMap[lhs] = mat.getTranspose();
				else
					this->m_VariantMatricesMap[lhs] = mat;
			}
			}, m_VariantMatricesMap[rhs]
		);
	}



	void displayResult()
	{
		std::visit(overloaded{
			[](Matrix<int >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<float >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<double >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<long int >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<long double >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<std::complex<int> >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<std::complex<float> >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<std::complex<double> >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<std::complex<long int> >& mat)
			{
				std::cout << mat;
			},
			[](Matrix<std::complex<long double> >& mat)
			{
				std::cout << mat;
			}
			}, m_ResultMatrix
		);
	}

	void displayMatrix(std::string &matName, bool need2Transpose = false)
	{
		std::string matrixName;
		auto tokens = Utils::getInputStatementTokensWithTransposeDelimeters(matName);
		if (tokens.empty())
			matrixName = matName;
		else
			matrixName = tokens[0];

		std::visit(overloaded{
			[need2Transpose](Matrix<int >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<float >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<double >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<long int >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<long double >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<std::complex<int> >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<std::complex<float> >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<std::complex<double> >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<std::complex<long int> >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			},
			[need2Transpose](Matrix<std::complex<long double> >& mat)
			{
				if (need2Transpose)
					std::cout << mat.getTranspose();
				else
					std::cout << mat;
			}
			}, m_VariantMatricesMap[matrixName]
		);
	}

	void transpose(VariantMatrix& varMatrix)
	{
		std::visit(overloaded{
			[](Matrix<int >& mat)
			{
				mat.transpose();
			},
			[](Matrix<float >& mat)
			{
				mat.transpose();
			},
			[](Matrix<double >& mat)
			{
				mat.transpose();
			},
			[](Matrix<long int >& mat)
			{
				mat.transpose();
			},
			[](Matrix<long double >& mat)
			{
				mat.transpose();
			},
			[](Matrix<std::complex<int> >& mat)
			{
				mat.transpose();
			},
			[](Matrix<std::complex<float> >& mat)
			{
				mat.transpose();
			},
			[](Matrix<std::complex<double> >& mat)
			{
				mat.transpose();
			},
			[](Matrix<std::complex<long int> >& mat)
			{
				mat.transpose();
			},
			[](Matrix<std::complex<long double> >& mat)
			{
				mat.transpose();
			}
			}, m_ResultMatrix
		);
	}

	void multiply(const std::vector<std::string>& tags)
	{
		std::vector < std::string> matTokensWithTransposes;
		bool need2Transpose = false;

		auto need2TRansposeLmbda = [](const std::string& tag, bool& need2Transpose, 
			std::vector<std::string>& matTokensWithTransposes, std::string& matName) {
			matTokensWithTransposes = Utils::getInputStatementTokensWithTransposeDelimeters(tag);
			need2Transpose = false;
			int nTransposes = 0;
			for (const auto& matToken : matTokensWithTransposes)
			{
				if (matToken == "^")
					nTransposes++;
			}
			if (nTransposes % 2 == 1)
				need2Transpose = true;

			matName = matTokensWithTransposes[0];
		};

		try {

			std::string matrixName;
			need2TRansposeLmbda(tags[0], need2Transpose, matTokensWithTransposes, matrixName);

			m_ResultMatrix = m_VariantMatricesMap[matrixName];
			if (need2Transpose)
				this->transpose(m_ResultMatrix);

			for (auto ii = 1u; ii < tags.size(); ii++)
			{
				
				need2TRansposeLmbda(tags[ii], need2Transpose, matTokensWithTransposes, matrixName);
				std::visit(overloaded{
					[this, need2Transpose](Matrix<int >& mat)
					{
						//Matrix<int> tr = mat;
						if (need2Transpose)
							this->m_ResultMatrix = std::get<Matrix<int>>(m_ResultMatrix) * mat.getTranspose();
						else
							this->m_ResultMatrix = std::get<Matrix<int>>(m_ResultMatrix) * mat;

					},
					[this, need2Transpose](Matrix<float >& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<float>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<double>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<double>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<long int>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<long int>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<long double>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<long double>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<std::complex<int>>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<std::complex<int>>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<std::complex<float>>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<std::complex<float>>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<std::complex<double>>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<std::complex<double>>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<std::complex<long int>>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<std::complex<long int>>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					},
					[this, need2Transpose](Matrix<std::complex<long double>>& mat)
					{
						this->m_ResultMatrix = std::get<Matrix<std::complex<long double>>>(m_ResultMatrix) * (need2Transpose ? (mat.getTranspose()) : (mat));
					}
					}, m_VariantMatricesMap[matrixName]);
			}
		}
		catch (...)
		{
			throw std::runtime_error("Invalid or non-existent or incomtabile matrices in multiplying. Exiting\n");
		}

		try {
			std::visit(overloaded{
					[this](Matrix<int >& mat)
					{
						mat.evaluate();

					},
					[this](Matrix<float >& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<double>& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<long int>& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<long double>& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<std::complex<int>>& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<std::complex<float>>& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<std::complex<double>>& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<std::complex<long int>>& mat)
					{
						mat.evaluate();
					},
					[this](Matrix<std::complex<long double>>& mat)
					{
						mat.evaluate();
					}
				}, m_ResultMatrix);
		}
		catch (std::exception ex)
		{
			throw std::runtime_error("Error while evaluating the result of multiplication. Exiting\n");
		}
		
	}

};