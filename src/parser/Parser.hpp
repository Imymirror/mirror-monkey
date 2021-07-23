#ifndef PARSER_HPP
#define PARSER_HPP

#include "BlockStatement.hpp"
#include "ExpressionStatement.hpp"
#include "LetStatement.hpp"
#include "Lexer.hpp"
#include "Program.hpp"
#include "ReturnStatement.hpp"
#include "Token.hpp"

using namespace mirror;
using namespace std;
using namespace token;

namespace mirror {
enum class PRECEDENCE {
    LOWEST,
    EQUALS,      // ==
    LESSGREATER, // > or <
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // -X or !X
    CALL,        // myFunction(X)
    INDEX        // array[index]
};

class Parser {
  public:
    Parser(Lexer &l);
    Parser(string input);

  private:
    unique_ptr<Lexer> m_lexer;
    vector<string> m_errors;

    unique_ptr<Token> m_cur_token;
    unique_ptr<Token> m_peek_token;

    using fn_prefix = function<unique_ptr<Expression>()>;
    using fn_infix = function<unique_ptr<Expression>(unique_ptr<Expression>)>;

    map<TOKEN_TYPE, fn_prefix> m_prefix_fns;
    map<TOKEN_TYPE, fn_infix> m_infix_fns;

  private:
    void init();

  public:
    using t_map_precedence = map<TOKEN_TYPE, PRECEDENCE>;
    const t_map_precedence c_precedences = {
        {TOKEN_TYPE::EQ, PRECEDENCE::EQUALS},
        {TOKEN_TYPE::NOT_EQ, PRECEDENCE::EQUALS},
        {TOKEN_TYPE::LT, PRECEDENCE::LESSGREATER},
        {TOKEN_TYPE::GT, PRECEDENCE::LESSGREATER},
        {TOKEN_TYPE::PLUS, PRECEDENCE::SUM},
        {TOKEN_TYPE::MINUS, PRECEDENCE::SUM},
        {TOKEN_TYPE::SLASH, PRECEDENCE::PRODUCT},
        {TOKEN_TYPE::ASTERISK, PRECEDENCE::PRODUCT},
        {TOKEN_TYPE::LPAREN, PRECEDENCE::CALL},
        {TOKEN_TYPE::LBRACKET, PRECEDENCE::INDEX},
    };

  public:
    shared_ptr<Program> parse_program();
    vector<string> errors();

  private:
    void next_token();

    unique_ptr<Statement> parse_statement();
    unique_ptr<LetStatement> parse_let_statement();
    unique_ptr<ReturnStatement> parse_return_statement();
    unique_ptr<ExpressionStatement> parse_expression_statement();

    bool expect_peek(TOKEN_TYPE t);
    bool cur_token_is(TOKEN_TYPE t);
    bool peek_token_is(TOKEN_TYPE t);

    PRECEDENCE cur_precedence();
    PRECEDENCE peek_precedence();

    void peek_error(TOKEN_TYPE t);
    void no_prefix_parse_fn_error(TOKEN_TYPE t);

  private:
    void register_prefix(TOKEN_TYPE token_type, fn_prefix fn);
    void register_infix(TOKEN_TYPE token_type, fn_infix fn);

    unique_ptr<Expression> parse_expression(PRECEDENCE precedence);

    unique_ptr<Expression> parse_identifier();
    unique_ptr<Expression> parse_integer_literal();

    unique_ptr<Expression> parse_infix_expression(unique_ptr<Expression> exp);
    unique_ptr<Expression> parse_boolean();
    unique_ptr<Expression> parse_prefix_expression();
    unique_ptr<Expression> parse_grouped_expression();
    unique_ptr<Expression> parse_if_expression();
    unique_ptr<BlockStatement> parse_block_statement();
    unique_ptr<Expression> parse_function_literal();
    unique_ptr<vector<unique_ptr<Identifier>>> parse_function_parameters();
    unique_ptr<Expression> parse_call_expression(unique_ptr<Expression> exp);
    unique_ptr<vector<unique_ptr<Expression>>> parse_call_arguments();
    unique_ptr<vector<unique_ptr<Expression>>>
    parse_expression_list(TOKEN_TYPE end);
    unique_ptr<Expression> parse_array_literal();
    unique_ptr<Expression> parse_string_literal();
	unique_ptr<Expression> parse_index_expression(unique_ptr<Expression> left);
	unique_ptr<Expression> parse_hash_literal();
};
}
#endif /* PARSER_HPP */
