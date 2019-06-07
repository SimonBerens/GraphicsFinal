#include "color.h"

using namespace std;

Color::Color(double red, double green, double blue) :
        red(static_cast<unsigned char>(red)),
        green(static_cast<unsigned char>(green)),
        blue(static_cast<unsigned char>(blue)) {}

Color::Color() : Color(0, 0, 0) {}

Color::Color(const Vec_3D &v) :
        Color(v.x > 255 ? 255 : (v.x < 0 ? 0 : v.x),
              v.y > 255 ? 255 : (v.y < 0 ? 0 : v.y),
              v.z > 255 ? 255 : (v.z < 0 ? 0 : v.z)) {}

void Color::bound() {
    red = min(max(static_cast<u_char>(0), red), static_cast<u_char>(255));
    green = min(max(static_cast<u_char>(0), green), static_cast<u_char>(255));
    blue = min(max(static_cast<u_char>(0), blue), static_cast<u_char>(255));
}

Color::operator Vec_3D() {
    return Vec_3D(red, green, blue);
}

ostream &operator<<(ostream &strm, Color &c) {
    return strm << static_cast<int>(c.red) << " "
                << static_cast<int>(c.green) << " "
                << static_cast<int>(c.blue) << " ";
}