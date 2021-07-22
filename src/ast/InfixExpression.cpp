#include "InfixExpression.hpp"

using namespace std;

string InfixExpression::to_string() {
	string ret = "(" + m_left->to_string() + " " + m_operator + " " + m_right->to_string() + ")";

	return ret;
}