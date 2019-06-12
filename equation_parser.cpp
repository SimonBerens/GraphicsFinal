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

Eq::Operation::Operation()  : op(0) {
    params.reserve(5);
}

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
                throw runtime_error("/ undefined for more or less than 2 parameters");
            return params[0] / params[1];
        case '^':
            if (params.size() != 2)
                throw runtime_error("^ undefined for more or less than 2 parameters");
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
    ops.emplace(); // temp to store value
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
            ops.top().params.push_back(x - offset);
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

Eq::Eq(string s,double offset) : s(std::move(s)),offset(offset) {}
