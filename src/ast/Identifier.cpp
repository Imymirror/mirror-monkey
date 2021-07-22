#include "Identifier.hpp"

using namespace std;


string Identifier::token_literal(){
	return m_token->m_literal;
}

string Identifier::to_string(){
	return m_value;	
}