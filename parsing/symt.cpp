#include <iostream>
#include <unistd.h>
#include <stack>
#include <sstream>
#include <iomanip>
#include "symt.h"
#include "../draw.h"
#include "../matrices/tm.h"
#include "../matrices/sm.h"
#include "cmd.h"

using namespace std;

Parser::Parser() : static_image(false) {
    commands.reserve(128);
}

void Parser::add_symbol(const std::string &name, const SymData &data) {
    symbol_table.insert_or_assign(name, data);
}

const SymData &Parser::lookup_symbol(const std::string &name) {
    return symbol_table.at(name);
}

void Parser::add_command(Command &&command) {
    commands.push_back(command);
}

void Parser::lex(std::istream &is) {
    string line;
    int line_no = 0;
    while (getline(is, line)) {
        stringstream ss(line);
        string raw_token;
        ss >> raw_token;

        if (raw_token == "constants") {
            string name, kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb;
            ss >> name >> kar >> kag >> kab >> kdr >> kdg >> kdb >> ksr >> ksg >> ksb;
//            add_symbol(name, surface);
        } else if (raw_token == "sphere") {
            string surface_name, x, y, z, radius; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> radius;
//            add_command(Command{in_place_index<1>, DRAW_SPHERE{surface_name});
        } else if (raw_token == "torus") {
            string surface_name, x, y, z, inner_r, outer_r; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> inner_r >> outer_r; // todo optional surface name
            add_command(Command{in_place_index<2>, DRAW_TORUS{surface_name, {x, y, z}, inner_r, outer_r}});
        } else if (raw_token == "box") {
            string surface_name, x, y, z, h, w, d; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> h >> w >> d;
            add_command(Command{in_place_index<3>, DRAW_BOX{surface_name, {x, y, z}, h, w, d}});
        } else if (raw_token == "line") {
            string x0, y0, z0, x1, y1, z1;
            ss >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
            add_command(Command{in_place_index<4>, DRAW_LINE{{x0, y0, z0},
                                                             {x1, y1, z1}}});
        } else if (raw_token == "move") {
            string x, y, z;
            ss >> x >> y >> z;
            add_command(Command{in_place_index<5>, MOVE{}});
        } else if (raw_token == "scale") {
            string x, y, z;
            ss >> x >> y >> z;
            add_command(Command{in_place_index<6>, SCALE{}});
        } else if (raw_token == "rotate") {
            string axis, degrees;
            ss >> axis >> degrees;
            add_command(Command{in_place_index<7>, ROTATE{}});
        } else if (raw_token == "push") {
            // todo check line is empty
            add_command(Command{in_place_index<8>, PUSH{}});
        } else if (raw_token == "pop") {
            // todo check line is empty
            add_command(Command{in_place_index<9>, POP{}});
        } else if (raw_token == "display") {
            // todo check line is empty
            add_command(Command{in_place_index<10>, DISPLAY{}});
        } else if (raw_token == "basename") {
            ss >> basename;
        } else if (raw_token == "frames") {
            ss >> frames;
        } else if (raw_token == "vary") {
            string name, sf, ef, eq, mode;
            ss >> name >> sf >> ef >> eq >> mode;
            add_symbol(name,)
        } else if (raw_token == "save") {
            string name;
            ss >> name;
            add_command(Command{in_place_index<14>, SAVE{name}});
        }
        line_no++;

    }
}

