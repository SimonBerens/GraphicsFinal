#ifndef P_H
#define P_H

#include <array>
#include <ostream>
#include "scalable.h"

class P : public Scalable<double, 4> {
public:

    P(const Scalable<double, 4> &o);

    P(double x, double y, double z);

    double &x();

    double x() const;

    double &y();

    double y() const;

    double &z();

    double z() const;

    friend std::ostream &operator<<(std::ostream &os, const P &matrix);

    friend class P_Matrix;

    friend class M_Matrix;
};


#endif //P_H
