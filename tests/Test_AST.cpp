#ifndef TEST_AST_HPP
#define TEST_AST_HPP

// #define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <memory>

#include "LetStatement.hpp"
#include "Program.hpp"
#include "../token/Constant.hpp"

using namespace token;

using namespace std;

TEST_CASE("test program tostring ", "[ast]") {
    Program p;

    Token t(TOKEN_TYPE::LET, "let");
    Token t1(TOKEN_TYPE::IDENT, "myVar");
    Token t2(TOKEN_TYPE::IDENT, "anotherVar");

    auto ts = make_unique<LetStatement>(
        t, (make_unique<Identifier>(t1, "myVar")),
        make_unique<Identifier>(t2, "anotherVar"));


    p.m_statements.push_back(std::move(ts));

    REQUIRE(p.to_string() == "let myVar = anotherVar;");
}

#endif /* TEST_AST_HPP */
