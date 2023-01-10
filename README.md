# The repository contains the basic implementation of math kernel that multiplies two matrices and transposes a matrix

Salient features of the Matrix Kernel

- The matrix is implemented for INT, FLOAT, DOUBLE, LONG INT, LONG DOUBLE, and for COMPLEX numbers of the previously mentioned types.
- The operations throws exceptions with appropriate error messages. This is to keep up with modern designs when the returned error message/code alone is not sufficient.
- A lazy matrix multiplication schema is implemented, for rectangular (non-square) multiplications for matrices that are called using operator*() on more than 2 matrices
- The internal data that the matrix uses is a 1-d vector and not 2d.
- The transpose of the matrix is implemented by setting a transpose flag true. The getMatrixTransposeIndex() is called when the flag is unset. The function getMatrix2RowVectorIndex() is called when the transpose is requested. In any case, the data is not shifted or rotated or transposed in real sense. This flag m_Transposed is part of the state of the matrix object
