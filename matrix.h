#pragma once


#include <cstddef> // added missing header for std::size_t
#include <vector>
#include <type_traits>
#include <variant>
#include <iostream>
#include <stack>
#include <string>
#include <complex>

#include "tree.h"
//#include "ThreadPool.h"


template<typename T>
struct is_complex_t : public std::false_type {};

template<typename T>
struct is_complex_t<std::complex<T>> : public std::true_type {};

template<typename T>
constexpr bool is_complex_v()
{ 
    if constexpr (std::is_same_v<std::complex<int>, T> || std::is_same_v<std::complex<long int>, T> ||
        std::is_same_v<std::complex<float>, T> || std::is_same_v<std::complex<double>, T> || std::is_same_v<std::complex<long double>, T>)
        return true;
    else
        return false;
}

template<typename T>
constexpr bool is_complex_integral_v()
{
    if constexpr (std::is_same_v<std::complex<int>, T> || std::is_same_v<std::complex<long int>, T>)
        return true;
    else
        return false;
    
}

template<typename T>
class Matrix
{
public:
    // use type aliases so that users can correctly refer to values
    using value_type = typename std::vector<T>::value_type;
    using reference = typename std::vector<T>::reference;
    using size_type = typename std::vector<T>::size_type;

    /// <summary>
    /// Creates matrix with a name, rows and columns
    /// </summary>
    /// <param name="tag">Name of the marix</param>
    /// <param name="r">rows</param>
    /// <param name="c">columns</param>
    Matrix(const std::string& tag, size_type const r, size_type const c)
        : m_MatrixInternal{ r,c },m_Tag(tag)
    {   
        m_ChainMultiplicationEvaluated = true;
        m_Transposed = false;
    }

    /// <summary>
    /// Creates matrix with a name, rows and columns and initializaing with a vector
    /// </summary>
    /// <param name="tag">name of the matrix</param>
    /// <param name="r">rows</param>
    /// <param name="c">columns</param>
    /// <param name="vector">initialization vector</param>
    Matrix(const std::string& tag, size_type const r, size_type const c, const std::vector<T>& vector):
        m_MatrixInternal{r,c,vector},m_Tag(tag)
    {
        if (vector.size() != r * c)
            throw std::logic_error("Input to matrix data does not conform to the dimensions of the matrix");
        m_Transposed = false;
    }

    Matrix() :m_MatrixInternal{}, m_Tag("empty_matrix"){m_Transposed = false; }
    
    /// <summary>
    /// Fetching the value at i,j th position in the matrix. Only RValue.
    /// </summary>
    /// <param name="i">rows index</param>
    /// <param name="j">column index</param>
    /// <returns>value at the index. Throws exception if in the case of error with appropriate message</returns>
    T operator()(size_type const i, size_type const j) const noexcept 
    {
        if (!isEvaluated())
        {
            this->evaluate();
            m_ChainMultiplicationEvaluated = true;
        }
        if (m_Transposed)
        {
            size_t index = getMatrixTransposeIndex(i, j);
            T val = m_MatrixInternal.data()[index];
            return val;
        }
        else
        {
            T val = m_MatrixInternal.data()[getMatrix2RowVectorIndex(i, j)];
            return val;
        }
    }

    /// <summary>
    /// Fetching the value at i,j th position in the matrix. Also used for LValue
    /// </summary>
    /// <param name="i">row index</param>
    /// <param name="j">column index</param>
    /// <returns>LValue reference to the (i,)th position</returns>
    T& operator()(size_type const i, size_type const j)
    {
        if (!isEvaluated())
        {
            this->evaluate();
            m_ChainMultiplicationEvaluated = true;
        }
        if (m_Transposed)
        {
            return m_MatrixInternal.data().at(getMatrixTransposeIndex(i, j));
        }
        else
        {
            return m_MatrixInternal.data().at(getMatrix2RowVectorIndex(i, j));
        }
    }

    /// <summary>
    /// Fetches the std::vector<T> data, the matrix internally stores the data in as 1-d vector
    /// </summary>
    /// <returns> Copy of the data vector</returns>
    std::vector<T> getData() const
    {
        if (!isEvaluated())
        {
            this->evaluate();
            m_ChainMultiplicationEvaluated = true;
        }
        return m_MatrixInternal.getData();
    }

