# The repository contains the basic implementation of math kernel that multiplies two matrices and transposes a matrix

Salient features of the Matrix Kernel


- The math kernel is a header only implementation
- The matrix is implemented for INT, FLOAT, DOUBLE, LONG INT, LONG DOUBLE, and for COMPLEX numbers of the previously mentioned types.
- The operations throws exceptions with appropriate error messages. This is to keep up with modern designs when the returned error message/code alone is not sufficient.
- A **lazy matrix multiplication schema is implemented**, for rectangular (non-square) multiplications for matrices that are called using operator*() on more than 2 matrices
- The internal data that the matrix uses is a 1-d vector and not 2d.
- The transpose of the matrix is implemented by setting a transpose flag true. **No data is moved or copied when transpose is requested. Just that the way the vector<numbers> is read is changed.** The getMatrixTransposeIndex() is called when the flag is unset. The function getMatrix2RowVectorIndex() is called when the transpose is requested. In any case, the data is not shifted or rotated or transposed in real sense. This flag m_Transposed is part of the state of the matrix object

**Tests are provided to test the following
**
- Chain Rectangular Long Double Matrix ( to test the multiplication while preserving the type of the arithmetic ) ChainRectLongDoubleMatrixTest.cpp, The executable is **ChainRectLongDoubleMatrixTest**
- Complex Non-Square ChainMatrixTest ( to test the cascaded multiplication ) ComplexNonSquareChainMatrixTest.cpp. The executable is **ComplexNonSquareChainMatrixTest**
- Complex Square Matrix Test ( The square matrix multiplication is not optimizable in the current implementation. An honest try was made using threadpool, to implement cannon's algorithm. It is work under progress. However **ComplexSquareMatrixTest** can be tested
- 1X1 matrix, 1XN and N X 1 matrices (boundary cases are tested using **UnitaryMatrixTest**
- Identityfy() and identityfy(rows, columns) are two methods that sets the identity matrix to the existing matrix , AND re-sizes the current matrices respectively.. **IdentityMatrixTest** shall be used to test this.

An honest try is made to run the matrix multiplication and transpose in command line, like MATLAB or SCILAB. The executable for that is **CommandLineMatrixTest**

Note: Please feel free to modify the source files

**How to build the source**

- Download the source files
- Ensure that gnu++20 standards are available.

**- Run the following commands sequentially
**
   
   g++ -std=gnu++20 -c Utils.cpp
   
   g++ -std=gnu++20 -c Interpreter.cpp
   
   ar rvs Utils.a Utils.o
   
   ar rvs Interpreter.a Interpreter.o
   
   g++ -std=gnu++20 CommandLineMatrixTest.cpp  Utils.a Interpreter.a -o **CommandLineMatrixTest**
   
   g++ -std=gnu++20 ChainRectLongDoubleMatrixTest.cpp Utils.a Interpreter.a -o **ChainRectLongDoubleMatrixTest**
   
   g++ -std=gnu++20 ComplexNonSquareChainMatrixTest.cpp  Utils.a Interpreter.a -o **ComplexNonSquareChainMatrixTest**
   
   g++ -std=gnu++20 ComplexSquareMatrixTest.cpp  Utils.a Interpreter.a -o **ComplexSquareMatrixTest**
   
   g++ -std=gnu++20 UnitaryMatrixTest.cpp  Utils.a Interpreter.a -o **UnitaryMatrixTest**
   
   g++ -std=gnu++20 IdentityMatrixTest.cpp  Utils.a Interpreter.a -o **IdentityMatrixTest**
   
   
   
**Directions to run the command line tool**

The operation this supports are 
- **Matrix read from the standard input** (console)
- Statement > **a= int 2,3,0,1,2,3,4,5;** // reads and assigns matrix of size 2X3 and elements 0 1 2 3 4 5 in row read fashion like 
  0 1 2
  3 4 5
  int can be float double long_int or long_double
  In the case of complex numbers the input can be assigned as 
  **a= complex_int 2,3, (0,1), (2,3), (4,5), (6,7), (8,9), (10,10)**
  **a** is the name of the matrix ( treated as variable)
  
  
- **Matrix read from file**
Matrix can also be read from the file. The only difference is the file shall contain the data as above, but not the variable name
The file entry is as follows 
**complex_double 4,3,3,0,1,1,3,6,2
,3,0,9,7,8,3,1,7,
8,0,4,5,6,1,0,3,4;**
The above data in a file has complex_double

The matrix shall be read using the command **a=C:\Users\parth\source\repos\braincorp\LAKernel\LAKernel\int_file_10_10_file.txt
**

- **Multiplication**
For the matrices already read, say a and b,
the multiplication can be executed as **z=a*b**
or **z=a*b*c*d;** if all the four matrices are defined


**- Transform**
**a^** (enter) displays the transforms of **a**
**b=a^** computes the transform of a and stores in **b**


- **Assignment statements of the following types**
**a=b** is a simple assignment statement


**- Display**
**a** (enter)
**b** (enter)
displays the matrix at the console

