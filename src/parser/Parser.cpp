#include "Parser.hpp"
#include "ArrayLiteral.hpp"
#include "Boolean.hpp"
#include "CallExpression.hpp"

#include "Common.hpp"
#include "Constant.hpp"
#include "Expression.hpp"
#include "FunctionLiteral.hpp"
#include "HashLiteral.hpp"
#include "IfExpression.hpp"
#include "IndexExpression.hpp"
#include "InfixExpression.hpp"
#include "IntegerLiteral.hpp"
#include "Lexer.hpp"
#include "PrefixExpression.hpp"
#include "StringLiteral.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>

using namespace token;
using namespace std;
using namespace std::placeholders;
using namespace ast;

void Parser::init()
{
    register_prefix(TOKEN_TYPE::IDENT,
                    std::bind(&Parser::parse_identifier, this));
    register_prefix(TOKEN_TYPE::INT,
                    std::bind(&Parser::parse_integer_literal, this));
    register_prefix(TOKEN_TYPE::STRING,
                    std::bind(&Parser::parse_string_literal, this));
    register_prefix(TOKEN_TYPE::BANG,
                    std::bind(&Parser::parse_prefix_expression, this));
    register_prefix(TOKEN_TYPE::MINUS,
                    std::bind(&Parser::parse_prefix_expression, this));
    register_prefix(TOKEN_TYPE::TRUE_,
                    std::bind(&Parser::parse_boolean, this));
    register_prefix(TOKEN_TYPE::FALSE_,
                    std::bind(&Parser::parse_boolean, this));
    register_prefix(TOKEN_TYPE::LPAREN,
                    std::bind(&Parser::parse_grouped_expression, this));
    register_prefix(TOKEN_TYPE::IF,
                    std::bind(&Parser::parse_if_expression, this));
    register_prefix(TOKEN_TYPE::FUNCTION,
                    std::bind(&Parser::parse_function_literal, this));
    register_prefix(TOKEN_TYPE::LBRACKET,
                    std::bind(&Parser::parse_array_literal, this));
    register_prefix(TOKEN_TYPE::LBRACE,
                    std::bind(&Parser::parse_hash_literal, this));

    register_infix(TOKEN_TYPE::PLUS,
                   std::bind(&Parser::parse_infix_expression, this, _1));
    register_infix(TOKEN_TYPE::MINUS,
                   std::bind(&Parser::parse_infix_expression, this, _1));
    register_infix(TOKEN_TYPE::SLASH,
                   std::bind(&Parser::parse_infix_expression, this, _1));
    register_infix(TOKEN_TYPE::ASTERISK,
                   std::bind(&Parser::parse_infix_expression, this, _1));
    register_infix(TOKEN_TYPE::EQ,
                   std::bind(&Parser::parse_infix_expression, this, _1));
    register_infix(TOKEN_TYPE::NOT_EQ,
                   std::bind(&Parser::parse_infix_expression, this, _1));
    register_infix(TOKEN_TYPE::LT,
                   std::bind(&Parser::parse_infix_expression, this, _1));
    register_infix(TOKEN_TYPE::GT,
                   std::bind(&Parser::parse_infix_expression, this, _1));

    register_infix(TOKEN_TYPE::LPAREN,
                   std::bind(&Parser::parse_call_expression, this, _1));

    register_infix(TOKEN_TYPE::LBRACKET,
                   std::bind(&Parser::parse_index_expression, this, _1));

    next_token();
    next_token();
}

Parser::Parser(string input) : m_lexer(make_unique<Lexer>(input)) { init(); }

Parser::Parser(Lexer &l) : m_lexer(make_unique<Lexer>(l)) { init(); }

void Parser::next_token()
{
    m_cur_token = std::move(m_peek_token);
    m_peek_token = std::move(m_lexer->next_token());
}

shared_ptr<Program> Parser::parse_program()
{
    auto program = make_shared<Program>();

    while (!cur_token_is(TOKEN_TYPE::EOF_))
    {
        auto stmt = parse_statement();
        if (stmt)
        {
            program->m_statements.push_back(std::move(stmt));
        }
        next_token();
    }

    return program;
}

