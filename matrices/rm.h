#ifndef RM_H
#define RM_H

#include "m_matri.h"

enum Axis {
    X, Y, Z
};

struct RM : M_Matrix {
    RM(Axis axis, double degrees);

};

#endif //RM_H
