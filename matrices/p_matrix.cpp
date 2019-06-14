#include <iostream>
#include "p_matrix.h"

using namespace std;

P_Matrix::P_Matrix() {
    data.reserve(60);
}

P_Matrix::size_type P_Matrix::cols() const {
    return data.size();
}

double P_Matrix::operator()(size_type row, size_type col) const {
    return data[col].data[row];
}

double &P_Matrix::operator()(size_type row, size_type col) {
    return data[col].data[row];
}

P_Matrix &P_Matrix::mult(const M_Matrix &modifier) {
    P_Matrix &m = *this;
    for (int res_col = 0; res_col < cols(); ++res_col) {
        P p = data[res_col];
        for (int res_row = 0; res_row < 4; ++res_row) {
            double new_val = 0;
            for (int i = 0; i < 4; ++i)
                new_val += modifier(res_row, i) * p.data[i];
            m(res_row, res_col) = new_val;
        }
    }
    return *this;
}

void P_Matrix::add_point(P p) {
    data.push_back(p);
}

P P_Matrix::operator[](P_Matrix::size_type col) const {
    return data[col];
}

P &P_Matrix::operator[](P_Matrix::size_type col) {
    return data[col];
}

ostream &operator<<(ostream &os, const P_Matrix &matrix) {
    for (auto &p: matrix.data)
        os << p << "\n";
    return os;
}