    /// <summary>
    /// Fetches the std::vector<T> data, the matrix internally stores the data in as 1-d vector
    /// </summary>
    /// <returns>LValue reference to the data vector</returns>
    std::vector<T>& data()
    {
        if (!isEvaluated())
        {
            this->evaluate();
            m_ChainMultiplicationEvaluated = true;
        }
        return m_MatrixInternal.data();
    }
    
    /// <summary>
    /// Multiplies chain of matrices. Throws when if error is encountered.
    /// Internally stores the matrix chains in the same order. The evaluation
    /// is called when calling data() or getData(), operator(i,j). Explicit evaluate() 
    /// shall also be called after calling operator*()
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns>Multiplied matrix of this and rhs. </returns>
    Matrix operator*(const Matrix& rhs)
    {
        Matrix rhsMatrix(rhs);
        
        if (this->getColumns() != rhsMatrix.getRows())
            throw std::logic_error("Matrices for multiplication are not compatible");

        bool b1 = this->isEvaluated();
        bool s1 = this->m_MatrixInternal.getData().size() == 0;
        bool b2 = rhsMatrix.isEvaluated();
        bool s2 = rhsMatrix.m_MatrixInternal.getData().size() == 0;
        if ( (b1&&s1 ) || (b2&&s2) )
            throw std::logic_error("Null matrix(s) encountered in multiplication");

        Matrix matRes{ "mat_mult_chain", this->getRows(), rhsMatrix.getColumns()};
        
        if (!this->isEvaluated())
        {
            matRes.m_ChainMultiplicationVector.insert(matRes.m_ChainMultiplicationVector.end(),
                m_ChainMultiplicationVector.begin(), m_ChainMultiplicationVector.end());
        }
        else
        {
            matRes.m_ChainMultiplicationVector.push_back(*this);
        }

        if (!rhsMatrix.isEvaluated())
        {
            matRes.m_ChainMultiplicationVector.insert(matRes.m_ChainMultiplicationVector.end(),
                rhsMatrix.m_ChainMultiplicationVector.begin(), rhsMatrix.m_ChainMultiplicationVector.end());
        }
        else
        {
            matRes.m_ChainMultiplicationVector.push_back(rhsMatrix);
        }
        matRes.m_ChainMultiplicationEvaluated = false;
        return matRes;
    }


    /// <summary>
    /// Prints the contents of the matrix to the console
    /// </summary>
    /// <param name="os">input outstream</param>
    /// <param name="matrix">matrix to print</param>
    static void streamOut(std::ostream& os, const Matrix& matrix)
    {
        if (!matrix.isEvaluated())
        {
            matrix.evaluate();
            matrix.m_ChainMultiplicationEvaluated = true;
        }
        os << "\nTag = "<< matrix.m_Tag<<std::endl;
        auto rows = matrix.m_MatrixInternal.getRows();
        auto columns = matrix.m_MatrixInternal.getColumns();
        for (size_t ii = 0; ii < rows; ii++)
        {
            os << " |\t";
            for (size_t jj = 0; jj < columns; jj++)
                os << matrix(ii, jj) << "\t";
            os << "|" << std::endl;
        }
    }

