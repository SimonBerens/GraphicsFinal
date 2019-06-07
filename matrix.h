#ifndef CURVES_MATRIX_H
#define CURVES_MATRIX_H

#include <vector>

struct Matrix {
    std::vector<double> m;
    int rows;


    int getRows() const;

    int getCols() const;

    Matrix(int rows, int cols);

    Matrix(Matrix &other);

    Matrix(Matrix &&other) noexcept;

    virtual ~Matrix() = default;

    double &elem(int row, int col);

    double elem(int row, int col) const;

    void print();

    void mult(const Matrix &manip);
};

#endif //CURVES_MATRIX_H
