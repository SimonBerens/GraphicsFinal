#include <memory>
#include "cmd.h"

using namespace std;

SurfaceGenerator::SurfaceGenerator(
        const Eqptr &kar, const Eqptr &kag, const Eqptr &kab,
        const Eqptr &kdr, const Eqptr &kdg, const Eqptr &kdb,
        const Eqptr &ksr, const Eqptr &ksg, const Eqptr &ksb) :
        kar(kar), kag(kag), kab(kab),
        kdr(kdr), kdg(kdg), kdb(kdb),
        ksr(ksr), ksg(ksg), ksb(ksb) {}

std::unique_ptr<Surface> SurfaceGenerator::eval(double x) {
    return std::make_unique<Surface>(Scalable<double, 3>{{kar->eval(x), kag->eval(x), kab->eval(x)}}, // bruh
                                     Scalable<double, 3>{{kdr->eval(x), kdg->eval(x), kdb->eval(x)}},
                                     Scalable<double, 3>{{ksr->eval(x), ksg->eval(x), ksb->eval(x)}});
}


DRAW::DRAW(Sgptr sgptr) : sgptr(sgptr) {} // todo move???

std::unique_ptr<Surface> DRAW::surface(unsigned int frame) {
    return move(sgptr->eval(frame));
}

DRAW_SPHERE::DRAW_SPHERE(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &radius)
        : DRAW(sgptr), cx(cx), cy(cy), cz(cz), radius(radius) {}

unique_ptr<FL> DRAW_SPHERE::matrix(unsigned int frame) {
    auto t = std::make_unique<FL>();
    t->add_sphere({cx->eval(frame), cy->eval(frame), cz->eval(frame)}, radius->eval(frame));
    return t;
}

DRAW_TORUS::DRAW_TORUS(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &innerR,
                       const Eqptr &outerR) : DRAW(sgptr), cx(cx), cy(cy), cz(cz), inner_r(innerR), outer_r(outerR) {}


unique_ptr<FL> DRAW_TORUS::matrix(unsigned int frame) {
    auto t = std::make_unique<FL>();
    t->add_torus({cx->eval(frame), cy->eval(frame), cz->eval(frame)}, inner_r->eval(frame), outer_r->eval(frame));
    return t;
}

DRAW_BOX::DRAW_BOX(const Sgptr &sgptr, const Eqptr &ulcx, const Eqptr &ulcy, const Eqptr &ulcz, const Eqptr &width,
                   const Eqptr &height, const Eqptr &depth) : DRAW(sgptr), ulcx(ulcx), ulcy(ulcy), ulcz(ulcz),
                                                              width(width), height(height), depth(depth) {}


unique_ptr<FL> DRAW_BOX::matrix(unsigned int frame) {
    auto t = std::make_unique<FL>();
    t->add_box({ulcx->eval(frame), ulcy->eval(frame), ulcz->eval(frame)}, width->eval(frame), width->eval(frame),
               depth->eval(frame));
    return t;
}

MOVE::MOVE(const Eqptr &x, const Eqptr &y, const Eqptr &z) : x(x), y(y), z(z) {}

std::unique_ptr<M_Matrix> MOVE::matrix(unsigned int frame) {
    return std::make_unique<TM>(x->eval(frame), y->eval(frame), z->eval(frame));
}

SCALE::SCALE(const Eqptr &x, const Eqptr &y, const Eqptr &z) : x(x), y(y), z(z) {}

std::unique_ptr<M_Matrix> SCALE::matrix(unsigned int frame) {
    return std::make_unique<SM>(x->eval(frame), y->eval(frame), z->eval(frame));
}

ROTATE::ROTATE(RM::Axis axis, const Eqptr &degrees) : axis(axis), degrees(degrees) {}

std::unique_ptr<M_Matrix> ROTATE::matrix(unsigned int frame) {
    return std::make_unique<RM>(axis, degrees->eval(frame));
}

PUSH::PUSH() = default;

POP::POP() = default;

DISPLAY::DISPLAY() = default;

