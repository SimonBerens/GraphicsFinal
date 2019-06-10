#include <stack>
#include "command.h"
#include <map>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <iostream>
#include <unistd.h>
#include "draw.h"
#include "y.tab.h"
#include "symbol_table.h"
#include "matrices/m_matri.h"
#include "matrices/EdgeList.h"
#include "matrices/face.h"
#include "matrices/tm.h"
#include "matrices/rm.h"
#include "matrices/sm.h" // todo organize

using namespace std;

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
    string basename("default");
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

    if (vary_exists && !frames) {
        cout << "Uh oh vary present but no frames set" << endl;
        return;
    }

    if (!frames)
        frames = 1;

    // second pass

    vector<map<string, double>> knobs(frames);
    if (vary_exists) {
        for (int i = 0; i < lastop; ++i) {
            command cmd = op[i];
            if (cmd.opcode == VARY) {
                int sf = cmd.op.vary.start_frame, ef = cmd.op.vary.end_frame;
                double sv = cmd.op.vary.start_val, ev = cmd.op.vary.end_val;
                if (sf < 0 || ef >= frames) {
                    cout << "Uh oh vary range invalid" << endl;
                    return;
                }
                double v = sv, inc = (ev - sv) / (ef - sf);
                for (int j = sf; j <= ef; ++j, v += inc) {
                    knobs[j][cmd.op.vary.p->name] = v;
                }
            }
        }
    }


    if (vary_exists) {
        system("mkdir build");
        chdir("./build");
    }

    for (int f = 0; f < frames; ++f) {
        cout << "Frame: " << f << endl;
        Frame<500, 500> frame;
        stack<M_Matrix> cs_stack;
        cs_stack.emplace();
        for (int i = 0; i < lastop; ++i) {
            command cmd = op[i];
            if (cmd.opcode == PUSH) {
                cs_stack.push(M_Matrix(cs_stack.top()));
            } else if (cmd.opcode == POP) {
                cs_stack.pop();
            } else {
                if (cmd.opcode == MOVE) {
                    double scale = 1;
                    if (cmd.op.move.p)
                        scale = knobs[f][cmd.op.move.p->name];
                    TM t(cmd.op.move.d[0] * scale, cmd.op.move.d[1] * scale, cmd.op.move.d[2] * scale);
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (cmd.opcode == ROTATE) {
                    Axis axis = static_cast<Axis>(cmd.op.rotate.axis);
                    double scale = 1;
                    if (cmd.op.rotate.p)
                        scale = knobs[f][cmd.op.rotate.p->name];
                    RM t(axis, cmd.op.rotate.degrees * scale);
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (cmd.opcode == SCALE) {
                    double scale = 1;
                    if (cmd.op.scale.p)
                        scale = knobs[f][cmd.op.scale.p->name];
                    SM t(cmd.op.scale.d[0] * scale, cmd.op.scale.d[1] * scale, cmd.op.scale.d[2] * scale);
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (cmd.opcode == BOX) {
                    FL t;
                    t.add_box(cmd.op.box.d0[0], cmd.op.box.d0[1], cmd.op.box.d0[2],
                              cmd.op.box.d1[0], cmd.op.box.d1[1], cmd.op.box.d1[2]);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, cmd.op.box.constants ? cmd.op.box.constants->s.c : default_lighting);
                } else if (cmd.opcode == SPHERE) {
                    FL t;
                    t.add_sphere(cmd.op.sphere.d[0], cmd.op.sphere.d[1], cmd.op.sphere.d[2], cmd.op.sphere.r);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, cmd.op.sphere.constants ? cmd.op.sphere.constants->s.c : default_lighting);
                } else if (cmd.opcode == TORUS) {
                    FL t;
                    t.add_torus(cmd.op.torus.d[0], cmd.op.torus.d[1], cmd.op.torus.d[2],
                                cmd.op.torus.r0, cmd.op.torus.r1);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, cmd.op.torus.constants ? cmd.op.torus.constants->s.c : default_lighting);
                } else if (cmd.opcode == LINE) {
                    EL t;
                    t.add_edge({cmd.op.line.p0[0], cmd.op.line.p0[1], cmd.op.line.p0[2]},
                               {cmd.op.line.p1[0], cmd.op.line.p1[1], cmd.op.line.p1[2]});
                    t.mult(cs_stack.top());
                    frame.draw_lines(t);
                } else if (!vary_exists && cmd.opcode == SAVE) {
                    frame.save(cmd.op.save.p->name);
                } else if (!vary_exists && cmd.opcode == DISPLAY) {
                    FILE *fd = popen("display", "w");
                    frame.write_to(fd);
                    pclose(fd);
                }
            }
            if (vary_exists) {
                stringstream fname;
                fname << setfill('0') << setw(3) << f;
                frame.save(basename + fname.str());
            }
        }
    }
    if (vary_exists) {
        chdir("..");
        system((string("convert -delay 1.7 build/") + basename + "* " + basename + ".gif").c_str());
    }


}