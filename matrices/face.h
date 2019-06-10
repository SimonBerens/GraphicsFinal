#ifndef FINAL_FACE_H
#define FINAL_FACE_H

#include "p_matrix.h"

struct FL : P_Matrix {
    void add_face(P p0, P p1, P p2); // todo &&

    void add_box(double x, double y, double z, double width, double height, double depth);

    void add_sphere(double cx, double cy, double cz, double r);

    void add_torus(double cx, double cy, double cz, double r0, double r1);
};

#endif //FINAL_FACE_H
