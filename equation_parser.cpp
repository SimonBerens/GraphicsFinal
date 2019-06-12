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

using namespace std;

double Eq::Operation::eval() {
    switch (op) {
        case '+':
            return accumulate(params.begin(), params.end(), 0.);
        case '-':
            return -accumulate(params.begin(), params.end(), 0.);
        case '*':
            return accumulate(params.begin(), params.end(), 1., multiplies<>());
        case '/':
            if (params.size() != 2)
                throw "/ undefined for more or less than 2 parameters";
            return params[0] / params[1];
        case '^':
            if (params.size() != 2)
                throw "^ undefined for more or less than 2 parameters";
            return pow(params[0], params[1]);

    }
    return 0;
}

double Eq::eval(double x) {
    if (s == "x")
        return x;
    try {
        return stod(s);
    } catch (invalid_argument &e) {}
    stringstream ss(s);
    stack<Operation> ops;
    string token;
    while (ss >> token) {
        if (token == "(")
            ops.emplace();
        else if (token == ")") {
            double res = ops.top().eval();
            ops.pop();
            ops.top().params.push_back(res);
        } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^")
            ops.top().op = token[0];
        else if (token == "x")
            ops.top().params.push_back(x);
        try {
            ops.top().params.push_back(stod(token));
        } catch (invalid_argument &e) {
            throw "Unknown token in equation [" + s + "]";
        }
    }

}

Eq::Eq(string s) : s(std::move(s)) {}
