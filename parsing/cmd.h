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

    std::unique_ptr<Surface> eval(double x);

private:
    Eqptr kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb;
};

typedef std::shared_ptr<SurfaceGenerator> Sgptr;


struct DRAW {
    explicit DRAW(Sgptr sgptr);

    std::unique_ptr<Surface> surface(unsigned int frame);

    virtual std::unique_ptr<FL> matrix(unsigned int frame) = 0;

protected:
    Sgptr sgptr;
};

typedef std::shared_ptr<DRAW> DRAW_PTR;

struct MODIFY_CS {
    virtual std::unique_ptr<M_Matrix> matrix(unsigned int frame) = 0;
};

typedef std::shared_ptr<MODIFY_CS> MODIFY_CS_PTR;

struct DRAW_SPHERE : DRAW {
    DRAW_SPHERE(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &radius);

    std::unique_ptr<FL> matrix(unsigned int frame) override;

private:
    Eqptr cx, cy, cz, radius;
};


struct DRAW_TORUS : DRAW {
    DRAW_TORUS(const Sgptr &sgptr, const Eqptr &cx, const Eqptr &cy, const Eqptr &cz, const Eqptr &innerR,
               const Eqptr &outerR);

    std::unique_ptr<FL> matrix(unsigned int frame) override;

private:
    Eqptr cx, cy, cz, inner_r, outer_r;
};

struct DRAW_BOX : DRAW {
    DRAW_BOX(const Sgptr &sgptr, const Eqptr &ulcx, const Eqptr &ulcy, const Eqptr &ulcz, const Eqptr &width,
             const Eqptr &height, const Eqptr &depth);

    std::unique_ptr<FL> matrix(unsigned int frame) override;

private:
    Eqptr ulcx, ulcy, ulcz, width, height, depth;
};

struct DRAW_LINE { // todo maybe draw
    DRAW_LINE(const Eqptr &x0, const Eqptr &y0, const Eqptr &z0, const Eqptr &x1, const Eqptr &y1, const Eqptr &z1);

    EL operator()(double x);

private:
    Eqptr x0, y0, z0, x1, y1, z1;
};

struct MOVE : MODIFY_CS {
    MOVE(const Eqptr &x, const Eqptr &y, const Eqptr &z);

    std::unique_ptr<M_Matrix> matrix(unsigned int frame) override;

private:
    Eqptr x, y, z;
};

struct SCALE : MODIFY_CS {
    SCALE(const Eqptr &x, const Eqptr &y, const Eqptr &z);

    std::unique_ptr<M_Matrix> matrix(unsigned int frame) override;

private:
    Eqptr x, y, z;
};

struct ROTATE : MODIFY_CS {
    ROTATE(RM::Axis axis, const Eqptr &degrees);

    std::unique_ptr<M_Matrix> matrix(unsigned int frame) override;

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
        PUSH, POP, DISPLAY, DRAW_PTR, MODIFY_CS_PTR, DRAW_LINE> Command;

#endif //CMD_H
