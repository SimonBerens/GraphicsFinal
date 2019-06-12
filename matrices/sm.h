#ifndef SM_H
#define SM_H

#include "m_matri.h"
#include "../scalables/scalable.h"

struct SM : M_Matrix {
    SM(double sx, double sy, double sz);

    SM(const Scalable<double, 3> &scalable);
};

#endif //SM_H
