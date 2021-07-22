#ifndef TEST_PARSER_HPP
#define TEST_PARSER_HPP

#include "Expression.hpp"
#include "ExpressionStatement.hpp"
#include "catch.hpp"

#include "ArrayLiteral.hpp"
#include "Boolean.hpp"
#include "CallExpression.hpp"
#include "FunctionLiteral.hpp"
#include "HashLiteral.hpp"
#include "IfExpression.hpp"
#include "IndexExpression.hpp"
#include "InfixExpression.hpp"
#include "IntegerLiteral.hpp"
#include "LetStatement.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "PrefixExpression.hpp"
#include "ReturnStatement.hpp"
#include "StringLiteral.hpp"
#include <any>
#include <iostream>
#include <string>
#include <typeindex>
#include <utility>

using namespace std;
using namespace ast;

void test_literal_expression(Expression &exp, std::any expected_value);
void test_identifier(Expression *exp, string value);

void test_integer_literal(Expression &exp, int64_t value) {
    auto &integ = static_cast<IntegerLiteral &>(exp);

    REQUIRE(integ.m_value == value);
    REQUIRE(integ.token_literal() == to_string(value));
}

void check_parser_errors(Parser &p) {
    auto errors = p.errors();
    if (errors.size() == 0)
        return;

    for (auto msg : errors) {
        FAIL("parser error====: " + msg);
    }
}

shared_ptr<Program> check_and_get_program(Parser &p) {
    auto program = p.parse_program();
    // check errors
    check_parser_errors(p);

    if (program->m_statements.size() < 1) {
        FAIL("program.Statements does not contain 1 statements. got=" +
             to_string(program->m_statements.size()));
        return nullptr;
    }

    return program;
}

void test_identifier(Expression &exp, string value) {
    auto &ident = static_cast<Identifier &>(exp);

    REQUIRE(ident.m_value == value);
    REQUIRE(ident.token_literal() == value);
}

void test_infix_expression(Expression &exp, any left, string op, any right) {
    auto &expression = static_cast<InfixExpression &>(exp);

    test_literal_expression(*expression.m_left, left);
    REQUIRE(expression.m_operator == op);
    test_literal_expression(*expression.m_right, right);
}

void test_literal_expression(Expression &exp, std::any expected_value) {

    auto expected_value_type = std::type_index(expected_value.type());
    if (expected_value_type == std::type_index(typeid(int))) {
        REQUIRE(static_cast<IntegerLiteral &>(exp).m_value ==
                std::any_cast<int>(expected_value));
    } else if (expected_value_type == std::type_index(typeid(bool))) {
        REQUIRE(static_cast<Boolean &>(exp).m_value ==
                std::any_cast<bool>(expected_value));
    } else if (expected_value_type == std::type_index(typeid(string))) {
        REQUIRE(static_cast<Identifier &>(exp).m_value ==
                std::any_cast<string>(expected_value));
    }
}

TEST_CASE("test let statements ", "[parser]") {

    using t_tuple = tuple<string, string, std::any>;

    vector<t_tuple> tests = {
        {"let x = 5;", "x", 5},
        {"let foobar = y;", "foobar", string("y")},
        {"let y = true;", "y", true},
    };

    for (int i = 0; i < tests.size(); i++) {
        auto test = tests[i];

        auto input = get<0>(test);
        auto expected_identifier = get<1>(test);
        auto expected_value = get<2>(test);

        Parser p(input);
        auto pro = check_and_get_program(p);
        auto stmt = pro->m_statements[0].get();
        auto r_stmt = static_cast<LetStatement *>(stmt);

        REQUIRE(r_stmt->token_literal() == "let");

        REQUIRE(r_stmt->m_name->m_value == expected_identifier);
        REQUIRE(r_stmt->m_name->token_literal() == expected_identifier);

        auto r_value = r_stmt->m_value.get();
        auto t_index = std::type_index(expected_value.type());

        if (t_index == std::type_index(typeid(int))) {
            REQUIRE(static_cast<IntegerLiteral *>(r_value)->m_value ==
                    std::any_cast<int>(expected_value));
        } else if (t_index == std::type_index(typeid(string))) {
            REQUIRE(static_cast<Identifier *>(r_value)->m_value ==
                    std::any_cast<string>(expected_value));
        } else if (t_index == std::type_index(typeid(bool))) {
            REQUIRE(static_cast<Boolean *>(r_value)->m_value ==
                    std::any_cast<bool>(expected_value));
        }
    }
}

