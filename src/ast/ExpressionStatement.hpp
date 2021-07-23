#ifndef EXPRESSIONSTATEMENT_HPP
#define EXPRESSIONSTATEMENT_HPP

#include "Expression.hpp"
#include "Identifier.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <memory>

using namespace std;
namespace mirror {
class ExpressionStatement : public Statement {
  public:
    unique_ptr<Token> m_token; // the token.LET token
    unique_ptr<Expression> m_expression;

  public:
    ExpressionStatement(Token &token) : m_token(new Token(token)){};

  public:
    string token_literal() { return m_token->m_literal; };
    string to_string() {
        string ret = m_expression->to_string();
        return ret;
    };
};
} // namespace mirror
#endif /* EXPRESSIONSTATEMENT_HPP */
