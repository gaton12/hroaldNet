#include "matrix.h"

matrix::matrix(int rowsInput, int colsInput) 
{
    rows = rowsInput;
    cols = colsInput;
    for (int x = 0; x < rows; x++)
    {
        elements.push_back(std::vector<double>());
        for (int y = 0; y < cols; y++)
        {
            elements[x].push_back(0.0);
        }
    }
    
}

matrix::matrix()
{
    matrix m(1, 1);
    *this = m;
}

matrix::matrix(int rowsInput, int colsInput, std::vector<double> inputElements)
{
    rows = rowsInput;
    cols = colsInput;
    for (int x = 0; x < rows; x++)
    {
        elements.push_back(std::vector<double>());
        for (int y = 0; y < cols; y++)
        {
            elements[x].push_back(inputElements[x*cols + y]);
        }
    }
}

int matrix::getRows()
{
    return rows;
}
int matrix::getCols()
{
    return cols;
}
double matrix::getElement(int row, int col)
{
    return elements[row][col];
}

void matrix::printMatrix() 
{
    std::cout << "[";
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            std::cout << elements[x][y] << ", ";
        }
        if (x != rows - 1)
        {
            std::cout << std::endl << " ";
        }
    }
    std::cout << "]" << std::endl;
}

void matrix::setElement(int row, int col, double value) 
{
    elements[row][col] = value;
}

matrix matrix::transpose() 
{
    matrix m2(cols, rows);
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            m2.setElement(y, x, elements[x][y]);
        }
    }
/*     elements = m2.elements;
    int tempInt = rows;
    rows = cols;
    cols = tempInt; */
    return m2;
}

matrix matrix::append(matrix m1) 
{
    matrix m2 = matrix(getRows(), getCols() + m1.getCols());

    for (int x = 0; x < getRows(); x++)
    {
        for (int y = 0; y < getCols(); y++)
        {
            m2.setElement(x, y, getElement(x, y));
        }
    }
    for (int x = 0; x < m1.getRows(); x++)
    {
        for (int y = 0; y < m1.getCols(); y++)
        {
            m2.setElement(x, getCols() + y, m1.getElement(x, y));
        }
    }

    return m2;
}

matrix& matrix::operator+=(const matrix& m1)
{
    matrix copyLHS = *this;
    matrix copyRHS = m1;
    for (int x = 0; x < copyLHS.getRows(); x++)
    {
        for (int y = 0; y < copyLHS.getCols(); y++)
        {
            elements[x][y] += copyRHS.getElement(x, y);
        }
    }
    return *this;
}

matrix matrix::operator+(const matrix& m) const
{
    matrix copyLHS = *this;
    copyLHS += m;
    return copyLHS;
}
matrix matrix::operator-(const matrix& m) const
{
    matrix copyLHS = *this;
    matrix copyRHS = m;
    for (int x = 0; x < copyLHS.getRows(); x++)
    {
        for (int y = 0; y < copyLHS.getCols(); y++)
        {
            copyLHS.setElement(x, y, (copyLHS.getElement(x, y) - copyRHS.getElement(x, y)));
        }
    }
    return copyLHS;

}


matrix& matrix::operator*=(const matrix& m1) {
    matrix copyLHS = *this;
    matrix copyRHS = m1;
    matrix m2(copyLHS.getRows(), copyRHS.getCols());
    double sum = 0.0;
    for (int x = 0; x < copyLHS.getRows(); x++)
    {
        for (int y = 0; y < copyRHS.getCols(); y++)
        {
            sum = 0.0;
            for (int z = 0; z < copyLHS.getCols(); z++)
            {
                sum += copyLHS.getElement(x, z) * copyRHS.getElement(z, y);
            }
            m2.setElement(x, y, sum);
        }
    }
    /* rows = m2.getRows();
    cols = m2.getCols();
    elements = m2.elements; */
    *this = m2;
    return *this;
}

matrix matrix::operator*(const double& f) const
{
    matrix copyLHS = *this;
    for (int x = 0; x < copyLHS.getRows(); x++)
    {
        for (int y = 0; y < copyLHS.getCols(); y++)
        {
            copyLHS.setElement(x, y, copyLHS.getElement(x, y) * f);
        }
        
    }
    return copyLHS;
}

matrix matrix::operator*(const matrix &m) const
{
    matrix copyLHS = *this;
    copyLHS *= m;
    return copyLHS;
}


/* matrix& matrix::operator=(const matrix &m)
{
    rows = m.getRows();
    cols = m.getCols();
    elements = m.elements;
    return *this;
} */

matrix dotProduct(matrix m1, matrix m2) 
{
    matrix m3(m1.getRows(), m2.getCols());
    for (int x = 0; x < m1.getRows(); x++)
    {
        for (int y = 0; y < m2.getCols(); y++)
        {
            double sum = 0.0;
            for (int z = 0; z < m1.getCols(); z++)
            {
                sum += m1.getElement(x, z) * m2.getElement(z, y);
            }
            m3.setElement(x, y, sum);
        }
    }
    return m3;
}

matrix sigmafy(matrix m1)
{
    matrix m2(m1.getRows(), m1.getCols());
    for (int x = 0; x < m1.getRows(); x++)
    {
        for (int y = 0; y < m1.getCols(); y++)
        {
            if (m1.getElement(x, y) >= 0)
            {
                m2.setElement(x, y, m1.getElement(x, y));
            }
            else
            {
                m2.setElement(x, y, 0);
            }
            
        }
    }
    return m2;
}

matrix sigmaDerivative(matrix m1) {
    matrix m2 = matrix(m1.getRows(), m1.getRows());
    for (int x = 0; x < m2.getRows(); x++)
    {
        for (int y = 0; y < m2.getRows(); y++)
        {
            if (x == y)
            {
                if (m1.getElement(x, 0) > 0) // nollan måste kanske ändras 
                {
                    m2.setElement(x, y, 1);
                }
                else
                {
                    m2.setElement(x, y, 0);
                }
            }
            else
            {
                m2.setElement(x, y, 0);
            }
        }
    }
    return m2;
}

matrix unityMatrix(int rows, int cols)
{
    matrix m(rows, cols);
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            if (x == y)
            {
                m.setElement(x, y, 1);
            }
            else
            {
                m.setElement(x, y, 0);
            }
        }
    }
    return m;
}