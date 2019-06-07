#ifndef CURVES_CUSTOM_MATRICES_H
#define CURVES_CUSTOM_MATRICES_H

#include <functional>
#include "matrix.h"
#include "math_vec.h"

union Point {
    struct {
        double x;
        double y;
        double z;
    };
    Vec_3D vec;

    Point(double x, double y, double z);
};

struct PointMatrix : public Matrix {

    PointMatrix();

    void add_point(double x, double y, double z);

    Point get_point(int n) const;

};

struct EdgeList : public PointMatrix {

    void add_edge(double x0, double y0, double z0, double x1, double y1, double z1);

    void add_parametric(const std::function<double(double)> &fx, const std::function<double(double)> &fy, double step);

    void add_circle(double cx, double cy, double cz, double r);

    void add_hermite(double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1);

    void add_bezier(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);

};

enum Axis {
    X, Y, Z
};

struct Modifier : public Matrix {

    Modifier();

    void ident();

    void scale(const std::vector<double> &param_list);

    void move(const std::vector<double> &param_list);

    void rot(const Axis axis, double theta);
};

struct TriangleMatrix : public PointMatrix {

    void add_face(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);

    void add_face(Point &&p0, Point &&p1, Point &&p2);

    void add_box(double x, double y, double z, double width, double height, double depth);

    void add_sphere(double cx, double cy, double cz, double r);

    void add_torus(double cx, double cy, double cz, double r0, double r1);

};

#endif //CURVES_CUSTOM_MATRICES_H
