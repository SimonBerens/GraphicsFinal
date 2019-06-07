#define _USE_MATH_DEFINES

#include <cmath>
#include <functional>
#include "custom_matrices.h"


PointMatrix::PointMatrix() : Matrix(4, 60) {}

void PointMatrix::add_point(double x, double y, double z) {
    m.push_back(x);
    m.push_back(y);
    m.push_back(z);
    m.push_back(1);
}

Point PointMatrix::get_point(int n) const {
    return Point(elem(0, n), elem(1, n), elem(2, n));
}

Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}

void EdgeList::add_edge(double x0, double y0, double z0, double x1, double y1, double z1) {
    add_point(x0, y0, z0);
    add_point(x1, y1, z1);
}

void EdgeList::add_parametric(const std::function<double(double)>& fx, const std::function<double(double)>& fy, double step) {
    for (double t = 0; t < 1; t += step)
        add_edge(fx(t), fy(t), 0, fx(t + step), fy(t + step), 0);
}

void EdgeList::add_circle(double cx, double cy, double cz, double r) {
    add_parametric(
            [cx, r](double t) -> double { return r * cos(2 * M_PI * t) + cx; },
            [cy, r](double t) -> double { return r * sin(2 * M_PI * t) + cy; },
            0.01);
}

void EdgeList::add_hermite(double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1) {
    Modifier hm;
    hm.m = {2, -3, 0, 1,
            -2, 3, 0, 0,
            1, -2, 1, 0,
            1, -1, 0, 0}; // column major
    Matrix xm(4, 1);
    xm.m = {x0, x1, rx0, rx1};
    xm.mult(hm);
    Matrix ym(4, 1);
    ym.m = {y0, y1, ry0, ry1};
    ym.mult(hm);
    add_parametric(
            [&xm](double t) -> double {
                return xm.elem(0, 0) * pow(t, 3) +
                       xm.elem(1, 0) * pow(t, 2) +
                       xm.elem(2, 0) * pow(t, 1) +
                       xm.elem(3, 0);
            },
            [&ym](double t) -> double {
                return ym.elem(0, 0) * pow(t, 3) +
                       ym.elem(1, 0) * pow(t, 2) +
                       ym.elem(2, 0) * pow(t, 1) +
                       ym.elem(3, 0);
            },
            0.01);
}


void EdgeList::add_bezier(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
    Modifier bm;
    bm.m = {-1, 3, -3, 1,
            3, -6, 3, 0,
            -3, 3, 0, 0,
            1, 0, 0, 0};
    Matrix xm(4, 1);
    xm.m = {x0, x1, x2, x3};
    xm.mult(bm);
    Matrix ym(4, 1);
    ym.m = {y0, y1, y2, y3};
    ym.mult(bm);
    add_parametric(
            [&xm](double t) -> double {
                return xm.elem(0, 0) * pow(t, 3) +
                       xm.elem(1, 0) * pow(t, 2) +
                       xm.elem(2, 0) * pow(t, 1) +
                       xm.elem(3, 0);
            },
            [&ym](double t) -> double {
                return ym.elem(0, 0) * pow(t, 3) +
                       ym.elem(1, 0) * pow(t, 2) +
                       ym.elem(2, 0) * pow(t, 1) +
                       ym.elem(3, 0);
            },
            0.01);
}

Modifier::Modifier() : Matrix(4, 4) {
    ident();
}

void Modifier::ident() {
    m.clear();
    m.resize(rows * rows);
    for (int row = 0; row < getRows(); ++row)
        elem(row, row) = 1;
}

void Modifier::scale(const std::vector<double>& param_list) {
    Modifier t;
    for (int i = 0; i < 3; ++i)
        t.elem(i, i) = param_list[i];
    mult(t);
}

void Modifier::move(const std::vector<double>& param_list) {
    Modifier t;
    for (int i = 0; i < 3; ++i)
        t.elem(i, 3) = param_list[i];
    mult(t);
}

