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

Parser::Parser() : static_image(true), basename("default"), frames(1) {
    commands.reserve(128);
}

Sgptr Parser::add_surface(const std::string &name, Sgptr sgprt) { // todo rename add to h
    return surfaces.insert_or_assign(name, sgprt).first->second;
}

const Sgptr Parser::find_surface(const std::string &name) const {
    return surfaces.at(name);
}


Eqptr Parser::add_eq(const std::string &name, Eqptr eqptr) {
    return equations.insert_or_assign(name, eqptr).first->second;
}

const Eqptr Parser::find_eq(const std::string &name) {
    try {
        stod(name);
    } catch (invalid_argument &e) {
        return equations.at(name);
    }
    return add_eq(name, std::make_shared<Eq>(name, 0));
}


void Parser::add_command(Command &&command) {
    commands.push_back(command);
}

void Parser::lex(std::istream &is) { // todo rename
    string line;
    int line_no = 0;
    while (getline(is, line)) {
        stringstream ss(line);
        string command;
        ss >> command;
        if (command == "constants") {
            string name, kar, kdr, ksr, kag, kdg, ksg, kab, kdb, ksb;
            ss >> name >> kar >> kdr >> ksr >> kag >> kdg >> ksg >> kab >> kdb >> ksb;
            add_surface(name, std::make_shared<SurfaceGenerator>(
                    find_eq(kar), find_eq(kag), find_eq(kab),
                    find_eq(kdr), find_eq(kdg), find_eq(kdb),
                    find_eq(ksr), find_eq(ksg), find_eq(ksb))
            );
        } else if (command == "sphere") {
            string surface_name, x, y, z, radius; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> radius;
            add_command(Command{in_place_index<3>, make_shared<DRAW_SPHERE>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(radius))});
        } else if (command == "torus") {
            string surface_name, x, y, z, inner_r, outer_r; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> inner_r >> outer_r; // todo optional surface name
            add_command(Command{in_place_index<3>, make_shared<DRAW_TORUS>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(inner_r), find_eq(outer_r)
            )});
        } else if (command == "box") {
            string surface_name, x, y, z, h, w, d; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> h >> w >> d;
            add_command(Command{in_place_index<3>, make_shared<DRAW_BOX>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(h), find_eq(w), find_eq(d))
            });
        } else if (command == "move") {
            string x, y, z;
            ss >> x >> y >> z;
            add_command(Command{in_place_index<4>, make_shared<MOVE>(find_eq(x), find_eq(y), find_eq(z))});
        } else if (command == "scale") {
            string x, y, z;
            ss >> x >> y >> z;
            add_command(Command{in_place_index<4>, make_shared<SCALE>(find_eq(x), find_eq(y), find_eq(z))});
        } else if (command == "rotate") {
            string axis, degrees;
            ss >> axis >> degrees;
            add_command(Command{in_place_index<4>, make_shared<ROTATE>(
                    axis == "X" ? RM::X : axis == "Y" ? RM::Y : RM::Z, // todo error check this
                    find_eq(degrees))});
        } else if (command == "push") {
            // todo check line is empty
            add_command(Command{in_place_index<0>, PUSH{}});
        } else if (command == "pop") {
            // todo check line is empty
            add_command(Command{in_place_index<1>, POP{}});
        } else if (command == "display") {
            // todo check line is empty
            add_command(Command{in_place_index<2>, DISPLAY{}});
        } else if (command == "basename") {
            ss >> basename;
        } else if (command == "frames") {
            ss >> frames;
        } else if (command == "vary") {
            static_image = false;
            string name, eq, mode;
            double sf, ef;
            ss >> name >> sf >> ef;
            string rev;
            getline(ss, rev);
            reverse(rev.begin(), rev.end());
            stringstream rev_ss(rev);
            rev_ss >> mode;
            reverse(mode.begin(), mode.end());
            getline(rev_ss, eq);
            reverse(eq.begin(), eq.end());
            add_eq(name, std::make_shared<Eq>(eq, mode == "relative" ? sf : 0)); // todo error check mode
        }
        line_no++;
    }
//    if ()
}

void Parser::parse() { // todo rename

    if (!static_image) {
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
                if (holds_alternative<DRAW_PTR>(cmd)) {
                    auto dptr = get<DRAW_PTR>(cmd);
                    frame.draw_faces(dptr->matrix(f)->mult(cs_stack.top()), *dptr->surface(f));
                } else if (holds_alternative<MODIFY_CS_PTR>(cmd)) {
                    auto m = get<MODIFY_CS_PTR>(cmd)->matrix(f);
                    m->mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(*m);
                } else if (holds_alternative<DISPLAY>(cmd)) {
                    FILE *fd = popen("display", "w");
                    frame.write_to(fd);
                    pclose(fd);
                }
            }
            if (static_image)
                frame.save(basename + ".png");
            else {
                stringstream fname;
                fname << setfill('0') << setw(3) << f;
                frame.save(basename + fname.str());
            }
        }
    }
    if (!static_image) {
        chdir("..");
        system((string("convert -delay 1.7 build/") + basename + "* " + basename + ".gif").c_str());
        system("rm -rf ./build");
    }
}
