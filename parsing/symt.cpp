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

Parser::Parser() : static_image(true), basename("default"), frames(1), base(make_shared<WORLD>(0, 0)) {
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
    return add_eq(name, std::make_shared<Eq>(name));
}


void Parser::add_command(Command &&command) {
    commands.push_back(command);
}

void Parser::lex(std::istream &is) { // todo rename
    stack<WORLD::WORLD_PTR> worlds;
    worlds.push(base); // todo constructor?
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
        } else if (command == "vary") {
            static_image = false;
            string name, eq;
            ss >> name;
            getline(ss, eq);
            add_eq(name, std::make_shared<Eq>(eq)); // todo error check mode
        } else if (command == "light") {
            string r, g, b, x, y, z;
            ss >> r >> g >> b >> x >> y >> z;
            lights.push_back(make_shared<LightGenerator>(find_eq(r), find_eq(g), find_eq(b), find_eq(x), find_eq(y),
                                                         find_eq(z)));
        } else if (command == "sphere") {
            string surface_name, x, y, z, radius; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> radius;
            worlds.top()->commands.emplace_back(in_place_index<0>, make_shared<DRAW_SPHERE>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(radius)));
        } else if (command == "torus") {
            string surface_name, x, y, z, inner_r, outer_r; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> inner_r >> outer_r; // todo optional surface name
            worlds.top()->commands.emplace_back(in_place_index<0>, make_shared<DRAW_TORUS>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(inner_r), find_eq(outer_r)
            ));
        } else if (command == "box") {
            string surface_name, x, y, z, h, w, d; // todo optional surface name
            ss >> surface_name >> x >> y >> z >> h >> w >> d;
            worlds.top()->commands.emplace_back(in_place_index<0>, make_shared<DRAW_BOX>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(h), find_eq(w),
                    find_eq(d)));
        } else if (command == "move") {
            string x, y, z;
            ss >> x >> y >> z;
            worlds.top()->commands.emplace_back(in_place_index<1>,
                                                make_shared<MOVE>(find_eq(x), find_eq(y), find_eq(z)));
        } else if (command == "scale") {
            string x, y, z;
            ss >> x >> y >> z;
            worlds.top()->commands.emplace_back(
                    in_place_index<1>, make_shared<SCALE>(find_eq(x), find_eq(y), find_eq(z)));
        } else if (command == "rotate") {
            string axis, degrees;
            ss >> axis >> degrees;
            worlds.top()->commands.emplace_back(in_place_index<1>, make_shared<ROTATE>(
                    axis == "X" ? RM::X : axis == "Y" ? RM::Y : RM::Z, // todo error check this
                    find_eq(degrees))); // todo lowercase
        } else if (command == "push") {
            double sf, ef;
            ss >> sf >> ef;
            auto world_ptr = make_shared<WORLD>(sf, ef);
            worlds.top()->commands.emplace_back(in_place_index<2>, world_ptr);
            worlds.push(world_ptr);
        } else if (command == "pop") {
            worlds.pop();
        } else if (command == "display") {
            // todo remove?
            add_command(Command{in_place_index<2>, DISPLAY{}});
        } else if (command == "basename") {
            ss >> basename;
        } else if (command == "frames") {
            ss >> frames;
            base->end_frame = frames;
        }
        // todo check empty line
        line_no++;
    }
}

void Parser::parse() { // todo rename
    if (static_image) {
        Frame<500, 500> frame;
        base->exec_world(frame, 0, lights);
        frame.save(basename + ".png");
    } else {
        system("mkdir build");
        chdir("./build");
        for (int f = 0; f < frames; ++f) {
            cout << "Frame: " << f << endl;
            Frame<500, 500> frame;
            base->exec_world(frame, f, lights);
            stringstream fname;
            fname << setfill('0') << setw(3) << f;
            frame.save(basename + fname.str() + ".png");
        }
        chdir("..");
        system((string("convert -delay 1.7 build/") + basename + "* " + basename + ".gif").c_str());
        system("rm -rf ./build");
    }
}
