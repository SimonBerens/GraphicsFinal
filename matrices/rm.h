#ifndef RM_H
#define RM_H

#include "m_matri.h"


struct RM : M_Matrix {
    enum Axis {
        X, Y, Z
    };

    RM(Axis axis, double degrees);

};

#endif //RM_H
