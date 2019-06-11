#include <string>
#include <iostream>
#include <sstream>

using namespace std;

string replace_str(const string &original, const string &target, const string &replace) {
    string copy_original = original;
    while (copy_original.find(target) != -1) {
        unsigned long replace_location = copy_original.find(target);
        unsigned long target_len = target.size();
        copy_original = copy_original.substr(0, replace_location) + replace +
               copy_original.substr(replace_location + target_len, copy_original.size());
    }
    return copy_original;
}

string operations[] = {"+", "-", "*", "/", "^"};

double eval(const string &equation) {
    unsigned long end_loc = equation.find(")");
    unsigned long start_loc = equation.rfind("(");
    string eq_subsection = equation.substr(start_loc, end_loc + 1);
    int op_loc = -1;
    int ctr = 0;
    while (op_loc == -1 && ctr < operations->size()){
        op_loc = (int)eq_subsection.find(operations[ctr]);
        ctr++;
    }
    string somewhat = eq_subsection.substr(0,op_loc) + eq_subsection.substr(op_loc + 1, eq_subsection.size());
    string param_array[] = {somewhat.substr(0,somewhat.find(" ")),somewhat.substr(somewhat.rfind(" "),somewhat.size())};
    stringstream param1(param_array[0]);
    stringstream param2(param_array[1]);
    double p1,p2;
    param1 >> p1;
    param2 >> p2;
    cout <<p1;
    cout << p2;
    double val;
    switch (eq_subsection[op_loc]){
        case '+':
            val = p1 + p2;
            break;
        case '-':
            val = p1 - p2;
            break;
        case '*':
            val = p1 * p2;
            break;
        case '/':
            val = p1 / p2;
            break;
//        case '^':
//            val = p1 ** p2;
//            break;
        default:
            cout << "FDFSDFDSFDSFDSFS";
    }
    if (eq_subsection == equation){
        return val;
    }
    return eval(equation.substr(0,start_loc) + to_string(val) + equation.substr(end_loc + 1,equation.size()));
}

double eval_sub(const string &equation, double x) {
    return eval(replace_str(equation, "x", to_string(x)));
}



int main() {
//    cout << (replace_str("helloxtestxx", "x", "MEN"));
    cout << (eval_sub("(3 + 2)",3));
}
