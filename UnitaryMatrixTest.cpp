
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

    
    std::cout << "Unitary matrices\n";
    std::vector<long double> rowVectors1{ 2.5 };
    Matrix<long double> m1_11{ "m1_11", 1,1,rowVectors1 };

    
    std::vector<long double> rowVectors2{ 3.5 };
    Matrix<long double> m2_11{ "m2_11", 1,1,rowVectors2 };

    std::vector<long double> rowVectors3{ 4.5 };
    Matrix<long double> m3_11{ "m3_11", 1,1,rowVectors3 };

    std::vector<long double> rowVectors4{ 0.5,1.5,2.5,3.5,4.5 };
    Matrix<long double> m4_15{ "m4_15", 1,5,rowVectors4 };

    std::vector<long double> rowVectors5{ 0.5,1.5,2.5,3.5,4.5 };
    Matrix<long double> m5_51{ "m5_51", 5,1,rowVectors5 };

    std::vector<long double> rowVectors6{ 0.5,1.5,2.5,3.5,4.5 };
    Matrix<long double> m5_15{ "m5_15", 1,5,rowVectors6 };
    


    //auto m1m2 = m1 * m2 * m3 * m4 * m5 * m6;
    auto m1m2m3 = m1_11 * m2_11 * m3_11;
    std::cout << m1m2m3;
    auto m1m2 = m1_11 * m2_11;
    std::cout << m1m2;
    auto m5_51_m5_15 = m5_51 * m5_15;
    m5_51_m5_15.evaluate();
    std::cout << m5_51_m5_15;

    /*std::cout << "Before Transposing" << std::endl;
    std::cout << m7;*/

    /*Matrix<long double> m1m2T = m1m2.getTranspose();
    m1m2T.m_Tag = "Transposed";
    std::cout << m1m2T;

    std::cout << "Transpose of transpose" << std::endl;
    m1m2.transpose().transpose();
    std::cout << m1m2;
    */
    int aa = 0;
        std::cin >> aa;

    return 0;
}