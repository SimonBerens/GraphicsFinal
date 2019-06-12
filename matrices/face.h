#ifndef FINAL_FACE_H
#define FINAL_FACE_H

#include "p_matrix.h"

struct FL : P_Matrix {
    void add_face(P p0, P p1, P p2); // todo &&

    void add_box(const P &upper_left_corner, double width, double height, double depth);

    void add_sphere(const P &center, double r);

    void add_torus(const P &center, double r0, double r1);
};

#endif //FINAL_FACE_H
