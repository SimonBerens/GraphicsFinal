#ifndef TM_H
#define TM_H

#include "m_matri.h"
#include "../scalables/scalable.h"

struct TM : M_Matrix {
    TM(double tx, double ty, double tz);

    TM(const Scalable<double, 3> &scalable);

};

#endif //TM_H
