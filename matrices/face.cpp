#include <cmath>
#include "face.h"

using namespace std;

void FL::add_face(P p0, P p1, P p2) {
    add_point(p0);
    add_point(p1);
    add_point(p2);
}

void FL::add_box(const P &upper_left_corner, double width, double height, double depth) {
    double x = upper_left_corner.x(), y = upper_left_corner.y(), z = upper_left_corner.z();
    double x1 = x + width, y1 = y - height, z1 = z - depth;
    // front face
    add_face({x, y, z}, {x, y1, z}, {x1, y1, z});
    add_face({x, y, z}, {x1, y1, z}, {x1, y, z});
    // left face
    add_face({x, y1, z}, {x, y, z1}, {x, y1, z1});
    add_face({x, y1, z}, {x, y, z}, {x, y, z1});
    // top face
    add_face({x1, y, z}, {x, y, z1}, {x, y, z});
    add_face({x1, y, z}, {x1, y, z1}, {x, y, z1});
    // bottom face
    add_face({x1, y1, z1}, {x, y1, z}, {x, y1, z1});
    add_face({x1, y1, z1}, {x1, y1, z}, {x, y1, z});

    add_face({x1, y1, z1}, {x1, y, z}, {x1, y1, z});
    add_face({x1, y1, z1}, {x1, y, z1}, {x1, y, z});

    add_face({x, y1, z1}, {x1, y, z1}, {x1, y1, z1});
    add_face({x, y1, z1}, {x, y, z1}, {x1, y, z1});
}

void FL::add_sphere(const P &center, double r) {
    const double cx = center.x(), cy = center.y(), cz = center.z();
    FL temp;
    int psteps = 20, tsteps = 20;
    for (int pstep = 0; pstep <= psteps; pstep++) {
        double phi = 2 * M_PI / psteps * pstep;
        for (int tstep = 0; tstep <= tsteps; tstep++) {
            double theta = M_PI / tsteps * tstep;
            temp.add_point({r * cos(theta) + cx, r * sin(theta) * cos(phi) + cy, r * sin(theta) * sin(phi) + cz});
        }
    }
    tsteps += 1;
    for (int i = 0; i < temp.cols() - tsteps; i++) {
        add_face(temp[i], temp[i + 1], temp[i + tsteps + 1]);
        add_face(temp[i + 1], temp[i + tsteps + 2], temp[i + tsteps + 1]);
        if (i % tsteps == tsteps - 3)
            i += 2;
    }
}

void FL::add_torus(const P &center, double r0, double r1) {
    double cx = center.x(), cy = center.y(), cz = center.z();
    FL temp;
    int psteps = 100, tsteps = 50;
    for (int pstep = 0; pstep <= psteps; pstep++) {
        double phi = 2 * M_PI / psteps * pstep;
        for (int tstep = 0; tstep <= tsteps; tstep++) {
            double theta = 2 * M_PI / tsteps * tstep;
            temp.add_point({cos(phi) * (r0 * cos(theta) + r1) + cx,
                            r0 * sin(theta) + cy,
                            -sin(phi) * (r0 * cos(theta) + r1) + cz});
        }
    }
    tsteps += 1;
    for (int i = 0; i < temp.cols() - tsteps; i++) {
        add_face(temp[i], temp[i + tsteps], temp[i + 1]);
        add_face(temp[i + tsteps], temp[i + tsteps + 1], temp[i + 1]);
        if (i % tsteps == tsteps - 2)
            i++;
    }
}

FL &FL::mult(const M_Matrix &modifier) {
    P_Matrix::mult(modifier);
    return *this;
}
