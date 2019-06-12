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

        std::string op;

        std::vector<double> params;

        void param_num_check(int num_required, const std::string& op_attempted);

        double eval();
    };

    double offset;
    std::string s;
};

#endif //EQUATION_PARSER_H
