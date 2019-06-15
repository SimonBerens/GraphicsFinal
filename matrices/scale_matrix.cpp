#include "scale_matrix.h"

ScaleMatrix::ScaleMatrix(double sx, double sy, double sz) {
    ScaleMatrix &m = *this;
    m(0, 0) = sx;
    m(1, 1) = sy;
    m(2, 2) = sz;
}

ScaleMatrix::ScaleMatrix(const Scalable<double, 3> &scalable) : ScaleMatrix(scalable[0], scalable[1], scalable[2]) {}
