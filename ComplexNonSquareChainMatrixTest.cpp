
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
    std::cout << "Complex non-square chain matrices\n";
    std::vector<std::complex<int>> rowVectors5{ {1,2},{3,4},{5,6},{7,8},{9,0},{1,2} };
    Matrix<std::complex<int>> m5_23{ "m5_23", 2,3,rowVectors5 };
    
    std::vector<std::complex<int>> rowVectors6{ {1,2},{3,4},{5,6},{7,8},{9,0},{1,2},{0,2},{2,1},{1,0},{3,4},{4,0},{1,1} };
    const Matrix<std::complex<int>> m6_34{ "m6_34", 3,4,rowVectors6 };
    
    std::vector<std::complex<int>> rowVectors3{ {1,2},{3,4},{5,6},{7,8},{9,0},{1,2},{0,2},{2,1},{1,0},{3,4},{4,0},{1,1},
        {1,2},{2,1},{1,1},{2,2},{1,1},{1,1},{1,1},{1,1} };
    Matrix<std::complex<int>> m3_45{ "m3_45", 4,5,rowVectors3 };
    

    std::vector<std::complex<int>> rowVectors1{ {1,2},{3,4},{5,6},{7,8},{9,0},{1,2},{0,2},{2,1},{1,0},{3,4},{4,0},{1,1},
        {1,2},{2,1},{1,1},{2,2},{1,1},{1,1},{1,1},{1,1},{1,1},{1,2},{1,1},{2,3},{1,1},{1,1},{2,1},{2,2},{1,1},{2,3} };
    Matrix<std::complex<int>> m1_56{ "m1_56", 5,6,rowVectors1 };
    

    std::vector<std::complex<int>> rowVectors2{ {1,2},{3,4},{5,6},{7,8},{9,0},{1,2},{0,2},{2,1},{1,0},{3,4},{4,0},{1,1},
        {1,2},{2,1},{1,1},{2,2},{1,1},{1,1},{1,1},{1,1},{1,1},{1,2},{1,1},{2,3} };
    Matrix<std::complex<int>> m2_64{ "m2_64", 6,4,rowVectors2 };

    std::vector<std::complex<int>> rowVectors4{ {1,2},{2,1},{1,1},{2,2},{1,1},{1,1},{1,1},{1,1},{1,1},{1,2} };
    Matrix<std::complex<int>> m4_52{ "m4_52", 5,2,rowVectors4 };

    
    auto m1m2 = m1_56 * m2_64 * m3_45;
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