#ifndef CMD_H
#define CMD_H

#include <memory>
#include <variant>
#include "sym.h"
#include "../matrices/rm.h"
#include "../equation_parser.h"
#include "../matrices/face.h"
#include "../matrices/EdgeList.h"
#include "../matrices/tm.h"
#include "../matrices/sm.h"

typedef std::shared_ptr<Eq> Eqptr;

class SurfaceGenerator {
public:
// todo optimize pointers
    SurfaceGenerator(const Eqptr &kar, const Eqptr &kag, const Eqptr &kab,
                     const Eqptr &kdr, const Eqptr &kdg, const Eqptr &kdb,
                     const Eqptr &ksr, const Eqptr &ksg, const Eqptr &ksb);

    Surface operator()(double x);

private:
    Eqptr kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb;
};

typedef std::shared_ptr<SurfaceGenerator> Sgptr;

struct DRAW_SPHERE {
    DRAW_SPHERE(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &radius);

    FL operator()(double x);

private:
    Sgptr sgptr;
    Eqptr cx, cy, cz, radius;
};


struct DRAW_TORUS {
    DRAW_TORUS(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &innerR,
               const Eqptr &outerR);

    FL operator()(double x);

private:
    Sgptr sgptr;
    Eqptr cx, cy, cz, inner_r, outer_r;
};

struct DRAW_BOX {
    DRAW_BOX(const Sgptr &sgptr, const Eqptr &ulcx, const Eqptr &ulcy, const Eqptr &ulcz, const Eqptr &width,
             const Eqptr &height, const Eqptr &depth);

    FL operator()(double x);

private:
    Sgptr sgptr;
    Eqptr ulcx, ulcy, ulcz, width, height, depth;
};

struct DRAW_LINE {
    DRAW_LINE(const Eqptr &x0, const Eqptr &y0, const Eqptr &z0, const Eqptr &x1, const Eqptr &y1, const Eqptr &z1);

    EL operator()(double x);

private:
    Eqptr x0, y0, z0, x1, y1, z1;
};

struct MOVE {
    MOVE(const Eqptr &x, const Eqptr &y, const Eqptr &z);

    TM operator()(double x);

private:
    Eqptr x, y, z;
};

struct SCALE {
    SCALE(const Eqptr &x, const Eqptr &y, const Eqptr &z);

    SM operator()(double x);

private:
    Eqptr x, y, z;
};

struct ROTATE {
    ROTATE(RM::Axis axis, const Eqptr &degrees);

    RM operator()(double x);

private:
    RM::Axis axis;
    Eqptr degrees;
};

struct PUSH {
    PUSH();
};

struct POP {
    POP();
};

struct DISPLAY {
    DISPLAY();
};

typedef std::variant<
        DRAW_SPHERE, DRAW_TORUS, DRAW_BOX, DRAW_LINE, MOVE, SCALE, ROTATE, PUSH, POP, DISPLAY> Command;

#endif //CMD_H
