#include <cmath>
#include "rotation_matrix.h"

RotationMatrix::RotationMatrix(Axis axis, double rads) {
    RotationMatrix &m = *this;
    switch (axis) {
        case X:
            m(1, 1) = cos(rads);
            m(1, 2) = -sin(rads);
            m(2, 1) = sin(rads);
            m(2, 2) = cos(rads);
            break;
        case Y:
            m(0, 0) = cos(rads);
            m(0, 2) = sin(rads);
            m(2, 0) = -sin(rads);
            m(2, 2) = cos(rads);
            break;
        case Z:
            m(0, 0) = cos(rads);
            m(0, 1) = -sin(rads);
            m(1, 0) = sin(rads);
            m(1, 1) = cos(rads);
            break;
    }
}
