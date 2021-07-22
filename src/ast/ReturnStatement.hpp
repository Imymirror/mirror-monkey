#ifndef RETURNSTATEMENT_HPP
#define RETURNSTATEMENT_HPP

#include "Expression.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <memory>

using namespace std;

class ReturnStatement : public Statement {
  public:
    unique_ptr<Token> m_token; // the token.LET token
    unique_ptr<Expression> m_return_value;

  public:
    ReturnStatement(Token& token) : m_token(new Token(token)){};
	~ReturnStatement(){};
  public:
    string token_literal() override {return m_token->m_literal;};
    string to_string() override;
};

#endif /* RETURNSTATEMENT_HPP */
