#ifndef EQUATION_H
#define EQUATION_H

#include <vector>
#include <string>

class Equation {
public:
    double eval(double x);

    explicit Equation(std::string s);

    class EquationParsingException : public std::runtime_error {
    public:
        explicit EquationParsingException(const std::string &message);
    };

    static void throw_error(const std::string &message);
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

#endif //EQUATION_H
