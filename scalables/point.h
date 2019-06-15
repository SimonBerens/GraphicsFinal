#ifndef POINT_H
#define POINT_H

#include <array>
#include <ostream>
#include "scalable.h"

class Point : public Scalable<double, 4> {
public:

    Point();

    Point(const Scalable<double, 4> &o);

    Point(double x, double y, double z);

    double &x();

    double x() const;

    double &y();

    double y() const;

    double &z();

    double z() const;

    friend std::ostream &operator<<(std::ostream &os, const Point &matrix);

};


#endif //POINT_H
