#ifndef EQUATION_PARSER_H
#define EQUATION_PARSER_H

#include <vector>
#include <string>

class Eq {
public:
    double eval(double x);

    Eq(std::string s);

    struct Operation {

        Operation() : op(0) {
            params.reserve(5);
        };

        char op;

        std::vector<double> params;

        double eval();
    };

    std::string s;
};

#endif //EQUATION_PARSER_H