unique_ptr<Statement> Parser::parse_statement()
{
    switch (m_cur_token->m_type)
    {
    case TOKEN_TYPE::LET:
        return parse_let_statement();
        break;
    case TOKEN_TYPE::RETURN:
        return parse_return_statement();
        break;
    default:
        return parse_expression_statement();
        break;
    }
    return nullptr;
}

unique_ptr<ExpressionStatement> Parser::parse_expression_statement()
{
    auto stmt = make_unique<ExpressionStatement>(*m_cur_token);

    auto exp = parse_expression(PRECEDENCE::LOWEST);
    if (!exp)
    {
        return nullptr;
    }

    stmt->m_expression = std::move(exp);

    if (peek_token_is(TOKEN_TYPE::SEMICOLON))
    {
        next_token();
    }

    return stmt;
}

unique_ptr<ReturnStatement> Parser::parse_return_statement()
{
    auto stmt = make_unique<ReturnStatement>(*m_cur_token);

    next_token();

    auto exp = parse_expression(PRECEDENCE::LOWEST);
    if (!exp)
    {
        return nullptr;
    }

    stmt->m_return_value = std::move(exp);

    if (peek_token_is(TOKEN_TYPE::SEMICOLON))
        next_token();

    return stmt;
}

unique_ptr<LetStatement> Parser::parse_let_statement()
{
    auto stmt = make_unique<LetStatement>(*m_cur_token);
    if (!expect_peek(TOKEN_TYPE::IDENT))
    {
        return nullptr;
    }

    stmt->m_name =
        make_unique<Identifier>(*m_cur_token, m_cur_token->m_literal);

    if (!expect_peek(TOKEN_TYPE::ASSIGN))
    {
        return nullptr;
    }

    next_token();

    auto exp = parse_expression(PRECEDENCE::LOWEST);
    if (!exp)
    {
        return nullptr;
    }
    cout << "let statement:" << exp->to_string() << endl;
    stmt->m_value = std::move(exp);

    if (peek_token_is(TOKEN_TYPE::SEMICOLON))
        next_token();

    return stmt;
}

bool Parser::cur_token_is(TOKEN_TYPE t) { return m_cur_token->m_type == t; }
bool Parser::peek_token_is(TOKEN_TYPE t) { return m_peek_token->m_type == t; }
bool Parser::expect_peek(TOKEN_TYPE t)
{
    if (peek_token_is(t))
    {
        next_token();
        return true;
    }
    else
    {
        peek_error(t);
        return false;
    }
}

PRECEDENCE Parser::cur_precedence()
{
    auto it = c_precedences.find(m_cur_token->m_type);
    if (it != c_precedences.end())
    {
        return it->second;
    }

    return PRECEDENCE::LOWEST;
}

PRECEDENCE Parser::peek_precedence()
{
    auto it = c_precedences.find(m_peek_token->m_type);
    if (it != c_precedences.end())
    {
        return it->second;
    }

    return PRECEDENCE::LOWEST;
}

void Parser::register_prefix(TOKEN_TYPE token_type, fn_prefix fn)
{
    m_prefix_fns[token_type] = fn;
}

void Parser::register_infix(TOKEN_TYPE token_type, fn_infix fn)
{
    m_infix_fns[token_type] = fn;
}

unique_ptr<Expression> Parser::parse_identifier()
{
    return make_unique<Identifier>(*m_cur_token, m_cur_token->m_literal);
}

unique_ptr<Expression> Parser::parse_integer_literal()
{
    auto lit = make_unique<IntegerLiteral>(*m_cur_token);

    lit->m_value =
        static_cast<int64_t>(stoll(m_cur_token->m_literal, nullptr, 10));

    return lit;
}

vector<string> Parser::errors() { return m_errors; }

void Parser::peek_error(TOKEN_TYPE t)
{
    string msg = "expected next token to be " + Token::token_type_value(t) +
                 ", got " + Token::token_type_value(m_peek_token->m_type) +
                 " instead";
    m_errors.push_back(msg);
}

