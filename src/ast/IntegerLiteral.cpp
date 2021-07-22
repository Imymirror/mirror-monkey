#include "IntegerLiteral.hpp"
#include <string>

using namespace std;

string IntegerLiteral::token_literal(){
	return m_token->m_literal;
}

string IntegerLiteral::to_string(){
	return std::to_string(m_value) ;	
}