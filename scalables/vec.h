#ifndef VEC3D_H
#define VEC3D_H

#include "scalable.h"
#include "point.h"

class Vec3D : public Scalable<double, 3> {
public:
    Vec3D(const Scalable<double, 3> &o);

    Vec3D(const Point &o);

    Vec3D(double x, double y, double z);

    double &x();

    double x() const;

    double &y();

    double y() const;

    double &z();

    double z() const;

    friend double dot(const Vec3D &a, const Vec3D &b);

    friend Vec3D cross(const Vec3D &a, const Vec3D &b);

    double mag() const;

    Vec3D normalized() const;
};

#endif //VEC3D_H
