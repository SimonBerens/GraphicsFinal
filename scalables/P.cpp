#include "P.h"

using namespace std;

P::P(const Scalable<double, 4> &o) : Scalable(o) {}

P::P(double x, double y, double z) {
    data = {x, y, z, 1};
}

double &P::x() {
    return data[0];
}

double P::x() const {
    return data[0];
}

double &P::y() {
    return data[1];
}

double P::y() const {
    return data[1];
}

double &P::z() {
    return data[2];
}

double P::z() const {
    return data[2];
}

ostream &operator<<(ostream &os, const P &p) {
    return os << "(" << p.x() << " " << p.y() << " " << p.z() << ")";
}



