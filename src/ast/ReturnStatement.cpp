#include "ReturnStatement.hpp"

using namespace std;

string ReturnStatement::to_string() {
    string ret = token_literal() + " " + m_return_value->to_string() + ";";
    return ret;
}