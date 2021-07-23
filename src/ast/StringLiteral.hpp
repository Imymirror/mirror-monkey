#ifndef STRINGLITERAL_HPP
#define STRINGLITERAL_HPP

#include "Expression.hpp"
#include "Token.hpp"

using namespace std;
namespace mirror {
class StringLiteral : public Expression {
  public:
    unique_ptr<Token> m_token; // the token.IDENT token
    string m_value;

    StringLiteral(Token &token, string value)
        : m_token(new Token(token)), m_value(value){};

  public:
    string token_literal() { return m_token->m_literal; };
    string to_string() { return m_token->m_literal; };
};
}
#endif /* STRINGLITERAL_HPP */
