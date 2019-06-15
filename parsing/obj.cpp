#include <fstream>
#include <sstream>
#include <memory>
#include "obj.h"

using namespace std;

shared_ptr<FaceList> parse_obj_file(const string &filename) {
    auto throw_error = [&filename](const string &message, unsigned int line_no) {
        throw OBJ_FileParsingException(message + " in file [" + filename + "] on line " + to_string(line_no));
    };
    ifstream file(filename);
    if (file.fail())
        throw_error("File does not exist", 0);
    FaceList fl;
    string line;
    vector<Point> vertices;
    unsigned int line_no = 1;
    vertices.reserve(1024);
    while (getline(file, line)) {
        stringstream ss(line);
        string type;
        ss >> type;
        if (type == "v") {
            double x, y, z;
            if (!(ss >> x >> y >> z))
                throw_error("Error parsing vertex", line_no);
            vertices.emplace_back(x, y, z);
        } else if (type == "f") {
            unsigned int vref;
            vector<unsigned int> vrefs;
            while (ss >> vref)
                vrefs.push_back(vref);
            if (vrefs.size() <= 2)
                throw_error("Error parsing face", line_no);
            for (int i = 1; i < vrefs.size() - 1; ++i)
                fl.add_face(vertices[vrefs[0] - 1], vertices[vrefs[i] - 1], vertices[vrefs[i + 1] - 1]);
        }
        line_no++;
    }
    return make_shared<FaceList>(move(fl));
}

OBJ_FileParsingException::OBJ_FileParsingException(const string &message) :
        runtime_error("OBJ_FileParsingException: " + message) {}

