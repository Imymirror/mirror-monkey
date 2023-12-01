#ifndef CALLEXPRESSION_HPP
#define CALLEXPRESSION_HPP

#include "Expression.hpp"
#include "Token.hpp"

using namespace std;
namespace mirror
{
  class CallExpression : public Expression
  {
  public:
    unique_ptr<Token> m_token;         // The '(' token
    unique_ptr<Expression> m_function; // Identifier or FunctionLiteral
    unique_ptr<vector<unique_ptr<Expression>>> m_arguments;

  public:
    CallExpression(Token &token, unique_ptr<Expression> &&exp)
        : m_token(new Token(token)), m_function(std::move(exp)){};

  public:
    string token_literal() override { return m_token->m_literal; };
    string to_string() override
    {
      string ret = "";

      ret += m_function->to_string();
      ret += "(";

      auto &arguments = *m_arguments;
      for (int i = 0; i < arguments.size(); i++)
      {
        ret += arguments[i]->to_string();
        if (i < arguments.size() - 1)
          ret += ", ";
      }

      ret += ")";

      return ret;
    };
  };
}
#endif /* CALLEXPRESSION_HPP */
