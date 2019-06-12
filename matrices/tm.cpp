#include "tm.h"

TM::TM(double tx, double ty, double tz) : M_Matrix() {
    TM &m = *this;
    m(0, 3) = tx;
    m(1, 3) = ty;
    m(2, 3) = tz;
}

TM::TM(const Scalable<double, 3> &scalable) : TM(scalable[0], scalable[1], scalable[2]) {}
