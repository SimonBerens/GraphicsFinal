#ifndef CMD_H
#define CMD_H

#include <memory>
#include <variant>
#include "sym.h"
#include "../matrices/rm.h"
#include "../equation_parser.h"
#include "../matrices/face.h"
#include "../matrices/tm.h"
#include "../matrices/sm.h"
#include "../draw.h"

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
        PUSH, POP, DISPLAY, DRAW_PTR, MODIFY_CS_PTR> Command;

struct WORLD {

    WORLD(unsigned int startFrame, unsigned int endFrame) : start_frame(startFrame), end_frame(endFrame) {
        commands.reserve(16);
    }

    typedef std::shared_ptr<WORLD> WORLD_PTR;

    typedef std::variant<DRAW_PTR, MODIFY_CS_PTR, WORLD_PTR> Command;
    std::vector<Command> commands;

    unsigned int start_frame;
    unsigned int end_frame;

    template<int width, int height>
    void exec_world(Frame<width, height> &frame, unsigned int frame_no, M_Matrix base_cs = M_Matrix()) {
        if (frame_no >= start_frame && frame_no <= end_frame) {
            unsigned int relative_frame_no = frame_no - start_frame;
            for (auto &cmd: commands) {
                if (std::holds_alternative<DRAW_PTR>(cmd)) {
                    auto dptr = std::get<DRAW_PTR>(cmd);
                    frame.draw_faces(dptr->matrix(relative_frame_no)->mult(base_cs), *dptr->surface(relative_frame_no));
                } else if (std::holds_alternative<MODIFY_CS_PTR>(cmd)) {
                    auto m = std::get<MODIFY_CS_PTR>(cmd)->matrix(relative_frame_no);
                    m->mult(base_cs);
                    base_cs = *m;
                } else if (std::holds_alternative<WORLD_PTR>(cmd)) {
                    std::get<WORLD_PTR>(cmd)->exec_world(frame, frame_no, base_cs);
                }
            }
        }
    }
};

#endif //CMD_H