TEST_CASE("test return statements", "[parser]") {

    vector<tuple<string, std::any>> tests = {
        {"return 5;", 5},
        {"return true;", true},
        {"return foobar;", string("foobar")},
    };

    for (int i = 0; i < tests.size(); i++) {
        auto test = tests[i];

        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        Parser p(input);
        auto program = check_and_get_program(p);
        auto stmt = program->m_statements[0].get();
        auto return_stmt = static_cast<ReturnStatement *>(stmt);

        REQUIRE(return_stmt->token_literal() == "return");

        auto return_value = return_stmt->m_return_value.get();

        auto expected_value_type = std::type_index(expected_value.type());

        test_literal_expression(*return_value, expected_value);
    }
}

TEST_CASE("test Identifier expression", "[parser]") {
    string input = "foobar;";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto stmt = program->m_statements[0].get();
    auto r_stmt = static_cast<ExpressionStatement *>(stmt);

    auto ident = static_cast<Identifier *>(r_stmt->m_expression.get());

    REQUIRE(ident->m_value == "foobar");
    REQUIRE(ident->token_literal() == "foobar");
}

TEST_CASE("test IntegerLiteral expression", "[parser]") {
    string input = "5;";

    Parser p(input);
    auto program = check_and_get_program(p);
    auto stmt = program->m_statements[0].get();

    auto r_stmt = static_cast<ExpressionStatement *>(stmt);
    auto r_exp = static_cast<IntegerLiteral *>(r_stmt->m_expression.get());

    REQUIRE(r_exp->m_value == 5);
    REQUIRE(r_exp->token_literal() == "5");
}

TEST_CASE("test parsing prefix expressions", "[parser]") {
    vector<tuple<string, string, any>> tests = {{"!5;", "!", 5},
                                                {"-15;", "-", 15},
                                                {"!foobar;", "!", "foobar"},
                                                {"-foobar;", "-", "foobar"},
                                                {"!true;", "!", true},
                                                {"!false;", "!", false}};

    for (int i = 0; i < tests.size(); i++) {
        string input = get<0>(tests[i]);
        auto expected_op = get<1>(tests[i]);
        auto expected_value = get<1>(tests[i]);

        Parser p(input);
        auto program = check_and_get_program(p);
        auto stmt = program->m_statements[0].get();
        auto r_stmt = static_cast<ExpressionStatement *>(stmt);
        auto r_expression =
            static_cast<PrefixExpression *>(r_stmt->m_expression.get());

        REQUIRE(r_expression->m_operator == expected_op);

        test_literal_expression(*r_expression, expected_value);
    }
}

TEST_CASE("test parsing infix expressions", "[parser]") {
    vector<tuple<string, any, string, any>> tests = {
        {"5 + 5;", 5, "+", 5},
        {"5 - 5;", 5, "-", 5},
        {"5 * 5;", 5, "*", 5},
        {"5 / 5;", 5, "/", 5},
        {"5 > 5;", 5, ">", 5},
        {"5 < 5;", 5, "<", 5},
        {"5 == 5;", 5, "==", 5},
        {"5 != 5;", 5, "!=", 5},
        {"foobar + barfoo;", "foobar", "+", "barfoo"},
        {"foobar - barfoo;", "foobar", "-", "barfoo"},
        {"foobar * barfoo;", "foobar", "*", "barfoo"},
        {"foobar / barfoo;", "foobar", "/", "barfoo"},
        {"foobar > barfoo;", "foobar", ">", "barfoo"},
        {"foobar < barfoo;", "foobar", "<", "barfoo"},
        {"foobar == barfoo;", "foobar", "==", "barfoo"},
        {"foobar != barfoo;", "foobar", "!=", "barfoo"},
        {"true == true", true, "==", true},
        {"true != false", true, "!=", false},
        {"false == false", false, "==", false},
    };

    for (int i = 0; i < tests.size(); i++) {
        string input = get<0>(tests[i]);
        auto expected_left = get<1>(tests[i]);
        auto expected_op = get<2>(tests[i]);
        auto expected_right = get<3>(tests[i]);

        Parser p(input);
        auto pro = check_and_get_program(p);
        auto stmt = pro->m_statements[0].get();
        auto r_stmt = static_cast<ExpressionStatement *>(stmt);
        auto expression =
            static_cast<InfixExpression *>(r_stmt->m_expression.get());

        test_literal_expression(*expression->m_left, expected_left);
        REQUIRE(expression->m_operator == expected_op);
        test_literal_expression(*expression->m_right, expected_right);
    }
}

