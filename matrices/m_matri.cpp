#include <cmath>
#include "m_matri.h"

using namespace std;

M_Matrix::M_Matrix(bool identity) {
    if (identity) {
        M_Matrix &m = *this;
        for (int i = 0; i < 4; ++i)
            m(i, i) = 1;
    }
}

double M_Matrix::operator()(size_type row, size_type col) const {
    return data[col][row];
}

double &M_Matrix::operator()(size_type row, size_type col) {
    return data[col][row];
}

void M_Matrix::mult(const M_Matrix &other) {
    M_Matrix result(false), &m = *this;
    for (int res_col = 0; res_col < 4; ++res_col)
        for (int res_row = 0; res_row < 4; ++res_row)
            for (int i = 0; i < 4; ++i)
                result(res_row, res_col) += other(res_row, i) * m(i, res_col);
    m = result;
}

ostream &operator<<(ostream &os, const M_Matrix &matrix) {
    for (auto &row: matrix.data) {
        for (auto &val: row)
            os << val << " ";
        os << "\n";
    }
    return os;
}
