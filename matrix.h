#include <vector>
#include <iostream>

class matrix 
{
    int rows;
    int cols;
    std::vector<std::vector<double>> elements;
    public:
        matrix();
        matrix(int rowsInput, int colsInput);
        matrix(int rowsInput, int colsInput, std::vector<double> inputElements);

        int getRows();
        int getCols();
        double getElement(int row, int col);
        void setElement(int row, int col, double value);
        void printMatrix();

        matrix transpose();
        matrix append(matrix m1);

        matrix& operator+=(const matrix& m1);
        matrix operator+(const matrix& m) const;

        matrix operator-(const matrix& m) const;

        matrix& operator*=(const matrix& m1);
        matrix operator*(const double& f) const;
        matrix operator*(const matrix &m) const;

        matrix& operator=(const matrix& m) = default;

};

matrix dotProduct(matrix m1, matrix m2);
matrix sigmafy(matrix m1);
matrix sigmaDerivative(matrix m1);
matrix unityMatrix(int rows, int cols);