void Modifier::rot(const Axis axis, double theta) {
    Modifier t;
    double rad = theta * M_PI / 180;
    switch (axis) {
        case X:
            t.elem(1, 1) = cos(rad);
            t.elem(1, 2) = -sin(rad);
            t.elem(2, 1) = sin(rad);
            t.elem(2, 2) = cos(rad);
            break;
        case Y:
            t.elem(0, 0) = cos(rad);
            t.elem(0, 2) = sin(rad);
            t.elem(2, 0) = -sin(rad);
            t.elem(2, 2) = cos(rad);
            break;
        case Z:
            t.elem(0, 0) = cos(rad);
            t.elem(0, 1) = -sin(rad);
            t.elem(1, 0) = sin(rad);
            t.elem(1, 1) = cos(rad);
            break;
    }
    mult(t);
}

void TriangleMatrix::add_face(double x0, double y0, double z0,
                              double x1, double y1, double z1,
                              double x2, double y2, double z2) {
    add_point(x0, y0, z0);
    add_point(x1, y1, z1);
    add_point(x2, y2, z2);
}

inline void TriangleMatrix::add_face(Point &&p0, Point &&p1, Point &&p2) {
    add_face(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

void TriangleMatrix::add_box(double x, double y, double z, double width, double height, double depth) {
    double x1 = x + width, y1 = y - height, z1 = z - depth;
    // front face
    add_face(x, y, z, x, y1, z, x1, y1, z);
    add_face(x, y, z, x1, y1, z, x1, y, z);
    // left face
    add_face(x, y1, z, x, y, z1, x, y1, z1);
    add_face(x, y1, z, x, y, z, x, y, z1);
    // top face
    add_face(x1, y, z, x, y, z1, x, y, z);
    add_face(x1, y, z, x1, y, z1, x, y, z1);
    // bottom face
    add_face(x1, y1, z1, x, y1, z, x, y1, z1);
    add_face(x1, y1, z1, x1, y1, z, x, y1, z);

    add_face(x1, y1, z1, x1, y, z, x1, y1, z);
    add_face(x1, y1, z1, x1, y, z1, x1, y, z);

    add_face(x, y1, z1, x1, y, z1, x1, y1, z1);
    add_face(x, y1, z1, x, y, z1, x1, y, z1);
}

void TriangleMatrix::add_sphere(double cx, double cy, double cz, double r) {
    TriangleMatrix temp;
    int psteps = 20, tsteps = 20;
    for (int pstep = 0; pstep <= psteps; pstep++) {
        double phi = 2 * M_PI / psteps * pstep;
        for (int tstep = 0; tstep <= tsteps; tstep++) {
            double theta = M_PI / tsteps * tstep;
            temp.add_point(r * cos(theta) + cx, r * sin(theta) * cos(phi) + cy, r * sin(theta) * sin(phi) + cz);
        }
    }
    tsteps += 1;
    for (int i = 0; i < temp.getCols() - tsteps; i++) {
        add_face(temp.get_point(i), temp.get_point(i + 1), temp.get_point(i + tsteps + 1));
        add_face(temp.get_point(i + 1), temp.get_point(i + tsteps + 2), temp.get_point(i + tsteps + 1));
        if (i % tsteps == tsteps - 3)
            i += 2;
    }
}

void TriangleMatrix::add_torus(double cx, double cy, double cz, double r0, double r1) {
    TriangleMatrix temp;
    int psteps = 100, tsteps = 50;
    for (int pstep = 0; pstep <= psteps; pstep++) {
        double phi = 2 * M_PI / psteps * pstep;
        for (int tstep = 0; tstep <= tsteps; tstep++) {
            double theta = 2 * M_PI / tsteps * tstep;
            temp.add_point(cos(phi) * (r0 * cos(theta) + r1) + cx,
                           r0 * sin(theta) + cy,
                           -sin(phi) * (r0 * cos(theta) + r1) + cz);
        }
    }
    tsteps += 1;
    for (int i = 0; i < temp.getCols() - tsteps; i++) {
        add_face(temp.get_point(i), temp.get_point(i + tsteps), temp.get_point(i + 1));
        add_face(temp.get_point(i + tsteps), temp.get_point(i + tsteps + 1), temp.get_point(i + 1));
        if (i % tsteps == tsteps - 2)
            i++;
    }
}
