#ifndef EQUATION_H
#define EQUATION_H

#include <memory>
#include <vector>
#include <map>
#include <string>

class Equation {
public:
    double eval(double x);

    Equation(std::string s);

    Equation(std::string s, std::map<std::string, std::shared_ptr<Equation>> links);

    static std::vector<std::string> find_linkables(const std::string &s);

    class EquationParsingException : public std::runtime_error {
    public:
        explicit EquationParsingException(const std::string &message);
    };

    void throw_error(const std::string &message);

private:
    struct Operation {

        Operation();

        std::string op;

        std::vector<double> params;

        void param_num_check(int num_required, const std::string &op_attempted);

        double eval();

        class OperationParsingException : public std::runtime_error {
        public:
            explicit OperationParsingException(const std::string &message);
        };

        static void throw_error(const std::string &message);
    };

    std::string s;

    std::map<std::string, std::shared_ptr<Equation>> links;
};

typedef std::shared_ptr<Equation> Eqptr;

#endif //EQUATION_H
