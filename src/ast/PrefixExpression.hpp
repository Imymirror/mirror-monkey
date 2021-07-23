#ifndef PREFIXEXPRESSION_HPP
#define PREFIXEXPRESSION_HPP

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>

using namespace std;
namespace mirror {
class PrefixExpression : public Expression {
  public:
    unique_ptr<Token> m_token; // The operator token, e.g. !, -
    string m_operator;
    unique_ptr<Expression> m_right;

  public:
    PrefixExpression(Token &token, string op)
        : m_token(new Token(token)), m_operator(op){};

  public:
    string token_literal() { return m_token->m_literal; };
    string to_string() {
        return "(" + m_operator  + m_right->to_string() + ")";
    };
};
}
#endif /* PREFIXEXPRESSION_HPP */
