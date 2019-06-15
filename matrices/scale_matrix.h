#ifndef SCALE_MATRIX_H
#define SCALE_MATRIX_H

#include "modifier_matrix.h"
#include "../scalables/scalable.h"

struct ScaleMatrix : ModifierMatrix {
    ScaleMatrix(double sx, double sy, double sz);

    ScaleMatrix(const Scalable<double, 3> &scalable);
};

#endif //SCALE_MATRIX_H
