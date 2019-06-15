#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <variant>
#include "light.h"
#include "surface.h"
#include "../matrices/rotation_matrix.h"
#include "equation.h"
#include "../matrices/face_list.h"
#include "../matrices/translation_matrix.h"
#include "../matrices/scale_matrix.h"
#include "../draw.h"

typedef std::shared_ptr<Equation> Eqptr;

class SurfaceGenerator {
public:
    SurfaceGenerator(Eqptr kar, Eqptr kag, Eqptr kab,
                     Eqptr kdr, Eqptr kdg, Eqptr kdb,
                     Eqptr ksr, Eqptr ksg, Eqptr ksb);

    std::unique_ptr<Surface> eval(double val);

private:
    Eqptr kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb;
};

typedef std::shared_ptr<SurfaceGenerator> Sgptr;

class LightGenerator {
public:

    LightGenerator(Eqptr red, Eqptr green, Eqptr blue, Eqptr x, Eqptr y, Eqptr z);

    std::unique_ptr<Light> eval(double val);

private:
    Eqptr red, green, blue, x, y, z;
};

typedef std::shared_ptr<LightGenerator> Lgptr;

class AmbientGenerator {
public:
    AmbientGenerator(Eqptr red, Eqptr green, Eqptr blue);

    std::unique_ptr<Color> eval(double val);

private:
    Eqptr red, green, blue;
};

typedef std::shared_ptr<AmbientGenerator> Agptr;

struct DRAW {
    explicit DRAW(Sgptr sgptr);

    std::unique_ptr<Surface> surface(unsigned int frame_no);

    virtual std::unique_ptr<FaceList> matrix(unsigned int frame) = 0;

protected:
    Sgptr sgptr;
};

typedef std::shared_ptr<DRAW> DRAW_PTR;

struct MODIFY_CS {
    virtual std::unique_ptr<ModifierMatrix> matrix(unsigned int frame) = 0;
};

typedef std::shared_ptr<MODIFY_CS> MODIFY_CS_PTR;

struct DRAW_SPHERE : DRAW {
    DRAW_SPHERE(Sgptr sgptr, Eqptr cx, Eqptr cy, Eqptr cz, Eqptr radius);

    std::unique_ptr<FaceList> matrix(unsigned int frame_no) override;

private:
    Eqptr cx, cy, cz, radius;
};


struct DRAW_TORUS : DRAW {
    DRAW_TORUS(Sgptr sgptr, Eqptr cx, Eqptr cy, Eqptr cz, Eqptr inner_r,
               Eqptr outer_r);

    std::unique_ptr<FaceList> matrix(unsigned int frame_no) override;

private:
    Eqptr cx, cy, cz, inner_r, outer_r;
};

struct DRAW_BOX : DRAW {
    DRAW_BOX(Sgptr sgptr, Eqptr ulcx, Eqptr ulcy, Eqptr ulcz, Eqptr width,
             Eqptr height, Eqptr depth);

    std::unique_ptr<FaceList> matrix(unsigned int frame_no) override;

private:
    Eqptr ulcx, ulcy, ulcz, width, height, depth;
};

struct MOVE : MODIFY_CS {
    MOVE(Eqptr x, Eqptr y, Eqptr z);

    std::unique_ptr<ModifierMatrix> matrix(unsigned int frame_no) override;

private:
    Eqptr x, y, z;
};

struct SCALE : MODIFY_CS {
    SCALE(Eqptr x, Eqptr y, Eqptr z);

    std::unique_ptr<ModifierMatrix> matrix(unsigned int frame_no) override;

private:
    Eqptr x, y, z;
};

struct ROTATE : MODIFY_CS {
    ROTATE(RotationMatrix::Axis axis, Eqptr degrees);

    std::unique_ptr<ModifierMatrix> matrix(unsigned int frame_no) override;

private:
    RotationMatrix::Axis axis;
    Eqptr degrees;
};

struct PUSH {
    PUSH();
};

struct POP {
    POP();
};

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
    void exec_world(Frame<width, height> &frame, unsigned int frame_no,
                    const std::vector<Lgptr> &light_generators, ModifierMatrix base_cs = ModifierMatrix()) {
        if (frame_no >= start_frame && frame_no <= end_frame) {
            std::vector<Light> lights;
            std::transform(light_generators.begin(), light_generators.end(), std::back_inserter(lights),
                           [frame_no](const Lgptr &lgptr) -> Light { return *lgptr->eval(frame_no); });
            unsigned int relative_frame_no = frame_no - start_frame;
            for (auto &cmd: commands) {
                if (std::holds_alternative<DRAW_PTR>(cmd)) {
                    auto dptr = std::get<DRAW_PTR>(cmd);
                    frame.draw_faces(dptr->matrix(relative_frame_no)->mult(base_cs), *dptr->surface(relative_frame_no),
                                     lights);
                } else if (std::holds_alternative<MODIFY_CS_PTR>(cmd)) {
                    auto m = std::get<MODIFY_CS_PTR>(cmd)->matrix(relative_frame_no);
                    m->mult(base_cs);
                    base_cs = *m;
                } else if (std::holds_alternative<WORLD_PTR>(cmd)) {
                    std::get<WORLD_PTR>(cmd)->exec_world(frame, frame_no, light_generators, base_cs);
                }
            }
        }
    }
};

#endif //COMMAND_H
