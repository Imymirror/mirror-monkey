#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <map>
#include <string>
#include <vector>
#include "Constant.hpp"

using namespace token;

using namespace std;

class Token {
  public:
    TOKEN_TYPE m_type;
    string m_literal;

  public:
    static map<string, TOKEN_TYPE> m_keywords;
    static TOKEN_TYPE lookup_ident(string ident);

  public:
    Token() = default;
    Token(Token &token) {
        m_type = token.m_type;
        m_literal = token.m_literal;
    };
    Token(TOKEN_TYPE token_type, string str);
    ~Token();

    friend ostream &operator<<(ostream &os, const Token &t);
    static string token_type_value(TOKEN_TYPE t);
};
#endif /* TOKEN_HPP */
