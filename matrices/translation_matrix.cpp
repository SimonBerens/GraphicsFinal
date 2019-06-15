#include "translation_matrix.h"

TranslationMatrix::TranslationMatrix(double tx, double ty, double tz) : ModifierMatrix() {
    TranslationMatrix &m = *this;
    m(0, 3) = tx;
    m(1, 3) = ty;
    m(2, 3) = tz;
}

TranslationMatrix::TranslationMatrix(const Scalable<double, 3> &scalable) : TranslationMatrix(scalable[0], scalable[1], scalable[2]) {}
