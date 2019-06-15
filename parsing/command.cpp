#include <memory>
#include "command.h"

using namespace std;

SurfaceGenerator::SurfaceGenerator(
        Eqptr kar, Eqptr kag, Eqptr kab,
        Eqptr kdr, Eqptr kdg, Eqptr kdb,
        Eqptr ksr, Eqptr ksg, Eqptr ksb) :
        kar(move(kar)), kag(move(kag)), kab(move(kab)),
        kdr(move(kdr)), kdg(move(kdg)), kdb(move(kdb)),
        ksr(move(ksr)), ksg(move(ksg)), ksb(move(ksb)) {}

unique_ptr<Surface> SurfaceGenerator::eval(double val) {
    return make_unique<Surface>(Scalable<double, 3>{{kar->eval(val), kag->eval(val), kab->eval(val)}},
                                Scalable<double, 3>{{kdr->eval(val), kdg->eval(val), kdb->eval(val)}},
                                Scalable<double, 3>{{ksr->eval(val), ksg->eval(val), ksb->eval(val)}});
}


LightGenerator::LightGenerator(Eqptr red, Eqptr green, Eqptr blue, Eqptr x, Eqptr y, Eqptr z) :
        red(move(red)), green(move(green)), blue(move(blue)), x(move(x)), y(move(y)), z(move(z)) {}

unique_ptr<Light> LightGenerator::eval(double val) {
    return make_unique<Light>(Color{red->eval(val), green->eval(val), blue->eval(val)},
                              Point{x->eval(val), y->eval(val), z->eval(val)});
}

DRAW::DRAW(Sgptr sgptr) : sgptr(move(sgptr)) {}

unique_ptr<Surface> DRAW::surface(unsigned int frame_no) {
    return move(sgptr->eval(frame_no));
}

DRAW_SPHERE::DRAW_SPHERE(Sgptr sgptr, Eqptr cx, Eqptr cy, Eqptr cz, Eqptr radius)
        : DRAW(move(sgptr)), cx(move(cx)), cy(move(cy)), cz(move(cz)), radius(move(radius)) {}


unique_ptr<FaceList> DRAW_SPHERE::matrix(unsigned int frame_no) {
    auto t = make_unique<FaceList>();
    t->add_sphere({cx->eval(frame_no), cy->eval(frame_no), cz->eval(frame_no)}, radius->eval(frame_no));
    return t;
}

DRAW_TORUS::DRAW_TORUS(Sgptr sgptr, Eqptr cx, Eqptr cy, Eqptr cz, Eqptr inner_r, Eqptr outer_r) :
        DRAW(move(sgptr)), cx(move(cx)), cy(move(cy)), cz(move(cz)), inner_r(move(inner_r)), outer_r(move(outer_r)) {}


unique_ptr<FaceList> DRAW_TORUS::matrix(unsigned int frame_no) {
    auto t = make_unique<FaceList>();
    t->add_torus({cx->eval(frame_no), cy->eval(frame_no), cz->eval(frame_no)}, inner_r->eval(frame_no),
                 outer_r->eval(frame_no));
    return t;
}

DRAW_BOX::DRAW_BOX(Sgptr sgptr, Eqptr ulcx, Eqptr ulcy, Eqptr ulcz, Eqptr width, Eqptr height, Eqptr depth) :
        DRAW(move(sgptr)), ulcx(move(ulcx)), ulcy(move(ulcy)), ulcz(move(ulcz)),
        width(move(width)), height(move(height)), depth(move(depth)) {}

unique_ptr<FaceList> DRAW_BOX::matrix(unsigned int frame_no) {
    auto t = make_unique<FaceList>();
    t->add_box({ulcx->eval(frame_no), ulcy->eval(frame_no), ulcz->eval(frame_no)}, width->eval(frame_no), width->eval(frame_no),
               depth->eval(frame_no));
    return t;
}

MOVE::MOVE(Eqptr x, Eqptr y, Eqptr z) : x(move(x)), y(move(y)), z(move(z)) {}

unique_ptr<ModifierMatrix> MOVE::matrix(unsigned int frame_no) {
    return make_unique<TranslationMatrix>(x->eval(frame_no), y->eval(frame_no), z->eval(frame_no));
}

SCALE::SCALE(Eqptr x, Eqptr y, Eqptr z) : x(move(x)), y(move(y)), z(move(z)) {}

unique_ptr<ModifierMatrix> SCALE::matrix(unsigned int frame_no) {
    return make_unique<ScaleMatrix>(x->eval(frame_no), y->eval(frame_no), z->eval(frame_no));
}

ROTATE::ROTATE(RotationMatrix::Axis axis, Eqptr degrees) : axis(axis), degrees(move(degrees)) {}

unique_ptr<ModifierMatrix> ROTATE::matrix(unsigned int frame_no) {
    return make_unique<RotationMatrix>(axis, degrees->eval(frame_no));
}

PUSH::PUSH() = default;

POP::POP() = default;