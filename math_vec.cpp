#include <cmath>
#include "math_vec.h"

using namespace std;

Vec_3D::Vec_3D(double x, double y, double z) : x(x), y(y), z(z) {}

Vec_3D Vec_3D::operator+(const Vec_3D &other) const {
    return Vec_3D(x + other.x, y + other.y, z + other.z);
}

Vec_3D Vec_3D::operator-(const Vec_3D &other) const {
    return Vec_3D(x - other.x, y - other.y, z - other.z);
}

double Vec_3D::dot(const Vec_3D &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec_3D Vec_3D::cross(const Vec_3D &other) const {
    return Vec_3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

double Vec_3D::mag() const {
    return sqrt(x * x + y * y + z * z);
}

Vec_3D Vec_3D::normalize() {
    double m = mag();
    return {x / m, y / m, z / m};
}

Vec_3D operator*(double c, const Vec_3D &v) {
    return Vec_3D(c * v.x, c * v.y, c * v.z);
}

ostream &operator<<(ostream &strm, const Vec_3D &c) {
    return strm << c.x << " " << c.y << " " << c.z << " ";
}