TEST_CASE("TestOperatorPrecedenceParsing", "[parser]") {
    vector<tuple<string, string>> tests = {
        {
            "-a * b",
            "((-a) * b)",
        },
        {
            "!-a",
            "(!(-a))",
        },
        {
            "a + b + c",
            "((a + b) + c)",
        },
        {
            "a + b - c",
            "((a + b) - c)",
        },
        {
            "a * b * c",
            "((a * b) * c)",
        },
        {
            "a * b / c",
            "((a * b) / c)",
        },
        {
            "a + b / c",
            "(a + (b / c))",
        },
        {
            "a + b * c + d / e - f",
            "(((a + (b * c)) + (d / e)) - f)",
        },
        {
            "3 + 4; -5 * 5",
            "(3 + 4)((-5) * 5)",
        },
        {
            "5 > 4 == 3 < 4",
            "((5 > 4) == (3 < 4))",
        },
        {
            "5 < 4 != 3 > 4",
            "((5 < 4) != (3 > 4))",
        },
        {
            "3 + 4 * 5 == 3 * 1 + 4 * 5",
            "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))",
        },
        {
            "true",
            "true",
        },
        {
            "false",
            "false",
        },
        {
            "3 > 5 == false",
            "((3 > 5) == false)",
        },
        {
            "3 < 5 == true",
            "((3 < 5) == true)",
        },
        {
            "1 + (2 + 3) + 4",
            "((1 + (2 + 3)) + 4)",
        },
        {
            "(5 + 5) * 2",
            "((5 + 5) * 2)",
        },
        {
            "2 / (5 + 5)",
            "(2 / (5 + 5))",
        },
        {
            "(5 + 5) * 2 * (5 + 5)",
            "(((5 + 5) * 2) * (5 + 5))",
        },
        {
            "-(5 + 5)",
            "(-(5 + 5))",
        },
        {
            "!(true == true)",
            "(!(true == true))",
        },
        {
            "a + add(b * c) + d",
            "((a + add((b * c))) + d)",
        },
        {
            "add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))",
            "add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))",
        },
        {
            "add(a + b + c * d / f + g)",
            "add((((a + b) + ((c * d) / f)) + g))",
        },
        {
            "a * [1, 2, 3, 4][b * c] * d",
            "((a * ([1, 2, 3, 4][(b * c)])) * d)",
        },
        {
            "add(a * b[2], b[1], 2 * [1, 2][1])",
            "add((a * (b[2])), (b[1]), (2 * ([1, 2][1])))",
        },
    };

    for (int i = 0; i < tests.size(); i++) {
        auto input = get<0>(tests[i]);
        auto expected_value = get<1>(tests[i]);

        Parser p(input);
        auto program = check_and_get_program(p);

        cout << __func__ << " $$$ " << program->to_string() << " $$$ " << endl;

        REQUIRE(program->to_string() == expected_value);
    }
}

TEST_CASE("test boolean expression") {

    vector<tuple<string, bool>> tests = {
        {"true;", true},
        {"false;", false},
    };

    for (int i = 0; i < tests.size(); i++) {
        auto test = tests[i];
        string input = get<0>(test);
        auto expected_value = get<1>(test);

        Parser p(input);
        auto program = check_and_get_program(p);
        auto stmt = program->m_statements[0].get();
        auto r_stmt = static_cast<ExpressionStatement *>(stmt);

        auto r_exp = static_cast<Boolean *>(r_stmt->m_expression.get());

        REQUIRE(r_exp->m_value == expected_value);
    }
}

TEST_CASE("test if expression") {

    auto input = "if (x < y) { x }";
    Parser p(input);
    auto program = check_and_get_program(p);

    auto stmt =
        static_cast<ExpressionStatement *>(program->m_statements[0].get());

    auto exp = static_cast<IfExpression *>(stmt->m_expression.get());

    test_infix_expression(*exp->m_condition, "x", "<", "y");

    auto consequence = static_cast<ExpressionStatement *>(
        exp->m_consequence->m_statements[0].get());
    test_identifier(*consequence->m_expression, "x");

    REQUIRE(exp->m_alternative == nullptr);
}

TEST_CASE("test if else expression") {
    auto input = "if (x < y) { x } else { y }";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto stmt =
        static_cast<ExpressionStatement *>(program->m_statements[0].get());

    auto exp = static_cast<IfExpression *>(stmt->m_expression.get());

    test_infix_expression(*exp->m_condition, "x", "<", "y");

    auto consequence = static_cast<ExpressionStatement *>(
        exp->m_consequence->m_statements[0].get());
    test_identifier(*consequence->m_expression, "x");

    auto alternative = static_cast<ExpressionStatement *>(
        exp->m_alternative->m_statements[0].get());
    test_identifier(*alternative->m_expression, "y");
}

TEST_CASE("test function literal parsing") {
    auto input = "fn(x, y) { x + y; }";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);

    auto &function = static_cast<FunctionLiteral &>(*stmt.m_expression);

    REQUIRE(function.m_parameters->size() == 2);

    test_literal_expression(*(*function.m_parameters)[0], "x");
    test_literal_expression(*(*function.m_parameters)[1], "y");

    REQUIRE(function.m_body->m_statements.size() == 1);

    auto &body_stmt =
        static_cast<ExpressionStatement &>(*(function.m_body->m_statements[0]));

    test_infix_expression(*body_stmt.m_expression, "x", "+", "y");
}

