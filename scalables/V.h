#ifndef V_H
#define V_H

#include "scalable.h"
#include "P.h"

class V : public Scalable<double, 3> {
public:
    V(const Scalable<double, 3> &o);

    V(const P &o);// todo names

    V(double x, double y, double z);

    double &x();

    double x() const;

    double &y();

    double y() const;

    double &z();

    double z() const;

    friend double dot(const V &a, const V &b);

    friend V cross(const V &a, const V &b);

    double mag() const;

    V normalized() const;
};

#endif //V_H
