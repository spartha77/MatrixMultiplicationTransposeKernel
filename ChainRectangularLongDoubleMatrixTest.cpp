
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
    std::cout << "Chain rectangular long double matrices\n";
    std::vector<long double> rowVectors5{ 2.5,3.5,4.5,5.5,6.5,0.5 };
    Matrix<long double> m5_23{ "m5_23", 2,3,rowVectors5 };

    std::vector<long double> rowVectors6{ 4.2,5.2,0.2,6.2,1.2,1.2,1.2,1.2,1.2,0.2,6.2,3.1 };
    const Matrix<long double> m6_34{ "m6_34", 3,4,rowVectors6 };
    


    std::vector<long double> rowVectors3{ 0.2,1.2,2.2,3.2,4.2,5.2,6.2,7.2,8.2,9.2,0.2,1.2,2.2,3.2,4.2,5.2,6.2,7.2,0.2,1.2 };
    Matrix<long double> m3_45{ "m3_45", 4,5,rowVectors3 };
    

    std::vector<long double> rowVectors1{ 0.2,1.2,2.2,3.2,4.2,5.2,6.2,7.2,8.2,9.2,0.2,1.2,2.2,3.2,4.2,5.2,6.2,7.2,0.2,1.2,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
    Matrix<long double> m1_56{ "m1_56", 5,6,rowVectors1 };
    

    std::vector<long double> rowVectors2{ 0.2,1.2,2.2,3.2,4.2,5.2,6.2,7.2,8.2,9.2,0.2,1.2,2.2,3.2,4.2,5.2,6.2,7.2,0.2,1.2,0.1,0.2,0.3,0.4};
    Matrix<long double> m2_64{ "m2_64", 6,4,rowVectors2 };

    std::vector<long double> rowVectors4{ 0.2,1.2,2.2,3.2,4.2,5.2,6.2,7.2,8.2,9.2 };
    Matrix<long double> m4_52{ "m4_52", 5,2,rowVectors4 };

    auto m1m2 = m1_56 * m2_64 * m3_45;
    m1m2.evaluate();
    Matrix<long double> m7(m1m2);

    std::cout << "Before Transposing" << std::endl;
    std::cout << m7;

    Matrix<long double> m1m2T = m1m2.getTranspose();
    m1m2T.m_Tag = "Transposed";
    std::cout << m1m2T;

    std::cout << "Transpose of transpose" << std::endl;
    m1m2.transpose().transpose();
    std::cout << m1m2;
    
    int aa = 0;
    ++aa;
    std::cin >> aa;

    return 0;
}