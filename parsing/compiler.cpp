#include <iostream>
#include <unistd.h>
#include <stack>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "compiler.h"
#include "../draw.h"
#include "../matrices/translation_matrix.h"
#include "../matrices/scale_matrix.h"
#include "command.h"

using namespace std;

MDL_Compiler::MDL_Compiler(const std::string &filename) :
        static_image(true), basename("default"), frames(1), base(make_shared<WORLD>(0, 0)) {
    ifstream file(filename);
    try {
        pre_process(file);
        execute();
    } catch (Equation::EquationParsingException &e) {
        cout << e.what() << endl;
    } catch (MDL_ParsingException &e) {
        cout << e.what() << endl;
    }

}

Sgptr MDL_Compiler::add_surface(const std::string &name, Sgptr sgprt) {
    return surfaces.insert_or_assign(name, sgprt).first->second;
}

const Sgptr MDL_Compiler::find_surface(const std::string &name) const {
    return surfaces.at(name);
}


Eqptr MDL_Compiler::add_eq(const std::string &name, Eqptr eqptr) {
    return equations.insert_or_assign(name, eqptr).first->second;
}

const Eqptr MDL_Compiler::find_eq(const std::string &name) {
    try {
        stod(name);
    } catch (invalid_argument &e) {
        return equations.at(name);
    }
    return add_eq(name, std::make_shared<Equation>(name));
}

void MDL_Compiler::pre_process(std::istream &is) {
    add_surface("default", std::make_shared<SurfaceGenerator>(
            find_eq("1"), find_eq("1"), find_eq("1"),
            find_eq("1"), find_eq("1"), find_eq("1"),
            find_eq("1"), find_eq("1"), find_eq("1"))
    );
    stack<WORLD::WORLD_PTR> worlds;
    worlds.push(base);
    string line;
    int line_no = 1;
    while (getline(is, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
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
            add_eq(name, std::make_shared<Equation>(eq));
        } else if (command == "ambient") {
            string r, g, b;
            ss >> r >> g >> b;
            ambient = make_shared<AmbientGenerator>(find_eq(r), find_eq(g), find_eq(b));
        } else if (command == "light") {
            string r, g, b, x, y, z;
            ss >> r >> g >> b >> x >> y >> z;
            lights.push_back(make_shared<LightGenerator>(find_eq(r), find_eq(g), find_eq(b),
                                                         find_eq(x), find_eq(y), find_eq(z)));
        } else if (command == "sphere") {
            string surface_name, x, y, z, radius;
            ss >> surface_name >> x >> y >> z >> radius;
            worlds.top()->commands.emplace_back(in_place_index<0>, make_shared<DRAW_SPHERE>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(radius)));
        } else if (command == "torus") {
            string surface_name, x, y, z, inner_r, outer_r;
            ss >> surface_name >> x >> y >> z >> inner_r >> outer_r;
            worlds.top()->commands.emplace_back(in_place_index<0>, make_shared<DRAW_TORUS>(
                    find_surface(surface_name), find_eq(x), find_eq(y), find_eq(z), find_eq(inner_r), find_eq(outer_r)
            ));
        } else if (command == "box") {
            string surface_name, x, y, z, h, w, d;
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
            RotationMatrix::Axis typed_axis =
                    axis == "x" ? RotationMatrix::X : axis == "y" ? RotationMatrix::Y : axis == "z" ? RotationMatrix::Z
                                                                                                    : (throw_error(
                                    "Invalid axis [" + axis + "]", line_no), RotationMatrix::Z);
            worlds.top()->commands.emplace_back(in_place_index<1>, make_shared<ROTATE>(typed_axis, find_eq(degrees)));
        } else if (command == "push") {
            double sf, ef;
            if (!(ss >> sf >> ef))
                throw_error("Could not parse double", line_no);
            auto world_ptr = make_shared<WORLD>(sf, ef);
            worlds.top()->commands.emplace_back(in_place_index<2>, world_ptr);
            worlds.push(world_ptr);
        } else if (command == "pop") {
            worlds.pop();
        } else if (command == "basename") {
            ss >> basename;
        } else if (command == "frames") {
            if (!(ss >> frames))
                throw_error("Could not parse unsigned int", line_no);
            base->end_frame = frames;
        }
        string test;
        if (ss >> test)
            throw_error("Extraneous characters for command [" + command + "]", line_no);
        line_no++;
    }
    if (frames == 1)
        static_image = true;
}

void MDL_Compiler::check_sys_call(int result) {
    if (result != 0)
        throw runtime_error(string("System call failed, strerror: ") + strerror(errno));
}

void MDL_Compiler::execute() {
    if (static_image) {
        Frame<500, 500> frame;
        base->exec_world(frame, 0, lights);
        frame.save(basename + ".png");
    } else {
        check_sys_call(system("mkdir -p build"));
        check_sys_call(chdir("./build"));
        for (int f = 0; f < frames; ++f) {
            cout << "Frame: " << f << endl;
            Frame<500, 500> frame;
            base->exec_world(frame, f, lights);
            stringstream fname;
            fname << setfill('0') << setw(3) << f;
            frame.save(basename + fname.str() + ".png");
        }
        check_sys_call(chdir(".."));
        check_sys_call(system(("convert -delay 1.7 build/" + basename + "* " + basename + ".gif").c_str()));
        check_sys_call(system("rm -rf ./build"));
    }
}

void MDL_Compiler::throw_error(const std::string &message, unsigned int line_no) {
    throw MDL_ParsingException(message + " on line " + to_string(line_no));
}

MDL_Compiler::MDL_ParsingException::MDL_ParsingException(const string &arg) :
        runtime_error("MDL_ParsingException: " + arg) {}
