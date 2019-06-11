#include <cmath>
#include "V.h"

V::V(const Scalable<double, 3> &o) : Scalable(o) {}

V::V(const P &o) : V(o.x(), o.y(), o.z()) {}

V::V(double x, double y, double z) {
    data = {x, y, z};
}

double &V::x() {
    return data[0];
}

double V::x() const {
    return data[0];
}

double &V::y() {
    return data[1];
}

double V::y() const {
    return data[1];
}

double &V::z() {
    return data[2];
}

double V::z() const {
    return data[2];
}

double dot(const V &a, const V &b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

V cross(const V &a, const V &b) {
    return {a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()};
}

double V::mag() const {
    return sqrt(x() * x() + y() * y() + z() * z());
}

V V::normalized() const {
    double m = mag();
    return {x() / m, y() / m, z() / m};
}

