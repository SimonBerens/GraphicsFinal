#ifndef EDGELIST_H
#define EDGELIST_H

#include "p_matrix.h"

struct EL : P_Matrix {
    void add_edge(P p0, P p1);
};

#endif //EDGELIST_H
