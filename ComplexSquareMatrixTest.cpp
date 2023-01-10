
#include <iostream>
#include <complex>
#include <algorithm>
#include <regex>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "matrix.h"

int main()
{
    std::cout << "Complex square matrix\n";
	std::vector<std::complex<int>> rowVectors5{ {1,2},{3,4},{5,6}, { 1,1 }, { 2,3 }, { 1,1 }, { 1,1 }, { 2,1 }, { 0,1 } };
    Matrix<std::complex<int>> m5_33{ "m5_33", 3,3,rowVectors5 };

    std::vector<std::complex<int>> rowVectors6{ {1,2},{3,4},{5,6},{1,1},{1,1},{1,1},{1,1},{1,1},{2,3} };
    const Matrix<std::complex<int>> m6_33{ "m6_33", 3,3,rowVectors6 };
    
    std::vector<std::complex<int>> rowVectors3{ {3,4},{4,0},{1,1}, {3,4},{2,0},{1,0}, {0,4},{0,0},{0,1} };
    Matrix<std::complex<int>> m3_33{ "m3_33", 3,3,rowVectors3 };
    
    std::vector<std::complex<int>> rowVectors1{ {1,1},{1,1},{1,1},{1,1},{1,1},{1,2},{1,1},{2,3},{1,1} };
    Matrix<std::complex<int>> m1_33{ "m1_33", 3,3,rowVectors1 };
    
    std::vector<std::complex<int>> rowVectors2{ {1,2},{2,1},{1,1},{2,2},{1,1},{1,1},{1,1},{1,1},{1,1} };
    Matrix<std::complex<int>> m2_33{ "m2_33", 3,3,rowVectors2 };

    std::vector<std::complex<int>> rowVectors4{ {2,1},{1,1},{2,2},{1,1},{1,1},{1,1},{1,1},{1,1},{1,2} };
    Matrix<std::complex<int>> m4_33{ "m4_33", 3,3,rowVectors4 };

    auto m1m2 = m1_33 * m2_33 * m3_33;
    m1m2.evaluate();
    Matrix<std::complex<int>> m7(m1m2);

    std::cout << m1m2;

    Matrix<std::complex<int>> m1m2T = m1m2.getTranspose();
    m1m2T.m_Tag = "Transposed";
    std::cout << m1m2T;

    m1m2.transpose().transpose();
    std::cout << m1m2;

    return 0;
}