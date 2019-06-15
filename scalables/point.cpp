#include "point.h"

using namespace std;

Point::Point(const Scalable<double, 4> &o) : Scalable(o) {}

Point::Point(double x, double y, double z) {
    data = {x, y, z, 1};
}

Point::Point() : Point(0, 0, 0) {}

double &Point::x() {
    return data[0];
}

double Point::x() const {
    return data[0];
}

double &Point::y() {
    return data[1];
}

double Point::y() const {
    return data[1];
}

double &Point::z() {
    return data[2];
}

double Point::z() const {
    return data[2];
}

ostream &operator<<(ostream &os, const Point &p) {
    return os << "(" << p.x() << " " << p.y() << " " << p.z() << ")";
}



