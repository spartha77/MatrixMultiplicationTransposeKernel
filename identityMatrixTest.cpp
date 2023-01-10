
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
    std::cout << "identity matrix text\n";
    
    Matrix<long double> m1_10_9{ "m1_10_10", 10,9 };
    m1_10_9.identityfy();
    std::cout << m1_10_9;

    Matrix<int> m2_nn;
    m2_nn.identityfy(4, 4);
    std::cout << m2_nn;

    Matrix<std::complex<double>> m2complx_nn;
    m2complx_nn.identityfy(8, 9);
    std::cout << m2complx_nn;

    m2_nn.identityfy(6, 6);
    std::cout << m2_nn;

    std::vector<std::complex<int>> vv{ (0,0),(1,1) };
    vv[1] = std::complex<int>(2, 2);


    return 0;
}