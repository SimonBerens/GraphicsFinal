#include "cmd.h"

SurfaceGenerator::SurfaceGenerator(
        const Eqptr &kar, const Eqptr &kag, const Eqptr &kab,
        const Eqptr &kdr, const Eqptr &kdg, const Eqptr &kdb,
        const Eqptr &ksr, const Eqptr &ksg, const Eqptr &ksb) :
        kar(kar), kag(kag), kab(kab),
        kdr(kdr), kdg(kdg), kdb(kdb),
        ksr(ksr), ksg(ksg), ksb(ksb) {}

Surface SurfaceGenerator::operator()(double x) {
    return Surface({{kar->eval(x), kag->eval(x), kab->eval(x)}},
                   {{kdr->eval(x), kdg->eval(x), kdb->eval(x)}},
                   {{ksr->eval(x), ksg->eval(x), ksb->eval(x)}});
}

DRAW_SPHERE::DRAW_SPHERE(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &radius)
        : sgptr(sgptr), cx(cx), cy(cy), cz(cz), radius(radius) {}

FL DRAW_SPHERE::operator()(double x) { // todo unique ptr?
    FL t;
    t.add_sphere({cx->eval(x), cy->eval(x), cz->eval(x)}, radius->eval(x));
    return t;
}

DRAW_TORUS::DRAW_TORUS(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &innerR,
                       const Eqptr &outerR) : sgptr(sgptr), cx(cx), cy(cy), cz(cz), inner_r(innerR), outer_r(outerR) {}

FL DRAW_TORUS::operator()(double x) {// todo unique ptr?
    FL t;
    t.add_torus({cx->eval(x), cy->eval(x), cz->eval(x)}, inner_r->eval(x), outer_r->eval(x));
    return t;
}

DRAW_BOX::DRAW_BOX(const Sgptr &sgptr, const Eqptr &ulcx, const Eqptr &ulcy, const Eqptr &ulcz, const Eqptr &width,
                   const Eqptr &height, const Eqptr &depth) : sgptr(sgptr), ulcx(ulcx), ulcy(ulcy), ulcz(ulcz),
                                                              width(width), height(height), depth(depth) {}

FL DRAW_BOX::operator()(double x) {// todo unique ptr?
    FL t;
    t.add_box({ulcx->eval(x), ulcy->eval(x), ulcz->eval(x)}, width->eval(x), width->eval(x), depth->eval(x));
    return t;
}

DRAW_LINE::DRAW_LINE(const Eqptr &x0, const Eqptr &y0, const Eqptr &z0, const Eqptr &x1, const Eqptr &y1,
                     const Eqptr &z1) : x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1) {}

EL DRAW_LINE::operator()(double x) {// todo unique ptr?
    EL t;
    t.add_edge({x0->eval(x), y0->eval(x), z0->eval(x)}, {x1->eval(x), y1->eval(x), z1->eval(x)});
    return t;
}

MOVE::MOVE(const Eqptr &x, const Eqptr &y, const Eqptr &z) : x(x), y(y), z(z) {}

TM MOVE::operator()(double x) {
    return {this->x->eval(x), y->eval(x), z->eval(x)};
}

SCALE::SCALE(const Eqptr &x, const Eqptr &y, const Eqptr &z) : x(x), y(y), z(z) {}

SM SCALE::operator()(double x) {
    return {this->x->eval(x), y->eval(x), z->eval(x)};
}

ROTATE::ROTATE(RM::Axis axis, const Eqptr &degrees) : axis(axis), degrees(degrees) {}

RM ROTATE::operator()(double x) {
    return {axis, degrees->eval(x)};
}

PUSH::PUSH() {}

POP::POP() {}

DISPLAY::DISPLAY() {}
