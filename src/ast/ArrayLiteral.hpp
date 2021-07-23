#ifndef ARRAYLITERAL_HPP
#define ARRAYLITERAL_HPP

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>

using namespace std;
namespace mirror {
class ArrayLiteral : public Expression {
  public:
    unique_ptr<Token> m_token; // the '[' token
    unique_ptr<vector<unique_ptr<Expression>>> m_elements;

    ArrayLiteral(Token &token) : m_token(new Token(token)){};

  public:
    string token_literal() override { return m_token->m_literal; };
    string to_string() override {
        string ret = "";

        ret += "[";

        auto &elements = *m_elements;
        for (int i = 0; i < elements.size(); i++) {
            ret += (*elements[i]).to_string();
            if (i != elements.size() - 1) {
                ret += ", ";
            }
        }
        ret += "]";

        return ret;
    };
};
}
#endif /* ARRAYLITERAL_HPP */
