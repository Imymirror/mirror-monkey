#ifndef INFIXEXPRESSION_HPP
#define INFIXEXPRESSION_HPP

#include "Expression.hpp"
#include "Token.hpp"

using namespace std;

class InfixExpression : public Expression {
  public:
    unique_ptr<Token> m_token; // // The operator token, e.g. +
    unique_ptr<Expression> m_left;
    string m_operator;
    unique_ptr<Expression> m_right;

  public:
    InfixExpression(Token &token, string op, unique_ptr<Expression> l)
        : m_token(new Token(token)), m_operator(op), m_left(std::move(l)){};

  public:
    string token_literal() override { return m_token->m_literal; };
    string to_string() override;
};

#endif /* INFIXEXPRESSION_HPP */