TEST_CASE("test function parameter parsing") {
    vector<tuple<string, vector<string>>> tests = {
        {"fn() {};", {}},
        {"fn(x) {};", {"x"}},
        {"fn(x, y, z) {};", {"x", "y", "z"}}};

    for (int i = 0; i < tests.size(); i++) {
        auto input = get<0>(tests[i]);
        auto expected_params = get<1>(tests[i]);

        Parser p(input);
        auto program = check_and_get_program(p);

        auto &stmt =
            static_cast<ExpressionStatement &>(*program->m_statements[0]);
        auto &function = static_cast<FunctionLiteral &>(*stmt.m_expression);

        REQUIRE(function.m_parameters->size() == expected_params.size());

        for (int j = 0; j < expected_params.size(); j++) {
            test_literal_expression(*((*function.m_parameters)[j]),
                                    expected_params[j]);
        }
    }
}

TEST_CASE("test call expression parsing") {
    auto input = "add(1, 2 * 3, 4 + 5);";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);
    auto &exp = static_cast<CallExpression &>(*stmt.m_expression);

    test_identifier(*exp.m_function, "add");

    REQUIRE(exp.m_arguments->size() == 3);
    test_literal_expression(*(*exp.m_arguments)[0], 1);
    test_infix_expression(*(*exp.m_arguments)[1], 2, "*", 3);
    test_infix_expression(*(*exp.m_arguments)[2], 4, "+", 5);
}

TEST_CASE("TestStringLiteralExpression") {
    auto input = R"("hello world";)";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);
    auto &exp = static_cast<StringLiteral &>(*stmt.m_expression);
    REQUIRE(exp.m_value == "hello world");
}

TEST_CASE("TestParsingArrayLiterals") {
    string input = "[1, 2 * 2, 3 + 3]";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);
    auto &array = static_cast<ArrayLiteral &>(*stmt.m_expression);

    REQUIRE(array.m_elements->size() == 3);
    test_integer_literal(*(*array.m_elements)[0], 1);
    test_infix_expression(*(*array.m_elements)[1], 2, "*", 2);
    test_infix_expression(*(*array.m_elements)[2], 3, "+", 3);
}

TEST_CASE("TestParsingIndexExpressions") {
    string input = "myArray[1 + 1]";
    Parser p(input);

    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);
    auto &exp = static_cast<IndexExpression &>(*stmt.m_expression);

    test_identifier(*exp.m_left, "myArray");

    test_infix_expression(*exp.m_index, 1, "+", 1);
}

TEST_CASE("TestParsingHashLiteralsStringKeys") {
    string input = R"({"one": 1, "two": 2, "three": 3})";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);
    auto &hash = static_cast<HashLiteral &>(*stmt.m_expression);

    map<string, int64_t> expected = {
        make_pair("one", 1),
        make_pair("two", 2),
        make_pair("three", 3),
    };

    REQUIRE(hash.m_pairs->size() == expected.size());

    auto it = hash.m_pairs->begin();
    while (it != hash.m_pairs->end()) {
        auto literal = static_cast<StringLiteral *>(it->first.get());

        auto expected_value = expected[literal->to_string()];
        test_integer_literal(*it->second, expected_value);
        it++;
    }
}

TEST_CASE("TestParsingEmptyHashLiteral") {
    string input = "{}";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);
    auto &hash = static_cast<HashLiteral &>(*stmt.m_expression);

    REQUIRE(hash.m_pairs->size() == 0);
}

TEST_CASE("TestParsingHashLiteralsWithExpressions") {
    string input = R"({"one": 0 + 1, "two": 10 - 8, "three": 15 / 5})";

    Parser p(input);
    auto program = check_and_get_program(p);

    auto &stmt = static_cast<ExpressionStatement &>(*program->m_statements[0]);
    auto &hash = static_cast<HashLiteral &>(*stmt.m_expression);

    REQUIRE(hash.m_pairs->size() == 3);

    auto tests = map<string, function<void(Expression &)>>{
        {"one", [](Expression &e) { test_infix_expression(e, 0, "+", 1); }},
        {"two", [](Expression &e) { test_infix_expression(e, 10, "-", 8); }},
        {"three", [](Expression &e) { test_infix_expression(e, 15, "/", 5); }},
    };

    auto it = hash.m_pairs->begin();
    while (it != hash.m_pairs->end()) {
        auto key = it->first;
        auto value = it->second;

        auto literal = static_cast<StringLiteral *>(key.get());
        auto test_func = tests[literal->to_string()];
        test_func(*value);
        it++;
    }

}

#endif /* TEST_PARSER_HPP */
