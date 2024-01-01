#ifndef INDEXEXPRESSION_HPP
#define INDEXEXPRESSION_HPP

#include "Expression.hpp"
#include "Token.hpp"

using namespace std;
namespace mirror
{
  class IndexExpression : public Expression
  {
  public:
    unique_ptr<Token> m_token; // the '[' token
    unique_ptr<Expression> m_left;
    unique_ptr<Expression> m_index;

    IndexExpression(Token &token) : m_token(new Token(token)) {}
    IndexExpression(Token &token, unique_ptr<Expression> left) : m_token(new Token(token)), m_left(std::move(left)) {}

  public:
    string token_literal() override { return m_token->m_literal; };
    string to_string() override
    {
      string ret = "";

      ret += "(";
      ret += m_left->to_string();
      ret += "[";
      ret += m_index->to_string();
      ret += "])";

      return ret;
    };
  };
}
#endif /* INDEXEXPRESSION_HPP */