void Parser::no_prefix_parse_fn_error(TOKEN_TYPE t)
{
    string msg =
        "no prefix parse function for " + Token::token_type_value(t) + " found";

    m_errors.push_back(msg);
}
unique_ptr<Expression> Parser::parse_expression(PRECEDENCE precedence)
{
    auto it = m_prefix_fns.find(m_cur_token->m_type);
    if (it == m_prefix_fns.end())
    {
        no_prefix_parse_fn_error(m_cur_token->m_type);
        return nullptr;
    }

    auto left_exp = it->second();

    while (!peek_token_is(TOKEN_TYPE::SEMICOLON) &&
           precedence < peek_precedence())
    {
        auto it = m_infix_fns.find(m_peek_token->m_type);
        if (it == m_infix_fns.end())
        {
            return left_exp;
        }

        next_token();
        left_exp = it->second(std::move(left_exp)); // parse_infix_expression
    }

    return left_exp;
}

unique_ptr<Expression>
Parser::parse_infix_expression(unique_ptr<Expression> left)
{
    auto expression = make_unique<InfixExpression>(
        *m_cur_token, m_cur_token->m_literal, std::move(left));

    auto precedence = cur_precedence();
    next_token();

    expression->m_right = parse_expression(precedence);

    return expression;
}

unique_ptr<Expression> Parser::parse_boolean()
{
    return make_unique<Boolean>(*m_cur_token, cur_token_is(TOKEN_TYPE::TRUE_));
}

unique_ptr<Expression> Parser::parse_prefix_expression()
{
    auto expression =
        make_unique<PrefixExpression>(*m_cur_token, m_cur_token->m_literal);

    next_token();
    expression->m_right = parse_expression(PRECEDENCE::PREFIX);

    return expression;
}

unique_ptr<Expression> Parser::parse_grouped_expression()
{
    next_token();

    auto exp = parse_expression(PRECEDENCE::LOWEST);

    if (!expect_peek(TOKEN_TYPE::RPAREN))
    {
        return nullptr;
    }

    return exp;
}

unique_ptr<Expression> Parser::parse_if_expression()
{
    auto expression = make_unique<IfExpression>(*m_cur_token);

    if (!expect_peek(TOKEN_TYPE::LPAREN))
    {
        return nullptr;
    }

    next_token();
    expression->m_condition = parse_expression(PRECEDENCE::LOWEST);

    if (!expect_peek(TOKEN_TYPE::RPAREN))
    {
        return nullptr;
    }

    if (!expect_peek(TOKEN_TYPE::LBRACE))
    {
        return nullptr;
    }

    expression->m_consequence = parse_block_statement();

    if (peek_token_is(TOKEN_TYPE::ELSE))
    {
        next_token();

        if (!expect_peek(TOKEN_TYPE::LBRACE))
        {
            return nullptr;
        }

        expression->m_alternative = parse_block_statement();
    }

    return expression;
}

unique_ptr<BlockStatement> Parser::parse_block_statement()
{
    auto block = make_unique<BlockStatement>(*m_cur_token);

    next_token();

    while (!cur_token_is(TOKEN_TYPE::RBRACE) &&
           !cur_token_is(TOKEN_TYPE::EOF_))
    {
        auto stmt = parse_statement();
        if (stmt != nullptr)
        {
            block->m_statements.push_back(std::move(stmt));
        }
        next_token();
    }

    return block;
}

unique_ptr<Expression> Parser::parse_function_literal()
{
    auto lit = make_unique<FunctionLiteral>(*m_cur_token);

    if (!expect_peek(TOKEN_TYPE::LPAREN))
    {
        return nullptr;
    }

    if (auto params = parse_function_parameters())
    {
        lit->m_parameters = std::move(params);
    }
    else
    {
        return nullptr;
    }

    if (!expect_peek(TOKEN_TYPE::LBRACE))
    {
        return nullptr;
    }

    lit->m_body = parse_block_statement();

    return lit;
}

