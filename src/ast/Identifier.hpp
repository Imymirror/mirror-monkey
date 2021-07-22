#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>
#include <string>

using namespace std;

// Expressions
class Identifier : public Expression {
  public:
    unique_ptr<Token> m_token; // the token.IDENT token
    string m_value;

    string token_literal();
    string to_string();

  public:
    Identifier() = default;
    Identifier(Identifier &ident):m_token(make_unique<Token>(*ident.m_token)),m_value(ident.m_value) {
   };
    Identifier(Token &token, string value)
        : m_token(new Token(token)), m_value(value){};
};

#endif /* IDENTIFIER_HPP */
