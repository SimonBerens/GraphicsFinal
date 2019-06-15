#ifndef TRANSLATION_MATRIX_H
#define TRANSLATION_MATRIX_H

#include "modifier_matrix.h"
#include "../scalables/scalable.h"

struct TranslationMatrix : ModifierMatrix {
    TranslationMatrix(double tx, double ty, double tz);

    TranslationMatrix(const Scalable<double, 3> &scalable);

};

#endif //TRANSLATION_MATRIX_H
