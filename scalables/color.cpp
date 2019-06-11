#include "color.h"

using namespace std;

Color::Color(const Scalable<double, 3> &o) : Scalable(o) {}

Color::Color(double red, double green, double blue) : Scalable() {
    data = {red, green, blue};
}


Color::Color() : Scalable() {
    data = {0, 0, 0};
}

double &Color::red() {
    return data[0];
}

double Color::red() const {
    return data[0];
}

double &Color::blue() {
    return data[1];
}

double Color::blue() const {
    return data[1];
}

double &Color::green() {
    return data[2];
}

double Color::green() const {
    return data[2];
}

ostream &operator<<(ostream &strm, const Color &c) {
    for (auto &val: c.data)
        strm << static_cast<int>(Color::bound(val)) << " ";
    return strm;
}

u_char Color::bound(double v) {
    return static_cast<u_char>(max(0., min(v, 255.)));
}
