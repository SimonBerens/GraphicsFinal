#ifndef EQUATION_PARSER_H
#define EQUATION_PARSER_H

#include <vector>
#include <string>

class Eq {
public:
    double eval(double x);

    Eq(std::string s, double offset);

private:
    struct Operation {

        Operation();

        char op;

        std::vector<double> params;

        double eval();
    };

    double offset;
    std::string s;
};

#endif //EQUATION_PARSER_H
