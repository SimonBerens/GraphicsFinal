#include <cmath>
#include "rotation_matrix.h"

RotationMatrix::RotationMatrix(Axis axis, double degrees) {
    double rad = degrees * M_PI / 180;
    RotationMatrix &m = *this;
    switch (axis) {
        case X:
            m(1, 1) = cos(rad);
            m(1, 2) = -sin(rad);
            m(2, 1) = sin(rad);
            m(2, 2) = cos(rad);
            break;
        case Y:
            m(0, 0) = cos(rad);
            m(0, 2) = sin(rad);
            m(2, 0) = -sin(rad);
            m(2, 2) = cos(rad);
            break;
        case Z:
            m(0, 0) = cos(rad);
            m(0, 1) = -sin(rad);
            m(1, 0) = sin(rad);
            m(1, 1) = cos(rad);
            break;
    }
}