    /// <summary>
    /// initiates streaming of matrix to std::cout
    /// </summary>
    /// <param name="os">output stream</param>
    /// <param name="matrices">matrix to be printed in console</param>
    /// <returns></returns>
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrices) 
    {
        if (!matrices.isEvaluated())
        {
            matrices.evaluate();
            matrices.m_ChainMultiplicationEvaluated = true;
        }

        if (matrices.isEvaluated())
        {
            Matrix::streamOut(os, matrices);
        }
        else
        {
            for ( const auto& mat : matrices.m_ChainMultiplicationVector )
            {
                Matrix::streamOut(os, mat);
            }
        }
        return os;
    }

    /// <summary>
    /// Returns number of rows
    /// </summary>
    /// <returns>Returns number of rows</returns>
    size_t getRows() const 
    { 
        return m_MatrixInternal.getRows();
    }

    /// <summary>
    /// Fetch number of columns
    /// </summary>
    /// <returns>Returns number of columns</returns>
    size_t getColumns() const
    {  
        return m_MatrixInternal.getColumns();
    }

    /// <summary>
    /// Copy constructor
    /// </summary>
    /// <param name="rhs"></param>
    Matrix(const Matrix& rhs)
    {
        if (!rhs.m_ChainMultiplicationEvaluated)
        {
            m_ChainMultiplicationEvaluated = false;
            m_MatrixInternal.rows() = rhs.m_ChainMultiplicationVector[0].getRows();
            m_MatrixInternal.columns() = rhs.m_ChainMultiplicationVector[rhs.m_ChainMultiplicationVector.size() - 1].getColumns();
            m_ChainMultiplicationVector = rhs.m_ChainMultiplicationVector;
           
        }
        else
        {
            m_ChainMultiplicationVector.clear();
            m_MatrixInternal = rhs.m_MatrixInternal;
            m_ChainMultiplicationEvaluated = true;
        }
        m_Tag = rhs.m_Tag;
        m_Transposed = rhs.m_Transposed;
    }

    /// <summary>
    /// Copy assignment
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    Matrix& operator=(const Matrix& rhs)
    {
        if (!rhs.m_ChainMultiplicationEvaluated)
        {
            this->m_ChainMultiplicationEvaluated = false;
            this->m_MatrixInternal.rows() = rhs.m_ChainMultiplicationVector[0].getRows();
            this->m_MatrixInternal.columns() = rhs.m_ChainMultiplicationVector[rhs.m_ChainMultiplicationVector.size() - 1].getColumns();
            this->m_ChainMultiplicationVector = rhs.m_ChainMultiplicationVector;
        }
        else
        {
            this->m_MatrixInternal = rhs.m_MatrixInternal;
            this->m_ChainMultiplicationEvaluated = true;
        }
        this->m_Tag = rhs.m_Tag;
        this->m_Transposed = rhs.m_Transposed;
        return *this;
    }

    /// <summary>
    /// Move constructor
    /// </summary>
    /// <param name="rhs"></param>
    Matrix(const Matrix&& rhs)
    {
        if (!rhs.m_ChainMultiplicationEvaluated)
        {
            m_ChainMultiplicationEvaluated = false;
            m_MatrixInternal.rows() = rhs.m_ChainMultiplicationVector[0].getRows();
            m_MatrixInternal.columns() = rhs.m_ChainMultiplicationVector[rhs.m_ChainMultiplicationVector.size() - 1].getColumns();
            m_ChainMultiplicationVector = std::move(rhs.m_ChainMultiplicationVector);
        }
        else
        {
            m_MatrixInternal = std::move(rhs.m_MatrixInternal);
            m_ChainMultiplicationEvaluated = true;
        }
        m_Tag = rhs.m_Tag;
        m_Transposed = rhs.m_Transposed;
    }

    /// <summary>
    /// Move assignment
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    Matrix& operator=(Matrix&& rhs)
    {
        if (!rhs.m_ChainMultiplicationEvaluated)
        {
            this->m_ChainMultiplicationEvaluated = false;
            this->m_MatrixInternal.rows() = rhs.m_ChainMultiplicationVector[0].getRows();
            this->m_MatrixInternal.columns() = rhs.m_ChainMultiplicationVector[rhs.m_ChainMultiplicationVector.size() - 1].getColumns();
            this->m_ChainMultiplicationVector = std::move(rhs.m_ChainMultiplicationVector);
        }
        else
        {
            this->m_MatrixInternal = rhs.m_MatrixInternal;
            this->m_ChainMultiplicationEvaluated = true;
        }
        m_Tag = rhs.m_Tag;
        m_Transposed = rhs.m_Transposed;
        return *this;
    }

    bool isEvaluated() const 
    { 
        return m_ChainMultiplicationEvaluated;
    }
    void evaluate() const
    {
        if (!isEvaluated() && !this->m_ChainMultiplicationVector.empty())
        {
            auto matrix = MatrixUtils::template optimizeMultiply<Matrix>(m_ChainMultiplicationVector);
            this->m_MatrixInternal = matrix.m_MatrixInternal;
            this->m_ChainMultiplicationEvaluated = true;
            this->m_ChainMultiplicationVector.clear();
            this->m_Transposed = matrix.m_Transposed;
        }
    }
    
    std::string m_Tag;


    /// <summary>
    /// Returns the identity matrix. This redefined the matrix size
    /// </summary>
    /// <param name="rows">rows to be in the matrix</param>
    /// <param name="cols">columsn to be in the matrix</param>
    /// <returns></returns>
    Matrix<T> identityfy( size_t rows, size_t cols)
    {
        if (!rows || !cols)
        {
            throw std::runtime_error("Can not identityfy with 0 rows or columns. Exiting\n");
        }
        // Forget everything
        m_Transposed = false;
        this->data().clear();
        this->data().resize(rows*cols);
        this->m_MatrixInternal.rows() = rows;
        this->m_MatrixInternal.columns() = cols;
        return identityfy();
    }

    /// <summary>
    /// Makes the existing matrix identity. This does not redefine the matrix size
    /// </summary>
    /// <returns></returns>
    Matrix<T> identityfy()
    {
        if constexpr (std::is_arithmetic_v<T>)
		{
            auto rows = getRows();
            auto cols = getColumns();
			for (auto ii = 0u; ii < rows; ii++)
				for (auto jj = 0u; jj < cols; jj++)
				{
					if (ii == jj)
						(*this)(ii, jj) = static_cast<T>(1);
					else
						(*this)(ii, jj) = static_cast<T>(0);
				}
			return *this;
		}
        else if constexpr (is_complex_v<T>())
        {
            auto rows = getRows();
            auto cols = getColumns();
            for (auto ii = 0u; ii < rows; ii++)
            {
                for (auto jj = 0u; jj < cols; jj++)
                {
                    if (ii == jj)
                    {
                        if constexpr (is_complex_integral_v<T>())
                            (*this)(ii, jj) = T(1, 0);
                        else
                            (*this)(ii, jj) = T(1.0, 0.0);
                    }
                    else
                    {
                        if constexpr (is_complex_integral_v<T>())
                            (*this)(ii, jj) = T(0, 0);
                        else
                            (*this)(ii, jj) = T(0.0, 0.0);
                    }
                }
            }
            return *this;
        }
        
        throw std::runtime_error("Unknown matrix type requested to identityfy(). Exiting\n");
        
    }
    
    /// <summary>
    /// Returns the transpose of the matrix. The current matrix is 
    /// unaffected
    /// </summary>
    /// <returns>Copy of the matrix which is transposed</returns>
    auto getTranspose() const
	{
		if (!isEvaluated())
		{
			this->evaluate();
			m_ChainMultiplicationEvaluated = true;
		}
		Matrix transp(*this);
		transp.transpose();
		return transp;
	}

    /// <summary>
    /// Modifies the current matrix to its transpose
    /// </summary>
    /// <returns>Returns the same current object after transposing</returns>
    Matrix& transpose()
    {
        if (!isEvaluated())
        {
            this->evaluate();
            m_ChainMultiplicationEvaluated = true;
        }
        m_Transposed = !m_Transposed;
        size_t tmp = getRows();
        m_MatrixInternal.rows() = getColumns();
        m_MatrixInternal.columns() = tmp;

        size_t nRows = m_MatrixInternal.rows();
        size_t nCols = m_MatrixInternal.columns();

        return *this;
    }

