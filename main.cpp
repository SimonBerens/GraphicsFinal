#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    basic_regex<char> white_space("[ \\t\\n ]+");
    basic_regex<char> dbl("\\-?[0-9]+ |\n"
                          "\\-?[0-9]+\\. |\n"
                          "\\-?[0-9]+\\.[0-9]+ |\n"
                          "\\-?\\.[0-9]+");
    string s("7");
    smatch m;
    regex_match(s, m, dbl);
    cout << static_cast<bool>(m.empty()) << endl;
    return 0;
}