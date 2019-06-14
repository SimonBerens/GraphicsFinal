#include <iostream>
#include "parsing/symt.h"
#include "draw.h"

using namespace std;

int main() {
  //cout << "hi";
    ifstream file("simple_anim.mdl");
    Parser p;
    //cout << "before lex";
    p.lex(file);
    //cout << "after lex";
    p.parse();
    //    cout << "after parse";
    return 0;
}
