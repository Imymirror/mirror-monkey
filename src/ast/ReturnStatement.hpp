#ifndef RETURNSTATEMENT_HPP
#define RETURNSTATEMENT_HPP

#include "Expression.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <memory>

using namespace std;

namespace mirror {
class ReturnStatement : public Statement {
  public:
    unique_ptr<Token> m_token; // the token.LET token
    unique_ptr<Expression> m_return_value;

  public:
    ReturnStatement(Token &token) : m_token(new Token(token)){};
    ~ReturnStatement(){};

  public:
    string token_literal() override { return m_token->m_literal; };
    string to_string() override {
        string ret = token_literal() + " " + m_return_value->to_string() + ";";
        return ret;
    };
};
}
#endif /* RETURNSTATEMENT_HPP */
