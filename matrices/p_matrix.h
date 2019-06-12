#ifndef P_MATRIX_H
#define P_MATRIX_H

#include <vector>
#include "../scalables/P.h"
#include "m_matri.h"

class P_Matrix {
public:

    P_Matrix();

    typedef unsigned int size_type;

    size_type cols() const;

    void add_point(P p); // todo const stuff

    P operator[](size_type col) const;

    P &operator[](size_type col);

    friend std::ostream &operator<<(std::ostream &os, const P_Matrix &matrix);

    void mult(const M_Matrix &modifier); // todo reorganize
private:
    std::vector<P> data;

    double operator()(size_type row, size_type col) const;

    double &operator()(size_type row, size_type col);

};

#endif //P_MATRIX_H
