#include "Repl.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Environment.hpp"
#include "Evaluator.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <iostream>
#include <fstream>

using namespace std;
void print_parser_errors(vector<string> errors);
void print_anthor_name();
void main_process(const char *buf, Evaluator &e, shared_ptr<Environment> &env);

void Repl::start(const char *fileName)
{
    Evaluator e;
    auto env = make_shared<Environment>();

    if (fileName != nullptr)
    {
        ifstream ifs(fileName, std::ifstream::in);
        char *buf;
        if (ifs)
        {
            ifs.seekg(0, ifs.end);
            int length = ifs.tellg();
            ifs.seekg(0, ifs.beg);
            buf = new char[length];
            ifs.read(buf, length);
            if (!ifs)
            {
                cout << "read fail" << endl;
            }
            ifs.close();
            main_process(buf, e, env);
        }
        return;
    }
    print_anthor_name();
    const string PROMPT = ">> ";
    std::string line;
    while (1)
    {
        cout << PROMPT;
        getline(cin, line);
        if (line.find_first_of("-f") == 0)
        {
            start(line.substr(3).c_str());
            continue;
        }

        main_process(line.c_str(), e, env);
    }
}
void main_process(const char *buf, Evaluator &e, shared_ptr<Environment> &env)
{
    Lexer l();
    Parser p(buf);

    // for (auto tok = l.next_token(); (*tok).m_type != TOKEN_TYPE::EOF_;
    //      tok = l.next_token()) {
    //     cout << *tok << endl;
    // }

    auto program = p.parse_program();
    if (p.errors().size() != 0)
    {
        print_parser_errors(p.errors());
        return;
    }
    auto evaluated = e.eval(program.get(), env.get());
    if (evaluated)
    {
        cout << evaluated->Inspect() << endl;
    }
}

void print_anthor_name()
{
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

void print_parser_errors(vector<string> errors)
{
    print_anthor_name();
    cout << "Woops! We ran into some `mirror` business here!\n";
    cout << " parser errors:\n";
    for (int i = 0; i < errors.size(); i++)
    {
        cout << "\t" << errors[i] << endl;
    }
}