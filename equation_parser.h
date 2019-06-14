#ifndef EQUATION_PARSER_H
#define EQUATION_PARSER_H

#include <vector>
#include <string>

class Eq {
public:
    double eval(double x);

    explicit Eq(std::string s);

private:
    struct Operation {

        Operation();

        std::string op;

        std::vector<double> params;

        void param_num_check(int num_required, const std::string& op_attempted);

        double eval();
    };

    std::string s;
};

#endif //EQUATION_PARSER_H
