#include "ExpressionStatement.hpp"

using namespace std;

string ExpressionStatement::to_string() {
    string ret = m_expression->to_string();
    return ret;
}