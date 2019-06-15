#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include <map>
#include "command.h"
#include "light.h"


class MDL_Compiler {
public:

    explicit MDL_Compiler(const std::string &filename);

private:
    Sgptr add_surface(const std::string &name, Sgptr sgprt);

    const Sgptr find_surface(const std::string &name) const;

    Eqptr add_eq(const std::string &name, Eqptr eqptr);

    const Eqptr find_eq(const std::string &name);

    void pre_process(std::istream &is);

    static void check_sys_call(int result);

    void execute();

    class MDL_ParsingException : public std::runtime_error {
    public:
        explicit MDL_ParsingException(const std::string &arg);
    };

    static void throw_error(const std::string &message, unsigned int line_no);

    WORLD::WORLD_PTR base;
    std::string basename;
    unsigned int frames;
    bool static_image;
    std::map<std::string, Eqptr> equations;
    std::map<std::string, Sgptr> surfaces;
    Agptr ambient;
    std::vector<Lgptr> lights;

};


#endif //COMPILER_H
