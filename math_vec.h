#ifndef MATH_VEC_H
#define MATH_VEC_H

#include <iostream>

struct Vec_3D {
    double x;
    double y;
    double z;

    Vec_3D(double x, double y, double z);

    Vec_3D operator+(const Vec_3D &other) const;

    Vec_3D operator-(const Vec_3D &other) const;

    friend Vec_3D operator*(double c, const Vec_3D &v);

    double dot(const Vec_3D &other) const;

    Vec_3D cross(const Vec_3D &other) const;

    double mag() const;

    Vec_3D normalize();

};

std::ostream &operator<<(std::ostream &strm, const Vec_3D &c);

#endif //MATH_VEC_H
