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


double &Color::green() {
    return data[1];
}

double Color::green() const {
    return data[1];
}

double &Color::blue() {
    return data[2];
}

double Color::blue() const {
    return data[2];
}


ostream &operator<<(ostream &strm, const Color &c) {
    for (auto &val: c.data)
        strm << static_cast<int>(val) << " ";
    return strm;
}

Color &Color::bound() {
    red() = static_cast<int>(max(0., min(red(), 255.)));
    green() = static_cast<int>(max(0., min(green(), 255.)));
    blue() = static_cast<int>(max(0., min(blue(), 255.)));
    return *this;
}
