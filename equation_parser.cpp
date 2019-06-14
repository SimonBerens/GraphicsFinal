#include <utility>

#include <string>
#include <stack>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <numeric>
#include "equation_parser.h"


// todo rename files
using namespace std;

Eq::Operation::Operation() : op() {
    params.reserve(5);
}

void Eq::Operation::param_num_check(int num_required, const std::string &op_attempted) {
    if (params.size() != num_required)
        throw runtime_error(
                op_attempted + " undefined for more or less than " + to_string(num_required) + " parameters");
}

double Eq::Operation::eval() {
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
    } else {
        throw runtime_error(string("unknown operation [") + op + "]");
    }
}

//evaluates a string

double Eq::eval(double x) {
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
        throw runtime_error("Mismatched parentheses in equation [" + s + "]");
    ss = stringstream(s);

    stack<Operation> ops;
    ops.emplace(); // temp to store value
    while (ss >> token) {
        if (token == "(") {
            ops.emplace();
            paren_balance++;
        } else if (token == ")") {
            double res = ops.top().eval();
            ops.pop();
            ops.top().params.push_back(res);
            paren_balance--;
        } else if (token == "+" || token == "-" || token == "*" || token == "/" ||
                   token == "^" || token == "sin" || token == "cos" || token == "tan")
            ops.top().op = token;
        else if (token == "x")
            ops.top().params.push_back(x);
        else if (token == "e")
            ops.top().params.push_back(M_E);
        else if (token == "pi")
            ops.top().params.push_back(M_PI);
        else {
            try {
                ops.top().params.push_back(stod(token));
            } catch (invalid_argument &e) {
                throw runtime_error("Unknown token [" + token + "] in equation [" + s + "]");
            }
        }
    }
    return ops.top().params[0];
}

Eq::Eq(string s) : s(std::move(s)) {}
