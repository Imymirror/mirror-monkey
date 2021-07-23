#ifndef BLOCKSTATEMENT_HPP
#define BLOCKSTATEMENT_HPP

#include "LetStatement.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <memory>
#include <vector>

using namespace std;
namespace mirror {
class BlockStatement : public Statement {
  public:
    std::unique_ptr<Token> m_token;
    vector<unique_ptr<Statement>> m_statements;

  public:
    BlockStatement(Token &token) : m_token(new Token(token)) {}
    BlockStatement(BlockStatement &other) {
        m_token = make_unique<Token>(*other.m_token);
        for (int i = 0; i < other.m_statements.size(); i++) {
            auto stmt = other.m_statements[i].get();

            if (auto cast_node = dynamic_cast<LetStatement *>(stmt)) {
                m_statements[i] = make_unique<LetStatement>(*cast_node);
            }
        }
    };

  public:
    string token_literal() override { return m_token->m_literal; };
    string to_string() override {
        string ret = "";
        for (int i = 0; i < m_statements.size(); i++) {
            ret += m_statements[i]->to_string();
        }

        return ret;
    };
};
}
#endif /* BLOCKSTATEMENT_HPP */
