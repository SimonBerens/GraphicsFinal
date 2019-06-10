#include "sm.h"

SM::SM(double sx, double sy, double sz) {
    SM &m = *this;
    m(0, 0) = sx;
    m(1, 1) = sy;
    m(2, 2) = sz;
}