#include <stdlib.h>
#include <iostream>

#include "matrix.h"

Matrix::Matrix(int rows, int cols) {
    m.reserve(rows * cols);
    this->rows = rows;
}

void Matrix::print() {
    for (unsigned int i = 0; i < getRows(); ++i) {
        for (unsigned int j = i; j < m.size(); j += getRows())
            std::cout << m[j] << " ";
        std::cout << std::endl;
    }
}

inline double &Matrix::elem(int row, int col) {
    return this->m[col * getRows() + row];
}

inline double Matrix::elem(int row, int col) const {
    return this->m[col * getRows() + row];
}

void Matrix::mult(const Matrix &manip) {
    if (getRows() != manip.getCols())
            throw std::runtime_error("Matrix dimension mismatch");
    int common = getRows();
    auto *col_cpy = new double[common];
    for (int col = 0; col < getCols(); ++col) {
        for (int row = 0; row < getRows(); ++row)
            col_cpy[row] = elem(row, col);
        for (int row = 0; row < common; ++row) {
            double new_val = 0;
            for (int i = 0; i < common; ++i)
                new_val += manip.elem(row, i) * col_cpy[i];
            elem(row, col) = new_val;
        }
    }
    delete[](col_cpy);
}

Matrix::Matrix(Matrix &other) : m(other.m) , rows(other.rows){}

Matrix::Matrix(Matrix &&other) noexcept: m(std::move(other.m)), rows(other.rows) {}

inline int Matrix::getRows() const {
    return rows;
}

inline int Matrix::getCols() const {
    return static_cast<int>(m.size()) / getRows();
}


