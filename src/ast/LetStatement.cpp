#include "LetStatement.hpp"
#include <iostream>

using namespace std;


string LetStatement::token_literal(){
	return m_token->m_literal;
}

string LetStatement::to_string(){
	string ret = token_literal() + " " + m_name->to_string() + " = " + m_value->to_string() + ";";
	return ret;
}