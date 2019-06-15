#ifndef FACE_LIST_H
#define FACE_LIST_H

#include "point_matrix.h"

struct FaceList : PointMatrix {

    void add_face(Point p0, Point p1, Point p2);

    void add_box(const Point &upper_left_corner, double width, double height, double depth);

    void add_sphere(const Point &center, double r);

    void add_torus(const Point &center, double r0, double r1);

    FaceList &mult(const ModifierMatrix &modifier) override;
};

#endif //FACE_LIST_H
