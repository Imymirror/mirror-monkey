#ifndef IFEXPRESSION_HPP
#define IFEXPRESSION_HPP

#include "BlockStatement.hpp"
#include "Expression.hpp"
#include "Token.hpp"

using namespace std;

class IfExpression : public Expression {
  public:
    unique_ptr<Token> m_token; // The 'if' token
    unique_ptr<Expression> m_condition;
    unique_ptr<BlockStatement> m_consequence;
    unique_ptr<BlockStatement> m_alternative;

  public:
    IfExpression(Token &token) : m_token(new Token(token)) {}

  public:
    string token_literal() { return m_token->m_literal; }
    string to_string() {
        string ret = "";

        ret += "if";

        ret += m_condition->to_string();

        ret += " ";

        ret += m_consequence->to_string();

        if (m_alternative != nullptr) {
            ret += "else ";
            ret += m_alternative->to_string();
        }

        return ret;
    }
};

#endif /* IFEXPRESSION_HPP */