void Parser::parse() {

    // lighting defaults
    Surface default_lighting{{0.5},
                             {0.5},
                             {0.5}};

    // first pass
    int frames = 0;
    string basename("default");
    bool vary_exists = false;

    for (auto &cmd: commands) {
        if (holds_alternative<SET_FRAMES>(cmd)) {
            frames = get<SET_FRAMES>(cmd).num_frames;
        } else if (holds_alternative<VARY>(cmd)) {
            vary_exists = true;
        } else if (holds_alternative<SET_BASENAME>(cmd)) {
            basename = get<SET_BASENAME>(cmd).name;
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
        for (auto &cmd: commands) {
            if (holds_alternative<VARY>(cmd)) {
                int sf = get<VARY>(cmd).start_frame, ef = get<VARY>(cmd).end_frame;
                double sv = get<VARY>(cmd).start_val, ev = get<VARY>(cmd).end_val;
                if (sf < 0 || ef >= frames) {
                    cout << "Uh oh vary range invalid" << endl;
                    return;
                }
                double v = sv, inc = (ev - sv) / (ef - sf);
                for (int j = sf; j <= ef; ++j, v += inc) {
                    knobs[j][get<VARY>(cmd).name] = v;
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
        for (auto &cmd: commands) {
            if (holds_alternative<PUSH>(cmd)) {
                cs_stack.push(M_Matrix(cs_stack.top()));
            } else if (holds_alternative<POP>(cmd)) {
                cs_stack.pop();
            } else {
                if (holds_alternative<MOVE>(cmd)) {
                    double scale = 1;
                    if (get<MOVE>(cmd).scale_factor_name.empty())
                        scale = knobs[f][get<MOVE>(cmd).scale_factor_name];
                    TM t(get<MOVE>(cmd).params * scale);
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (holds_alternative<ROTATE>(cmd)) {
                    auto axis = static_cast<RM::Axis>(get<ROTATE>(cmd).axis);
                    double scale = 1;
                    if (get<ROTATE>(cmd).scale_factor_name.empty())
                        scale = knobs[f][get<ROTATE>(cmd).scale_factor_name];
                    RM t(axis, get<ROTATE>(cmd).degrees * scale);
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (holds_alternative<SCALE>(cmd)) {
                    double scale = 1;
                    if (get<SCALE>(cmd).scale_factor_name.empty())
                        scale = knobs[f][get<SCALE>(cmd).scale_factor_name];
                    SM t(get<SCALE>(cmd).params * scale);
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (holds_alternative<DRAW_BOX>(cmd)) {
                    FL t;
                    t.add_box(get<DRAW_BOX>(cmd).upper_left_corner, get<DRAW_BOX>(cmd).width, get<DRAW_BOX>(cmd).height,
                              get<DRAW_BOX>(cmd).depth);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, get<DRAW_BOX>(cmd).surface_name.empty() ? get<Surface>(
                            symbol_table.at(get<DRAW_BOX>(cmd).surface_name))
                                                                                : default_lighting);
                } else if (holds_alternative<DRAW_SPHERE>(cmd)) {
                    FL t;
                    t.add_sphere(get<DRAW_SPHERE>(cmd).center, get<DRAW_SPHERE>(cmd).radius);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, get<DRAW_BOX>(cmd).surface_name.empty() ? get<Surface>(
                            symbol_table.at(get<DRAW_SPHERE>(cmd).surface_name))
                                                                                : default_lighting);
                } else if (holds_alternative<DRAW_TORUS>(cmd)) {
                    FL t;
                    t.add_torus(get<DRAW_TORUS>(cmd).center, get<DRAW_TORUS>(cmd).inner_r,
                                get<DRAW_TORUS>(cmd).outer_r);
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, get<DRAW_BOX>(cmd).surface_name.empty() ? get<Surface>(
                            symbol_table.at(get<DRAW_TORUS>(cmd).surface_name))
                                                                                : default_lighting);
                } else if (holds_alternative<DRAW_LINE>(cmd)) {
                    EL t;
                    t.add_edge({get<DRAW_LINE>(cmd).p0[0], get<DRAW_LINE>(cmd).p0[1], get<DRAW_LINE>(cmd).p0[2]},
                               {get<DRAW_LINE>(cmd).p1[0], get<DRAW_LINE>(cmd).p1[1], get<DRAW_LINE>(cmd).p1[2]});
                    t.mult(cs_stack.top());
                    frame.draw_lines(t);
                } else if (!vary_exists && holds_alternative<SAVE>(cmd)) {
                    frame.save(get<SAVE>(cmd).name);
                } else if (!vary_exists && holds_alternative<DISPLAY>(cmd)) {
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
