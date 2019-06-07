#ifndef CURVES_COLOR_H
#define CURVES_COLOR_H

#include <fstream>
#include "math_vec.h"

struct Color {
    u_char red;
    u_char blue;
    u_char green;

    Color(double red, double green, double blue);

    Color(const Vec_3D &v);

    Color();

    operator Vec_3D();

    void bound();
};

std::ostream &operator<<(std::ostream &strm, Color &c);

#endif //CURVES_COLOR_H
