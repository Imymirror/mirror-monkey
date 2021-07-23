#ifndef LETSTATEMENT_HPP
#define LETSTATEMENT_HPP

#include "Expression.hpp"
#include "Identifier.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <memory>

using namespace std;

namespace mirror {
class LetStatement : public Statement {
  public:
    unique_ptr<Token> m_token; // the token.LET token
    unique_ptr<Identifier> m_name;
    unique_ptr<Expression> m_value;

  public:
    LetStatement() = default;
    LetStatement(LetStatement &other) {
        m_token = make_unique<Token>(*other.m_token);
        m_name = make_unique<Identifier>(*other.m_name);
        m_value = make_unique<Expression>(*other.m_value);
    };
    LetStatement(Token &token, unique_ptr<Identifier> name,
                 unique_ptr<Expression> value)
        : m_token(new Token(token)), m_name(std::move(name)),
          m_value(std::move(value)){};
    LetStatement(Token &token) : m_token(new Token(token)){};
    ~LetStatement(){};

  public:
    string token_literal() { return m_token->m_literal; };
    string to_string() {
        string ret = token_literal() + " " + m_name->to_string() + " = " +
                     m_value->to_string() + ";";
        return ret;
    };
};
}
#endif /* LETSTATEMENT_HPP */
