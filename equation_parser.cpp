#include <string>;
using namespace std;

string replace_str(const string & original, const string & target, const string & replace){
  int replace_location = original.find(target)
    int target_len = target.size()
    return original.substr(0,replace_location) + replace + original.substr(replace_location + target_len,original.size())
    }

double eval_sub(const string & equation, double x){
  return eval(replace_str(equation, "x",to_string(x)))
    }

string[] operations = ["+","-","*","/","^"]

double eval(const string & equation){
  int start_loc = equation.find(")")
    int end_loc= equation.rfind("(")
    string eq_subsection = equation.substr(start_loc,end_loc+1)
    
}
