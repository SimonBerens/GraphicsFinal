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

Parser::Parser() : static_image(false), basename("default"), frames(1) {
    commands.reserve(128);
}

void Parser::add_surface(const std::string &name, Sgptr sgprt) { // todo rename add to h
    surfaces.insert_or_assign(name, sgprt);
}

const Sgptr Parser::find_surface(const std::string &name) const {
    return surfaces.at(name);
}


void Parser::add_eq(const std::string &name, Eqptr eqptr) {
    equations.insert_or_assign(name, eqptr);

}

const Eqptr Parser::find_eq(const std::string &name) {
    try {
        stod(name);
    } catch (invalid_argument &e) {
        return equations.at(name);
    }
    add_eq(name, std::make_shared<Eq>(name, 0)); // todo change return
    return equations.at(name);
}

void Parser::add_command(Command &&command) {
    commands.push_back(command);
}

void Parser::lex(std::istream &is) {
    string line;
    int line_no = 0;
    while (getline(is, line)) {
        stringstream ss(line);
        string command;
        ss >> command;
        if (command == "constants") {
            string name, kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb;
            ss >> name >> kar >> kag >> kab >> kdr >> kdg >> kdb >> ksr >> ksg >> ksb;
            add_surface(name, std::make_shared<SurfaceGenerator>(
                    find_eq(kar), find_eq(kag), find_eq(kab),
                    find_eq(kdr), find_eq(kdg), find_eq(kdb),
                    find_eq(ksr), find_eq(ksg), find_eq(ksb))
            );
        } else if (command == "sphere") {
            string surface_name, x, y, z, radius; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> radius;
            add_command(Command{in_place_index<0>, DRAW_SPHERE{
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(radius)
            }});
        } else if (command == "torus") {
            string surface_name, x, y, z, inner_r, outer_r; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> inner_r >> outer_r; // todo optional surface name
            add_command(Command{in_place_index<1>, DRAW_TORUS{
                    find_surface(surface_name),
                    find_eq(x), find_eq(y), find_eq(z), find_eq(inner_r), find_eq(outer_r)
            }});
        } else if (command == "box") {
            string surface_name, x, y, z, h, w, d; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> h >> w >> d;
            add_command(Command{in_place_index<2>, DRAW_BOX{
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(h), find_eq(w), find_eq(d)}
            });
        } else if (command == "line") {
            string x0, y0, z0, x1, y1, z1;
            ss >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
            add_command(Command{in_place_index<3>, DRAW_LINE{
                    find_eq(x0), find_eq(y0), find_eq(z0), find_eq(x1), find_eq(y1), find_eq(z1)
            }});
        } else if (command == "move") {
            string x, y, z;
            ss >> x >> y >> z;
            add_command(Command{in_place_index<4>,
                                MOVE{find_eq(x), find_eq(y), find_eq(z)}}); // todo unique ptr polymorphism
        } else if (command == "scale") {
            string x, y, z;
            ss >> x >> y >> z;
            add_command(Command{in_place_index<5>, SCALE{find_eq(x), find_eq(y), find_eq(z)}});
        } else if (command == "rotate") {
            string axis, degrees;
            ss >> axis >> degrees;
            add_command(Command{in_place_index<6>, ROTATE{
                    axis == "X" ? RM::X : axis == "Y" ? RM::Y : RM::Z, // todo error check this
                    find_eq(degrees)}});
        } else if (command == "push") {
            // todo check line is empty
            add_command(Command{in_place_index<7>, PUSH{}});
        } else if (command == "pop") {
            // todo check line is empty
            add_command(Command{in_place_index<8>, POP{}});
        } else if (command == "display") {
            // todo check line is empty
            add_command(Command{in_place_index<9>, DISPLAY{}});
        } else if (command == "basename") {
            ss >> basename;
        } else if (command == "frames") {
            ss >> frames;
        } else if (command == "vary") {
            string name, eq_tok, eq, mode;
            double sf, ef;
            ss >> name >> sf >> ef;
            ss >> eq_tok;
            eq = eq_tok;
            if (eq_tok == "(") { // todo error checking
                int parens = 1;
                while (parens) {
                    ss >> eq_tok;
                    eq += eq_tok;
                    if (eq_tok == "(")
                        parens++;
                    else if (eq_tok == ")")
                        parens--;
                }
            }
            ss >> mode;
            add_eq(name, std::make_shared<Eq>(name, mode == "relative" ? sf : 0)); // todo error check mode
        }
        line_no++;

    }
}

void Parser::parse() {

    // lighting defaults
    Surface default_lighting{{0.5}, {0.5}, {0.5}};


    system("mkdir build");
    chdir("./build");

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
                if (holds_alternative<MOVE>(cmd)) { // todo reorder
                    double scale = 1;
                    TM t(get<MOVE>(cmd)(f));
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (holds_alternative<ROTATE>(cmd)) { // todo refactor to use polymorphism of unique_ptr
                    RM t(get<ROTATE>(cmd)(f));
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (holds_alternative<SCALE>(cmd)) {
                    SM t(get<SCALE>(cmd)(f));
                    t.mult(cs_stack.top());
                    cs_stack.pop();
                    cs_stack.push(t);
                } else if (holds_alternative<DRAW_BOX>(cmd)) {
                    FL t(get<DRAW_BOX>(cmd)(f));
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, default_lighting); // todo fix params so constants works
                } else if (holds_alternative<DRAW_SPHERE>(cmd)) {
                    FL t(get<DRAW_SPHERE>(cmd)(f));
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, default_lighting); // todo refactor mult to return edited matrix
                } else if (holds_alternative<DRAW_TORUS>(cmd)) {
                    FL t(get<DRAW_TORUS>(cmd)(f));
                    t.mult(cs_stack.top());
                    frame.draw_faces(t, default_lighting);
                } else if (holds_alternative<DRAW_LINE>(cmd)) {
                    EL t(get<DRAW_LINE>(cmd)(f));
                    t.mult(cs_stack.top());
                    frame.draw_lines(t);
                } else if (holds_alternative<DISPLAY>(cmd)) {
                    FILE *fd = popen("display", "w");
                    frame.write_to(fd);
                    pclose(fd);
                }
            }
            stringstream fname;
            fname << setfill('0') << setw(3) << f;
            frame.save(basename + fname.str());
        }
    }
    chdir("..");
    system((string("convert -delay 1.7 build/") + basename + "* " + basename + ".gif").c_str());
}
