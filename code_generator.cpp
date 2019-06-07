#include <stack>
#include "command.h"
#include <map>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <unistd.h>
#include "draw.h"
#include "y.tab.h"
#include "symbol_table.h"

void kerchow() {

    // lighting defaults
    auto *default_lighting = new constants;
    for (int i = 0; i < 3; ++i)
        default_lighting->r[i] = 0.5;
    for (int i = 0; i < 3; ++i)
        default_lighting->g[i] = 0.5;
    for (int i = 0; i < 3; ++i)
        default_lighting->b[i] = 0.5;

    // first pass
    int frames = 0;
    std::string basename("default");
    bool vary_exists = false;
    for (int i = 0; i < lastop; ++i) {
        command cmd = op[i];
        if (cmd.opcode == FRAMES) {
            frames = cmd.op.frames.num_frames;
        } else if (cmd.opcode == VARY) {
            vary_exists = true;
        } else if (cmd.opcode == BASENAME) {
            basename = cmd.op.basename.p->name;
        }
    }

    if (!vary_exists) {

        if (!frames)
            frames = 1;
        for (int f = 0; f < frames; ++f) {
            Frame frame(500, 500);
            std::stack<Modifier> cs_stack;
            cs_stack.emplace();
            for (int i = 0; i < lastop; ++i) {
                command cmd = op[i];
                if (cmd.opcode == PUSH) {
                    cs_stack.push(Modifier(cs_stack.top()));
                } else if (cmd.opcode == POP) {
                    cs_stack.pop();
                } else {
                    if (cmd.opcode == MOVE) {
                        Modifier t;
                        t.move({cmd.op.move.d[0], cmd.op.move.d[1], cmd.op.move.d[2]});
                        t.mult(cs_stack.top());
                        cs_stack.pop();
                        cs_stack.push(std::move(t));
                    } else if (cmd.opcode == ROTATE) {
                        Modifier t;
                        Axis axis = cmd.op.rotate.axis == 0 ? X : cmd.op.rotate.axis == 1 ? Y : Z;
                        t.rot(axis, cmd.op.rotate.degrees);
                        t.mult(cs_stack.top());
                        cs_stack.pop();
                        cs_stack.push(std::move(t));
                    } else if (cmd.opcode == SCALE) {
                        Modifier t;
                        t.scale({cmd.op.scale.d[0], cmd.op.scale.d[1], cmd.op.scale.d[2]});
                        t.mult(cs_stack.top());
                        cs_stack.pop();
                        cs_stack.push(std::move(t));
                    } else if (cmd.opcode == BOX) {
                        TriangleMatrix t;
                        t.add_box(cmd.op.box.d0[0], cmd.op.box.d0[1], cmd.op.box.d0[2],
                                  cmd.op.box.d1[0], cmd.op.box.d1[1], cmd.op.box.d1[2]);
                        t.mult(cs_stack.top());
                        frame.draw_faces(t, cmd.op.box.constants ? cmd.op.box.constants->s.c : default_lighting);
                    } else if (cmd.opcode == SPHERE) {

                        TriangleMatrix t;
                        t.add_sphere(cmd.op.sphere.d[0], cmd.op.sphere.d[1], cmd.op.sphere.d[2], cmd.op.sphere.r);
                        t.mult(cs_stack.top());
                        frame.draw_faces(t, cmd.op.sphere.constants ? cmd.op.sphere.constants->s.c : default_lighting);
                    } else if (cmd.opcode == TORUS) {
                        TriangleMatrix t;
                        t.add_torus(cmd.op.torus.d[0], cmd.op.torus.d[1], cmd.op.torus.d[2], cmd.op.torus.r0,
                                    cmd.op.torus.r1);
                        t.mult(cs_stack.top());
                        frame.draw_faces(t, cmd.op.torus.constants ? cmd.op.torus.constants->s.c : default_lighting);
                    } else if (cmd.opcode == LINE) {
                        EdgeList t;
                        t.add_edge(cmd.op.line.p0[0], cmd.op.line.p0[1], cmd.op.line.p0[2], cmd.op.line.p1[0],
                                   cmd.op.line.p1[1], cmd.op.line.p1[2]);
                        t.mult(cs_stack.top());
                        frame.draw_lines(t);
                    } else if (cmd.opcode == SAVE) {
                        frame.save(cmd.op.save.p->name);
                    } else if (cmd.opcode == DISPLAY) {
                        FILE *fd = popen("display", "w");
                        frame.write_to(fd);
                        pclose(fd);
                    }
                }

            }
        }
        return;
    } else if (!frames) {
        std::cout << "Uh oh vary present but no frames set" << std::endl;
        return;
    }
    // second pass

    std::vector<std::map<std::string, double>> knobs(frames);
    for (int i = 0; i < lastop; ++i) {
        command cmd = op[i];
        if (cmd.opcode == VARY) {
            int sf = cmd.op.vary.start_frame, ef = cmd.op.vary.end_frame;
            double sv = cmd.op.vary.start_val, ev = cmd.op.vary.end_val;
            if (sf < 0 || ef >= frames) {
                std::cout << "Uh oh vary range invalid" << std::endl;
                return;
            }

            double v = sv, inc = (ev - sv) / (ef - sf);

            for (int j = sf; j <= ef; ++j, v += inc) {
                knobs[j][cmd.op.vary.p->name] = v;
            }
        }
    }


    system("mkdir build");
    chdir("./build");

    for (int f = 0; f < frames; ++f) {
        std::cout << "Frame: " << f << std::endl;
        Frame frame(500, 500);
        std::stack<Modifier> cs_stack;
        cs_stack.emplace();
        for (int i = 0; i < lastop; ++i) {
            command cmd = op[i];
            if (cmd.opcode == PUSH) {
                cs_stack.push(Modifier(cs_stack.top()));
            } else if (cmd.opcode == POP) {
                cs_stack.pop();
            } else {
                if (cmd.opcode == MOVE) {
                    Modifier t;
                    double scale = 1;
                    if (cmd.op.move.p)
                        scale = knobs[f][cmd.op.move.p->name];
                    t.move({cmd.op.move.d[0] * scale, cmd.op.move.d[1] * scale, cmd.op.move.d[2] * scale});
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(std::move(t));
                } else if (cmd.opcode == ROTATE) {

                    Modifier t;
                    Axis axis = cmd.op.rotate.axis == 0 ? X : cmd.op.rotate.axis == 1 ? Y : Z;
                    double scale = 1;

                    if (cmd.op.rotate.p)
                        scale = knobs[f][cmd.op.rotate.p->name];
                    t.rot(axis, cmd.op.rotate.degrees * scale);
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(std::move(t));
                } else if (cmd.opcode == SCALE) {
                    Modifier t;
                    double scale = 1;
                    if (cmd.op.scale.p)
                        scale = knobs[f][cmd.op.scale.p->name];
                    t.scale({cmd.op.scale.d[0] * scale, cmd.op.scale.d[1] * scale, cmd.op.scale.d[2] * scale});
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(std::move(t));
                } else if (cmd.opcode == BOX) {
                    TriangleMatrix t;
                    t.add_box(cmd.op.box.d0[0], cmd.op.box.d0[1], cmd.op.box.d0[2],
                              cmd.op.box.d1[0], cmd.op.box.d1[1], cmd.op.box.d1[2]);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, cmd.op.box.constants ? cmd.op.box.constants->s.c : default_lighting);
                } else if (cmd.opcode == SPHERE) {

                    TriangleMatrix t;
                    t.add_sphere(cmd.op.sphere.d[0], cmd.op.sphere.d[1], cmd.op.sphere.d[2], cmd.op.sphere.r);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, cmd.op.sphere.constants ? cmd.op.sphere.constants->s.c : default_lighting);
                } else if (cmd.opcode == TORUS) {
                    TriangleMatrix t;
                    t.add_torus(cmd.op.torus.d[0], cmd.op.torus.d[1], cmd.op.torus.d[2], cmd.op.torus.r0,
                                cmd.op.torus.r1);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, cmd.op.torus.constants ? cmd.op.torus.constants->s.c : default_lighting);
                } else if (cmd.opcode == LINE) {
                    EdgeList t;
                    t.add_edge(cmd.op.line.p0[0], cmd.op.line.p0[1], cmd.op.line.p0[2], cmd.op.line.p1[0],
                               cmd.op.line.p1[1], cmd.op.line.p1[2]);
                    t.mult(cs_stack.top());
                    frame.draw_lines(t);
                }
            }
            std::stringstream fname;
            fname << std::setfill('0') << std::setw(3) << f;
            frame.save(basename + fname.str());
        }
    }
    chdir("..");
    system((std::string("convert -delay 1.7 build/") + basename + "* " + basename + ".gif").c_str());


}