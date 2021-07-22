#include "Program.hpp"
#include <iostream>

using namespace std;

string Program::token_literal() {
    if (m_statements.size() > 0) {
        return m_statements[0]->token_literal();
    } else {
        return "";
    }
}

string Program::to_string() {

    string ret = "";

    for (int i = 0; i < m_statements.size(); i++) {
        ret += m_statements[i]->to_string();
    }

    return ret;
}