unique_ptr<vector<unique_ptr<Identifier>>> Parser::parse_function_parameters()
{
    auto identifiers = make_unique<vector<unique_ptr<Identifier>>>();

    if (peek_token_is(TOKEN_TYPE::RPAREN))
    {
        next_token();
        return identifiers;
    }

    next_token();
    auto ident = make_unique<Identifier>(*m_cur_token, m_cur_token->m_literal);
    identifiers->push_back(std::move(ident));

    while (peek_token_is(TOKEN_TYPE::COMMA))
    {
        next_token();
        next_token();

        auto ident =
            make_unique<Identifier>(*m_cur_token, m_cur_token->m_literal);
        identifiers->push_back(std::move(ident));
    }

    if (!expect_peek(TOKEN_TYPE::RPAREN))
    {
        return nullptr;
    }

    return identifiers;
}

unique_ptr<Expression>
Parser::parse_call_expression(unique_ptr<Expression> funciton)
{
    auto exp = make_unique<CallExpression>(*m_cur_token, std::move(funciton));
    exp->m_arguments = parse_expression_list(TOKEN_TYPE::RPAREN);
    return exp;
}

// abandoned, replace by parse_expression_list
unique_ptr<vector<unique_ptr<Expression>>> Parser::parse_call_arguments()
{
    auto args = make_unique<vector<unique_ptr<Expression>>>();

    if (peek_token_is(TOKEN_TYPE::RPAREN))
    {
        next_token();
        return args;
    }

    next_token();
    auto exp = parse_expression(PRECEDENCE::LOWEST);
    args->push_back(std::move(exp));

    while (peek_token_is(TOKEN_TYPE::COMMA))
    {
        next_token();
        next_token();
        auto exp = parse_expression(PRECEDENCE::LOWEST);
        args->push_back(std::move(exp));
    }

    if (!expect_peek(TOKEN_TYPE::RPAREN))
    {
        return nullptr;
    }

    return args;
}

// copy from parse_call_arguments
unique_ptr<vector<unique_ptr<Expression>>>
Parser::parse_expression_list(TOKEN_TYPE end)
{
    auto args = make_unique<vector<unique_ptr<Expression>>>();

    if (peek_token_is(end))
    {
        next_token();
        return args;
    }

    next_token();
    auto exp = parse_expression(PRECEDENCE::LOWEST);
    args->push_back(std::move(exp));

    while (peek_token_is(TOKEN_TYPE::COMMA))
    {
        next_token();
        next_token();
        auto exp = parse_expression(PRECEDENCE::LOWEST);
        args->push_back(std::move(exp));
    }

    if (!expect_peek(end))
    {
        return nullptr;
    }

    return args;
}

unique_ptr<Expression> Parser::parse_string_literal()
{
    return make_unique<StringLiteral>(*m_cur_token, m_cur_token->m_literal);
}

unique_ptr<Expression> Parser::parse_array_literal()
{
    auto array = make_unique<ArrayLiteral>(*m_cur_token);

    array->m_elements = parse_expression_list(TOKEN_TYPE::RBRACKET);

    return array;
}

unique_ptr<Expression>
Parser::parse_index_expression(unique_ptr<Expression> left)
{

    auto exp = make_unique<IndexExpression>(*m_cur_token, std::move(left));
    next_token();
    exp->m_index = parse_expression(PRECEDENCE::LOWEST);

    if (!expect_peek(TOKEN_TYPE::RBRACKET))
        return nullptr;

    return exp;
}

unique_ptr<Expression> Parser::parse_hash_literal()
{
    auto hash = make_unique<HashLiteral>(*m_cur_token);

    hash->m_pairs =
        make_unique<map<shared_ptr<Expression>, shared_ptr<Expression>>>();

    while (!peek_token_is(TOKEN_TYPE::RBRACE))
    {
        next_token();

        auto key = parse_expression(PRECEDENCE::LOWEST);

        if (!expect_peek(TOKEN_TYPE::COLON))
        {
            return nullptr;
        }

        next_token();

        auto value = parse_expression(PRECEDENCE::LOWEST);
        (*hash->m_pairs)[std::move(key)] = std::move(value);

        if (!peek_token_is(TOKEN_TYPE::RBRACE) &&
            !expect_peek(TOKEN_TYPE::COMMA))
        {
            return nullptr;
        }
    }

    if (!expect_peek(TOKEN_TYPE::RBRACE))
    {
        return nullptr;
    }

    return hash;
}