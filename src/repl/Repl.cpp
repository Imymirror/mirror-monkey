#include "Repl.hpp"
#include "Environment.hpp"
#include "Evaluator.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include <algorithm>
#include <exception>
#include <iostream>

using namespace std;

void print_anthor_name();
void print_parser_errors(vector<string> errors);

void Repl::start() {

    print_anthor_name();

    const string PROMPT = ">> ";

    std::string line;

    Evaluator e;
    auto env = make_shared<Environment>();
    while (1) {
        cout << PROMPT;
        getline(cin, line);
        Lexer l(line);
        Parser p(l);

        // for (auto tok = l.next_token(); (*tok).m_type != TOKEN_TYPE::EOF_;
        //      tok = l.next_token()) {
        //     cout << *tok << endl;
        // }

        auto program = p.parse_program();
        if (p.errors().size() != 0) {
            print_parser_errors(p.errors());
            continue;
        }

        cout << program->to_string() << endl;

        auto evaluated = e.eval(program.get(), env.get());
        if (evaluated) {
            cout << evaluated->Inspect() << endl;
        }
    }
}

void print_anthor_name() {
    string name = R"(
	########################
	#╭━━╮╱╱╱╱╱╱╱╱╭╮        #
	#╰┃┃╋━━┳┳╮╭━━╋╋┳┳┳┳━┳┳╮#
	#╭┃┃┫┃┃┃┃┃┃┃┃┃┃╭┫╭┫╋┃╭╯#
	#╰━━┻┻┻╋╮┃╰┻┻┻┻╯╰╯╰━┻╯ #
	#╱╱╱╱╱╱╰━╯             #
	########################
)";

    cout << name << endl;
}

void print_parser_errors(vector<string> errors) {
    print_anthor_name();
    cout << "Woops! We ran into some `mirror` business here!\n";
    cout << " parser errors:\n";
    for (int i = 0; i < errors.size(); i++) {
        cout << "\t" << errors[i] << endl;
    }
}