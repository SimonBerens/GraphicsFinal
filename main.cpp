#include <iostream>
#include "parsing/compiler.h"
#include "draw.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 2)
        MDL_Compiler p(argv[1]);
    else
        cout << "Invalid number of arguments supplied, 1 required "
                "(./exec.zzz <filename>)" << endl;
    return 0;
}
