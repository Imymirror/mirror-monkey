#ifndef TEST_LEXER_HPP
#define TEST_LEXER_HPP

// #define CATCH_CONFIG_MAIN
#include "Constant.hpp"
#include "catch.hpp"

using namespace std;
using namespace mirror;

#include "Lexer.hpp"
#include <iostream>

TEST_CASE("lexer next_token test ", "[lexer]") {
    string input = R"(
let five = 5;
let ten = 10;

let add = fn(x, y) {
  x + y;
};

let result = add(five, ten);
!-/*5;
5 < 10 > 5;

if (5 < 10) {
	return true;
} else {
	return false;
}

10 == 10;
10 != 9;
"foobar"
"foo bar"
[1, 2];
{"foo": "bar"}
	)";


    using t2tuple = tuple<TOKEN_TYPE, string>;

    vector<t2tuple> vt = {

        {TOKEN_TYPE::LET, "let"},
        {TOKEN_TYPE::IDENT, "five"},
        {TOKEN_TYPE::ASSIGN, "="},
        {TOKEN_TYPE::INT, "5"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::LET, "let"},
        {TOKEN_TYPE::IDENT, "ten"},
        {TOKEN_TYPE::ASSIGN, "="},
        {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::LET, "let"},
        {TOKEN_TYPE::IDENT, "add"},
        {TOKEN_TYPE::ASSIGN, "="},
        {TOKEN_TYPE::FUNCTION, "fn"},
        {TOKEN_TYPE::LPAREN, "("},
        {TOKEN_TYPE::IDENT, "x"},
        {TOKEN_TYPE::COMMA, ","},
        {TOKEN_TYPE::IDENT, "y"},
        {TOKEN_TYPE::RPAREN, ")"},
        {TOKEN_TYPE::LBRACE, "{"},
        {TOKEN_TYPE::IDENT, "x"},
        {TOKEN_TYPE::PLUS, "+"},
        {TOKEN_TYPE::IDENT, "y"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::RBRACE, "}"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::LET, "let"},
        {TOKEN_TYPE::IDENT, "result"},
        {TOKEN_TYPE::ASSIGN, "="},
        {TOKEN_TYPE::IDENT, "add"},
        {TOKEN_TYPE::LPAREN, "("},
        {TOKEN_TYPE::IDENT, "five"},
        {TOKEN_TYPE::COMMA, ","},
        {TOKEN_TYPE::IDENT, "ten"},
        {TOKEN_TYPE::RPAREN, ")"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::BANG, "!"},
        {TOKEN_TYPE::MINUS, "-"},
        {TOKEN_TYPE::SLASH, "/"},
        {TOKEN_TYPE::ASTERISK, "*"},
        {TOKEN_TYPE::INT, "5"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::INT, "5"},
        {TOKEN_TYPE::LT, "<"},
        {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::GT, ">"},
        {TOKEN_TYPE::INT, "5"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::IF, "if"},
        {TOKEN_TYPE::LPAREN, "("},
        {TOKEN_TYPE::INT, "5"},
        {TOKEN_TYPE::LT, "<"},
        {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::RPAREN, ")"},
        {TOKEN_TYPE::LBRACE, "{"},
        {TOKEN_TYPE::RETURN, "return"},
        {TOKEN_TYPE::TRUE_, "true"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::RBRACE, "}"},
        {TOKEN_TYPE::ELSE, "else"},
        {TOKEN_TYPE::LBRACE, "{"},
        {TOKEN_TYPE::RETURN, "return"},
        {TOKEN_TYPE::FALSE_, "false"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::RBRACE, "}"},
        {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::EQ, "=="},
        {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::NOT_EQ, "!="},
        {TOKEN_TYPE::INT, "9"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::STRING, "foobar"},
        {TOKEN_TYPE::STRING, "foo bar"},
        {TOKEN_TYPE::LBRACKET, "["},
        {TOKEN_TYPE::INT, "1"},
        {TOKEN_TYPE::COMMA, ","},
        {TOKEN_TYPE::INT, "2"},
        {TOKEN_TYPE::RBRACKET, "]"},
        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::LBRACE, "{"},
        {TOKEN_TYPE::STRING, "foo"},
        {TOKEN_TYPE::COLON, ":"},
        {TOKEN_TYPE::STRING, "bar"},
        {TOKEN_TYPE::RBRACE, "}"},
        {TOKEN_TYPE::EOF_, ""},

    };

    Lexer l(input);

    for (int i = 0; i < vt.size(); i++) {
        auto t = vt[i];

        auto expected_type = get<0>(t);
        auto expected_literal = get<1>(t);

        auto tok = l.next_token();

        REQUIRE(tok->m_type == expected_type);
        REQUIRE(tok->m_literal == expected_literal);
    }
}

TEST_CASE("TestNextToken", "[lexer]") {
    string input = R"(
	let five = 5;
	let ten = 10;

	let add = fn(x, y) {
	x + y;
	};

	let result = add(five, ten);
	!-/*5;
	5 < 10 > 5;

	if (5 < 10) {
		return true;
	} else {
		return false;
	}

	10 == 10;
	10 != 9;
	"foobar"
	"foo bar"
	[1, 2];
	{"foo": "bar"}
)";

    using t2tuple = tuple<TOKEN_TYPE, string>;

    vector<t2tuple> vt = {
        {TOKEN_TYPE::LET, "let"},        {TOKEN_TYPE::IDENT, "five"},
        {TOKEN_TYPE::ASSIGN, "="},       {TOKEN_TYPE::INT, "5"},
        {TOKEN_TYPE::SEMICOLON, ";"},    {TOKEN_TYPE::LET, "let"},
        {TOKEN_TYPE::IDENT, "ten"},      {TOKEN_TYPE::ASSIGN, "="},
        {TOKEN_TYPE::INT, "10"},         {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::LET, "let"},        {TOKEN_TYPE::IDENT, "add"},
        {TOKEN_TYPE::ASSIGN, "="},       {TOKEN_TYPE::FUNCTION, "fn"},
        {TOKEN_TYPE::LPAREN, "("},       {TOKEN_TYPE::IDENT, "x"},
        {TOKEN_TYPE::COMMA, ","},        {TOKEN_TYPE::IDENT, "y"},
        {TOKEN_TYPE::RPAREN, ")"},       {TOKEN_TYPE::LBRACE, "{"},
        {TOKEN_TYPE::IDENT, "x"},        {TOKEN_TYPE::PLUS, "+"},
        {TOKEN_TYPE::IDENT, "y"},        {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::RBRACE, "}"},       {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::LET, "let"},        {TOKEN_TYPE::IDENT, "result"},
        {TOKEN_TYPE::ASSIGN, "="},       {TOKEN_TYPE::IDENT, "add"},
        {TOKEN_TYPE::LPAREN, "("},       {TOKEN_TYPE::IDENT, "five"},
        {TOKEN_TYPE::COMMA, ","},        {TOKEN_TYPE::IDENT, "ten"},
        {TOKEN_TYPE::RPAREN, ")"},       {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::BANG, "!"},         {TOKEN_TYPE::MINUS, "-"},
        {TOKEN_TYPE::SLASH, "/"},        {TOKEN_TYPE::ASTERISK, "*"},
        {TOKEN_TYPE::INT, "5"},          {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::INT, "5"},          {TOKEN_TYPE::LT, "<"},
        {TOKEN_TYPE::INT, "10"},         {TOKEN_TYPE::GT, ">"},
        {TOKEN_TYPE::INT, "5"},          {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::IF, "if"},          {TOKEN_TYPE::LPAREN, "("},
        {TOKEN_TYPE::INT, "5"},          {TOKEN_TYPE::LT, "<"},
        {TOKEN_TYPE::INT, "10"},         {TOKEN_TYPE::RPAREN, ")"},
        {TOKEN_TYPE::LBRACE, "{"},       {TOKEN_TYPE::RETURN, "return"},
        {TOKEN_TYPE::TRUE_, "true"},     {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::RBRACE, "}"},       {TOKEN_TYPE::ELSE, "else"},
        {TOKEN_TYPE::LBRACE, "{"},       {TOKEN_TYPE::RETURN, "return"},
        {TOKEN_TYPE::FALSE_, "false"},   {TOKEN_TYPE::SEMICOLON, ";"},
        {TOKEN_TYPE::RBRACE, "}"},       {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::EQ, "=="},          {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::SEMICOLON, ";"},    {TOKEN_TYPE::INT, "10"},
        {TOKEN_TYPE::NOT_EQ, "!="},      {TOKEN_TYPE::INT, "9"},
        {TOKEN_TYPE::SEMICOLON, ";"},    {TOKEN_TYPE::STRING, "foobar"},
        {TOKEN_TYPE::STRING, "foo bar"}, {TOKEN_TYPE::LBRACKET, "["},
        {TOKEN_TYPE::INT, "1"},          {TOKEN_TYPE::COMMA, ","},
        {TOKEN_TYPE::INT, "2"},          {TOKEN_TYPE::RBRACKET, "]"},
        {TOKEN_TYPE::SEMICOLON, ";"},    {TOKEN_TYPE::LBRACE, "{"},
        {TOKEN_TYPE::STRING, "foo"},     {TOKEN_TYPE::COLON, ":"},
        {TOKEN_TYPE::STRING, "bar"},     {TOKEN_TYPE::RBRACE, "}"},
        {TOKEN_TYPE::EOF_, ""},

    };

    Lexer l(input);

    for (int i = 0; i < vt.size(); i++) {
        auto t = vt[i];

        auto expected_type = get<0>(t);
        auto expected_literal = get<1>(t);

        auto tok = l.next_token();

        cout << __func__ << " $$$ " << expected_literal << " $$$ " << endl;

        REQUIRE(tok->m_type == expected_type);
        REQUIRE(tok->m_literal == expected_literal);
    }
}

#endif /* TEST_LEXER_HPP */