private:
    
    mutable bool m_Transposed = false;
    mutable std::vector<Matrix> m_ChainMultiplicationVector;
    mutable bool m_ChainMultiplicationEvaluated = true;
    
    /// <summary>
    /// Internal data storage for the matrix
    /// </summary>
    struct MatrixInternal
    {
        std::tuple< size_type, size_type, std::vector<T>> m_MatrixInternalTuple;
        MatrixInternal() {}
        MatrixInternal(size_type const r, size_type const c, const std::vector<T>& vector) :
            m_MatrixInternalTuple{ r,c,vector }
        {
            int aa = 0;
            ++aa;
        }
        MatrixInternal(size_type const r, size_type const c) :
            m_MatrixInternalTuple{ r,c,std::vector<T>(r * c) }
        {
            int aa = 0;
            ++aa;
        }
        size_type& rows() {
            return std::get<0>(m_MatrixInternalTuple);
        }
        size_type getRows() const {
            return std::get<0>(m_MatrixInternalTuple);
        }

        size_type& columns() {
            return std::get<1>(m_MatrixInternalTuple);
        }
        size_type getColumns() const {
            return std::get<1>(m_MatrixInternalTuple);
        }
        std::vector<T>& data() {
            return std::get<2>(m_MatrixInternalTuple);
        }
        const std::vector<T>& getData() const {
            return std::get<2>(m_MatrixInternalTuple);
        }
    };
    mutable MatrixInternal m_MatrixInternal;
    
    /// <summary>
    /// Getter of the element in the matrix 
    /// </summary>
    /// <param name="i">at ith row index</param>
    /// <param name="j">at jth column index</param>
    /// <returns></returns>
    size_t getMatrix2RowVectorIndex( const size_t& i, const size_t& j) const
    { 
        auto rows = m_MatrixInternal.getRows();
        auto cols = m_MatrixInternal.getColumns();
        if ( i<0 || i>= rows)
            throw std::runtime_error("Row index invalid");
        if (j<0 || j>= cols)
            throw std::runtime_error("Column index invalid");
        return i * cols + j;
    }
    

    /// <summary>
    /// Returns the element of the transpose of the current matrix
    /// </summary>
    /// <param name="i">element at ith row after transpose</param>
    /// <param name="j">element at jth column after transpose</param>
    /// <returns></returns>
    size_t getMatrixTransposeIndex(const size_t& i, const size_t& j) const
    {
        if (i < 0 || i >= m_MatrixInternal.getRows())
            throw std::runtime_error("Row index invalid");
        if (j < 0 || j >= m_MatrixInternal.getColumns())
            throw std::runtime_error("Column index invalid");
        auto cols = m_MatrixInternal.getColumns();
        auto rows = m_MatrixInternal.getRows();
        return i + rows * j;
    }

    /// <summary>
    /// Utility class that performs the matrix chain multiplication
    /// </summary>
    class MatrixUtils
    {
    public:

        /// <summary>
        /// This uses dynamic programming technique to find that couples of the matrices in 
        /// case of more than 2 multiplications, to find out the associative multiplication
        /// that can be achieved in lesser multiplications. 
        /// The d_vals vector stores the minimum number of multiplication. The K matrix is used to
        /// find the best associations of the matrices.
        /// 
        /// The matrix multiplication is lazy. evaluate() method is called only when needed (to query the element or
        /// when getting the data vector from the matrix)
        /// </summary>
        /// <typeparam name="MatrixType"></typeparam>
        /// <param name="inputChain"></param>
        /// <returns></returns>
        template<typename MatrixType>
        static MatrixType optimizeMultiply(std::vector<MatrixType>& inputChain)
        {
            // If all the matrices are square, this algorithm does not in its form, finds the less possible number of 
            // multiplications. So, hardMultiply is called.
            std::vector<MatrixType*> inpMatPtrChain;
            for (auto& chain : inputChain)
                inpMatPtrChain.emplace_back(&chain);

            bool isSquareMatrices = true;
            for (const auto& mat : inputChain)
            {
                isSquareMatrices = (mat.getRows() == mat.getColumns());
            }
            if ( isSquareMatrices || inputChain.size() ==2 )
            {
                MatrixType multRes = MatrixUtils::hardMultiply<MatrixType>(inpMatPtrChain);
                return multRes;
            }

            size_t inputChainSize = inputChain.size();
            size_t matSize = inputChainSize * inputChainSize;
            
            std::vector<std::vector<size_t>> cost(inputChainSize, std::vector<size_t>(inputChainSize,0));
            std::vector<std::vector<int>> kMatrix(inputChainSize, std::vector<int>(inputChainSize, -1));
            for (size_t ii = 0; ii < inputChainSize; ii++)
                kMatrix[ii][ii] = 0;
            
            
            // Compute D vector
            std::vector<size_t> d_vals;
            d_vals.push_back(inputChain[0].getRows());
            for (size_t ii=1; ii<inputChainSize; ii++ )
            {
                d_vals.push_back(inputChain[ii].getRows());
            }
            d_vals.push_back(inputChain[inputChainSize-1].getColumns());

          
            
            size_t ii = 0, incr = 1;
            size_t jj = incr;

            while (incr <= inputChainSize - 1)
            {
                ii = 0; jj = incr;
                while (jj < inputChainSize)
                {
                    //std::cout << "ii = " << ii << "  jj = " << jj << std::endl;
                    std::vector<size_t> costEvals;
                    cost[ii][jj] = std::numeric_limits<size_t>::max();
                    for (size_t kk = ii; kk < jj; kk++)
                    {
                        size_t costEval = cost[ii][kk] + cost[kk + 1][jj] + d_vals[ii] * d_vals[kk + 1] * d_vals[jj + 1];
                        if (costEval < cost[ii][jj])
                        {
                            cost[ii][jj] = costEval;
                            kMatrix[ii][jj] = int(kk);
                        }
                    }
                    ii++; jj++;
                }
                incr++;
            }

            /// <summary>
            /// The following code computes the matrix multiplication by 
            /// extracting computable matrices ( matrices that are in index next to each other)
            /// A binary expression tree is created. The nodes (matrices) are extracted 
            /// by prefix order traversal.
            /// </summary>
            /// <typeparam name="MatrixType"></typeparam>
            /// <param name="inputChain"></param>
            /// <returns></returns>

            std::vector<std::unique_ptr<MatrixType>> m_ResultsTemp;
            using ExpressionChain = std::tuple<size_t, size_t, std::vector<MatrixType*>>;
            
            auto treeRootElement = make_tuple(0, inpMatPtrChain.size() - 1, inpMatPtrChain);
            
            Tree<ExpressionChain> expressionTree(treeRootElement);


            auto buildTreeFn = [&inpMatPtrChain, &kMatrix](Node<ExpressionChain>* node) mutable {
                if (std::get<2>(node->getNodeObject()).size() > 2)
                {   
                    size_t st = std::get<0>(node->getNodeObject());
                    size_t end = std::get<1>(node->getNodeObject());
                    std::vector<MatrixType*> emptyVec1;
                    ExpressionChain partChain1 = std::make_tuple(st, kMatrix[st][end], emptyVec1);
                    for (size_t ii = st; ii <= kMatrix[st][end]; ii++)
                    {
                        std::get<std::vector<MatrixType*>>(partChain1).emplace_back(inpMatPtrChain[ii]);
                    }
                    node->setLeft(partChain1);

                    std::vector<MatrixType*> emptyVec2;
                    ExpressionChain partChain2 = std::make_tuple(kMatrix[st][end]+1, end, emptyVec2);
                    for (size_t ii = kMatrix[st][end]+1; ii<=end; ii++)
                    {
                        std::get<std::vector<MatrixType*>>(partChain2).emplace_back(inpMatPtrChain[ii]);
                    }
                    node->setRight(partChain2);
                }
            };

            expressionTree._BuildFn = buildTreeFn;
            expressionTree.buildByPreorderTraversal();
            std::vector<Node< ExpressionChain>*> prefixExpressions = expressionTree.getPreOrderExpression();

            std::stack< std::vector<MatrixType*>> matrixChainStack;
            std::vector< MatrixType*> resOperands;
            // Evaluate expressions using prefix expression
            for (auto revIter = prefixExpressions.rbegin();  revIter != prefixExpressions.rend(); revIter++)
            {
                ExpressionChain nodeChain = (*revIter)->getNodeObject();
                if (std::get<1>(nodeChain) - std::get<0>(nodeChain) +1 > 2) // like operator
                {
                    resOperands.clear();
                    std::vector<MatrixType*> operand1 = matrixChainStack.top();
                    matrixChainStack.pop();
                    std::vector<MatrixType*> operand2 = matrixChainStack.top();
                    matrixChainStack.pop();

                    m_ResultsTemp.emplace_back(std::make_unique<MatrixType>(hardMultiply<MatrixType>(operand1, operand2)));
                    //std::cout << res;
                    resOperands.emplace_back(m_ResultsTemp.back().get());
                    matrixChainStack.push(resOperands);

                }
                else
                {
                    std::vector<MatrixType*> obj = std::get<2>((*revIter)->getNodeObject());
                    matrixChainStack.push(obj);
                }
            }
            
            auto resPoppsed = matrixChainStack.top();
            matrixChainStack.pop();
            
            // If it throws exception, then the chain multiplication and subsequent 
            // hard multiplications are at fault.
            return *resPoppsed[0];

        }


        /// <summary>
        /// This is the typical matrix multiplication between two matrices
        /// in the association provided
        /// </summary>
        /// <typeparam name="MatrixType"></typeparam>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        template<typename MatrixType>
        static MatrixType hardMultiply(const std::vector<MatrixType*>& left,
            std::vector<MatrixType*>& right)
        {
            if (!left[0])
                throw std::runtime_error("Matrix null encountered. Exiting\n");
            MatrixType allLeftEvaluated = *left[0];
            for (int ii = 1; ii < left.size(); ii++)
            {
                if (!left[ii])
                    throw std::runtime_error("Matrix null encountered. Exiting\n");
                allLeftEvaluated = hardMultiply(allLeftEvaluated, *left[ii]);
            }
            MatrixType allRightEvaluated = *right[0];
            for (int ii = 1; ii < right.size(); ii++)
            {  
                allRightEvaluated = hardMultiply(allRightEvaluated, *right[ii]);
            }

            //std::cout << allLeftEvaluated << std::endl;
            //std::cout << allRightEvaluated << std::endl;
            MatrixType res = hardMultiply(allLeftEvaluated, allRightEvaluated);
            return res;
        }


        /// <summary>
        /// This is the typical matrix multiplication between two matrices
        /// in the association provided</summary>
        /// <typeparam name="MatrixType"></typeparam>
        /// <param name="chain"></param>
        /// <returns></returns>
        template<typename MatrixType>
        static MatrixType hardMultiply(const std::vector<MatrixType*>& chain)
        {
            MatrixType multEvaluated;
            if (chain.empty())
                return multEvaluated;
            if (!chain[0])
                throw std::runtime_error("Matrix null encountered. Exiting\n");
            multEvaluated = *chain[0];
            for (int ii = 1; ii < chain.size(); ii++)
            {
                if (!chain[ii])
                    throw std::runtime_error("Matrix null encountered. Exiting\n");
                multEvaluated = hardMultiply(multEvaluated, *chain[ii]);
            }
            return multEvaluated;
        }



        /// <summary>
        /// This is the typical matrix multiplication between two matrices
        /// in the association provided</summary>
        /// <typeparam name="MatrixType"></typeparam>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        template<typename MatrixType>
        static MatrixType hardMultiply(const MatrixType& left, const MatrixType& right)
        {
            if (left.getColumns() != right.getRows())
                throw std::logic_error("Matrix multiplication incompatible");

            auto tag = left.m_Tag + " " + right.m_Tag;
            MatrixType res{ tag, left.getRows(), right.getColumns()};

            size_t rows = left.getRows(), columns = right.getColumns(), interm = left.getColumns();
            for (size_t ii = 0; ii < rows; ii++)
            {
                for (size_t jj = 0; jj < columns; jj++)
                {
                    T resatij = 0;
                    for (size_t kk = 0; kk < interm; kk++)
                    {   
                        resatij += left(ii, kk) * right(kk, jj);
                    }
                    res(ii, jj) = resatij;
                }
            }

            return res;
        }

        // The following code was used to parallelize the multiplication of two matrices.
        // In the case when matrix size is more, an honest try was made to run the multiplication
        // using threadpool. I was debugging this as I had a few troubles in assynchronous fetching
        // The work is under progress

        // THREADPOOL MULTIPLY Implementation
        //template<typename MatrixType>
        //static MatrixType hardMultiply(const MatrixType& left, const MatrixType& right)
        //{
        //    if (left.getColumns() != right.getRows())
        //        throw std::logic_error("Matrix multiplication incompatible");

        //    auto tag = left.m_Tag + " " + right.m_Tag;
        //    MatrixType res{ tag, left.getRows(), right.getColumns() };

        //    size_t rows = left.getRows(), columns = right.getColumns(), interm = left.getColumns();
        //    {
        //        ThreadPool tp{ 36 };

        //        std::vector<std::function<void(void)>> m_Funs;
        //        for (size_t ii = 0; ii < rows; ii++)
        //        {
        //            m_Funs.emplace_back(([&left, ii = ii, &right, cols = columns, interm = interm, &res]()
        //                {
        //                    for (size_t jj = 0; jj < cols; jj++)
        //                    {
        //                        T resatij = 0;
        //                        for (size_t kk = 0; kk < interm; kk++)
        //                        {
        //                            resatij += left(ii, kk) * right(kk, jj);
        //                        }
        //                        res(ii, jj) = resatij;
        //                    }
        //                }));
        //            /*{
        //                ThreadPool tp;
        //                for (size_t jj = 0; jj < columns; jj++)
        //                {
        //                    T resatij = 0;
        //                    for (size_t kk = 0; kk < interm; kk++)
        //                    {
        //                        resatij += left(ii, kk) * right(kk, jj);
        //                    }
        //                    res(ii, jj) = resatij;
        //                }
        //            }*/
        //        }

        //        {
        //            for (auto ii = 0u; ii < rows; ii++)
        //                tp.submit(m_Funs[ii]);
        //        }
        //    }

        //    return res;
        //}
    };
};


