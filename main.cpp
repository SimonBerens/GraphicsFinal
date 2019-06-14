#include <iostream>
#include "parsing/symt.h"
#include "draw.h"

using namespace std;

int main() {
    ifstream file("rocket.mdl");
    Parser p;
    p.lex(file);
    p.parse();
    return 0;
}