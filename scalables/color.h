#ifndef COLOR_H
#define COLOR_H

#include <fstream>
#include "scalable.h"

struct Color : public Scalable<double, 3> {

    Color(const Scalable<double, 3> &o);

    Color(double red, double green, double blue);

    Color();

    double &red();

    double red() const;

    double &blue();

    double blue() const;

    double &green();

    double green() const;

    Color &bound();

    friend std::ostream &operator<<(std::ostream &strm, const Color &c);

};


#endif //COLOR_H
