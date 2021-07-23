#include "../object/Boolean.hpp"
#include "HashKey.hpp"
#include "Repl.hpp"
#include "String.hpp"
#include <any>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <ratio>
#include <string>
#include <vector>

using namespace mirror;
using namespace std;

int main(int argc, char *argv[]) {
    cout << "Hello! This is the Mirror-Monkey programming language!" << endl;
    cout << "Feel free to type in commands" << string(2, '\n');

    Repl repl;
    repl.start();
}
