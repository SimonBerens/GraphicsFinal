#include <iostream>
#include "point_matrix.h"

using namespace std;

PointMatrix::PointMatrix() {
    data.reserve(60);
}

PointMatrix::size_type PointMatrix::cols() const {
    return data.size();
}

double PointMatrix::operator()(size_type row, size_type col) const {
    return data[col][row];
}

double &PointMatrix::operator()(size_type row, size_type col) {
    return data[col][row];
}

PointMatrix &PointMatrix::mult(const ModifierMatrix &modifier) {
    PointMatrix &m = *this;
    for (int res_col = 0; res_col < cols(); ++res_col) {
        Point p = data[res_col];
        for (int res_row = 0; res_row < 4; ++res_row) {
            double new_val = 0;
            for (int i = 0; i < 4; ++i)
                new_val += modifier(res_row, i) * p[i];
            m(res_row, res_col) = new_val;
        }
    }
    return *this;
}

void PointMatrix::add_point(const Point &p) {
    data.push_back(p);
}

Point PointMatrix::operator[](PointMatrix::size_type col) const {
    return data[col];
}

Point &PointMatrix::operator[](PointMatrix::size_type col) {
    return data[col];
}

ostream &operator<<(ostream &os, const PointMatrix &matrix) {
    for (auto &p: matrix.data)
        os << p << "\n";
    return os;
}
