#ifndef ROTATION_MATRIX_H
#define ROTATION_MATRIX_H

#include "modifier_matrix.h"


struct RotationMatrix : ModifierMatrix {
    enum Axis {
        X, Y, Z
    };

    RotationMatrix(Axis axis, double degrees);

};

#endif //ROTATION_MATRIX_H
