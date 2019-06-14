#ifndef SYMT_H
#define SYMT_H

#include <vector>
#include <string>
#include <map>
#include "cmd.h"
#include "sym.h"


class Parser { // todo rename
public:

    Parser();

    void add_command(Command &&command);

    Sgptr add_surface(const std::string &name, Sgptr sgprt);

    const Sgptr find_surface(const std::string &name) const;

    Eqptr add_eq(const std::string &name, Eqptr eqptr);

    const Eqptr find_eq(const std::string &name);

    void lex(std::istream &is);

    void parse();

private:

    // todo states eg frames basename
    std::string basename;
    int frames;
    bool static_image;
    std::vector<Command> commands;
    std::map<std::string, Eqptr> equations;
    std::map<std::string, Sgptr> surfaces;
};


#endif //SYMT_H
