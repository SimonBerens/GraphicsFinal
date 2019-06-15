#include <utility>

#include <utility>

#include <utility>

#include <utility>
#include <string>
#include <stack>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <numeric>
#include "equation.h"

using namespace std;

Equation::Operation::Operation() : op() {
    params.reserve(5);
}

void Equation::Operation::param_num_check(int num_required, const std::string &op_attempted) {
    if (params.size() != num_required)
        throw_error(op_attempted + " undefined for more or less than " + to_string(num_required) + " parameters");
}

double Equation::Operation::eval() {
    if (op == "+") {
        return accumulate(params.begin(), params.end(), 0.);
    } else if (op == "-") {
        param_num_check(2, "subtraction");
        return params[0] - params[1];
    } else if (op == "*") {
        return accumulate(params.begin(), params.end(), 1., multiplies<>());
    } else if (op == "/") {
        param_num_check(2, "division");
        return params[0] / params[1];
    } else if (op == "^") {
        param_num_check(2, "exponentiation");
        return pow(params[0], params[1]);
    } else if (op == "sin") {
        param_num_check(1, "sin");
        return sin(params[0]);
    } else if (op == "cos") {
        param_num_check(1, "cos");
        return cos(params[0]);
    } else if (op == "tan") {
        param_num_check(1, "tan");
        return tan(params[0]);
    } else if (op == "arcsin") {
        param_num_check(1, "arcsin");
        return asin(params[0]);
    } else if (op == "arccos") {
        param_num_check(1, "arccos");
        return acos(params[0]);
    } else if (op == "arctan") {
        param_num_check(1, "arctan");
        return atan(params[0]);
    } else if (op == "abs") {
        param_num_check(1, "abs");
        return abs(params[0]);
    } else if (op == "?>") {
        param_num_check(4, "?>");
        return params[0] > params[1] ? params[2] : params[3];
    } else if (op == "?<") {
        param_num_check(4, "?<");
        return params[0] < params[1] ? params[2] : params[3];
    } else {
        Equation::Operation::throw_error("unknown operation [" + op + "]");
    }
    return 0;
}

void Equation::Operation::throw_error(const std::string &message) {
    throw OperationParsingException(message);
}

double Equation::eval(double x) {
    if (s == "x")
        return x;
    try {
        return stod(s);
    } catch (invalid_argument &e) {}
    stringstream ss(s);
    string token;
    int paren_balance = 0;
    while (ss >> token) {
        if (token == "(")
            paren_balance++;
        else if (token == ")")
            paren_balance--;
    }
    if (paren_balance != 0)
        throw_error("Mismatched parentheses");
    ss = stringstream(s);

    stack<Operation> ops;
    ops.emplace();
    try {
        while (ss >> token) {
            if (token == "(") {
                ops.emplace();
            } else if (token == ")") {
                double res = ops.top().eval();
                ops.pop();
                ops.top().params.push_back(res);
            } else if (token == "+" || token == "-" || token == "*" || token == "/" ||
                       token == "^" || token == "sin" || token == "cos" || token == "tan" ||
                       token == "arcsin" || token == "arccos" || token == "arctan" || token == "abs" ||
                       token == "?>" || token == "?<")
                ops.top().op = token;
            else if (token == "x")
                ops.top().params.push_back(x);
            else if (links.find(token) != links.end())
                ops.top().params.push_back(links.find(token)->second->eval(x));
            else if (token == "e")
                ops.top().params.push_back(M_E);
            else if (token == "pi")
                ops.top().params.push_back(M_PI);
            else {
                try {
                    ops.top().params.push_back(stod(token));
                } catch (invalid_argument &e) {
                    throw_error("Unknown token [" + token + "]");
                }
            }
        }
    } catch (Equation::Operation::OperationParsingException &e) {
        Equation::Operation::throw_error(string(e.what()) + " in equation [" + s + "]");
    }
    return ops.top().params[0];
}

void Equation::throw_error(const std::string &message) {
    throw EquationParsingException(message + " in equation [" + s + "]");
}

vector<string> Equation::find_linkables(const std::string &s) {
    stringstream ss(s);
    string token;
    vector<string> link_names;
    while (ss >> token)
        if (valid_name(token))
            link_names.push_back(token);
    return link_names;
}

Equation::Equation(string s, map<string, shared_ptr<Equation>> links) : s(std::move(s)), links(std::move(links)) {}

Equation::Equation(string s) : s(std::move(s)) {}

bool Equation::valid_name(const std::string &token) {
    return isalpha(token[0]) && all_of(token.begin(), token.end(), ::isalnum) &&
           token != "sin" && token != "cos" && token != "tan" &&
           token != "arcsin" && token != "arcsin" && token != "arctan" && token != "abs" &&
           token != "x" && token != "pi" && token != "e";
}

Equation::EquationParsingException::EquationParsingException(const string &message) :
        runtime_error("EquationParsingException: " + message) {}

Equation::Operation::OperationParsingException::OperationParsingException(const std::string &message) : runtime_error(
        "OperationParsingException: " + message) {

}
