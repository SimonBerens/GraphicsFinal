#include <cmath>
#include "vec.h"

Vec3D::Vec3D(const Scalable<double, 3> &o) : Scalable(o) {}

Vec3D::Vec3D(const Point &o) : Vec3D(o.x(), o.y(), o.z()) {}

Vec3D::Vec3D(double x, double y, double z) {
    data = {x, y, z};
}

double &Vec3D::x() {
    return data[0];
}

double Vec3D::x() const {
    return data[0];
}

double &Vec3D::y() {
    return data[1];
}

double Vec3D::y() const {
    return data[1];
}

double &Vec3D::z() {
    return data[2];
}

double Vec3D::z() const {
    return data[2];
}

double dot(const Vec3D &a, const Vec3D &b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

Vec3D cross(const Vec3D &a, const Vec3D &b) {
    return {a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()};
}

double Vec3D::mag() const {
    return sqrt(x() * x() + y() * y() + z() * z());
}

Vec3D Vec3D::normalized() const {
    double m = mag();
    return {x() / m, y() / m, z() / m};
}

