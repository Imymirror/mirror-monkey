#ifndef HASHLITERAL_HPP
#define HASHLITERAL_HPP

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>

using namespace std;
namespace mirror {
class HashLiteral : public Expression {
  public:
    unique_ptr<Token> m_token;
    unique_ptr<map<shared_ptr<Expression>, shared_ptr<Expression>>> m_pairs;

    HashLiteral(Token &token) : m_token(new Token(token)){};

  public:
    string token_literal() override { return m_token->m_literal; };
    string to_string() override {
        string ret = "";
        ret += "{";

        map<shared_ptr<Expression>, shared_ptr<Expression>>::iterator cursor;

        auto &pairs = *m_pairs;

        auto size = pairs.size();
        auto count = 0;
        for (cursor = pairs.begin(); cursor != pairs.end(); cursor++) {
            ret +=
                cursor->first->to_string() + ":" + cursor->second->to_string();

            if (++count != size) {
                ret += ", ";
            }
        }

        ret += "}";
        return ret;
    };
};
}
#endif /* HASHLITERAL_HPP */
