#ifndef TEST_EVALUATOR_HPP
#define TEST_EVALUATOR_HPP

#include "../object/Boolean.hpp"

#include "Array.hpp"
#include "Function.hpp"
#include "Hash.hpp"
#include "Integer.hpp"
#include "Object.hpp"
#include "String.hpp"
#include "catch.hpp"

#include "Environment.hpp"
#include "Evaluator.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include <any>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ratio>
#include <tuple>
#include <typeindex>

using namespace mirror;
using namespace std;

void test_null_object(Object *obj)
{
    auto cast_obj = static_cast<Null *>(obj);
    REQUIRE(obj->type() == object::OBJECT_TYPE::NULL_OBJ);

    REQUIRE((obj->Inspect() == "null"));
}

shared_ptr<Object> test_eval(string input)
{
    Lexer l(input);
    Parser p(l);
    auto program = p.parse_program();

    // for (auto tok = l.next_token(); (*tok).m_type != TOKEN_TYPE::EOF_;
    //     tok = l.next_token()) {
    //     cout << *tok << endl;
    // }

    auto env = make_shared<Environment>();

    Evaluator e;
    return e.eval(program.get(), env.get());
}

void test_integer_object(Object &obj, int64_t expected)
{
    auto &result = static_cast<Integer &>(obj);
    REQUIRE(result.m_value == expected);
}

void test_boolean_object(Object &obj, bool expected)
{
    auto &result = static_cast<object::Boolean &>(obj);
    REQUIRE(result.m_value == expected);
}

TEST_CASE("test eval integer expression")
{

    vector<std::tuple<string, int64_t>> tests = {
        {"5", 5},
        {"10", 10},
        {"-5", -5},
        {"-10", -10},
        {"5 + 5 + 5 + 5 - 10", 10},
        {"2 * 2 * 2 * 2 * 2", 32},
        {"-50 + 100 + -50", 0},
        {"5 * 2 + 10", 20},
        {"5 + 2 * 10", 25},
        {"20 + 2 * -10", 0},
        {"50 / 2 * 2 + 10", 60},
        {"2 * (5 + 10)", 30},
        {"3 * 3 * 3 + 10", 37},
        {"3 * (3 * 3) + 10", 37},
        {"(5 + 10 * 2 + 15 / 3) * 2 + -10", 50},
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);
        test_integer_object(*evaluated, expected_value);
    }
}

TEST_CASE("test eval boolean expression")
{
    vector<tuple<string, bool>> tests = {
        {"true", true},
        {"false", false},
        {"1 < 2", true},
        {"1 > 2", false},
        {"1 < 1", false},
        {"1 > 1", false},
        {"1 == 1", true},
        {"1 != 1", false},
        {"1 == 2", false},
        {"1 != 2", true},
        {"true == true", true},
        {"false == false", true},
        {"true == false", false},
        {"true != false", true},
        {"false != true", true},
        {"(1 < 2) == true", true},
        {"(1 < 2) == false", false},
        {"(1 > 2) == true", false},
        {"(1 > 2) == false", true},
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);
        test_boolean_object(*evaluated, expected_value);
    }
}

TEST_CASE("test bang operator")
{
    vector<tuple<string, bool>> tests = {
        {"!true", false},
        {"!false", true},
        {"!5", false},
        {"!!true", true},
        {"!!false", false},
        {"!!5", true},
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);
        test_boolean_object(*evaluated, expected_value);
    }
}

TEST_CASE("TestIfElseExpressions")
{
    vector<tuple<string, any>> tests = {
        {"if (true) { 10 }", 10},
        {"if (false) { 10 }", nullptr},
        {"if (1) { 10 }", 10},
        {"if (1 < 2) { 10 }", 10},
        {"if (1 > 2) { 10 }", nullptr},
        {"if (1 > 2) { 10 } else { 20 }", 20},
        {"if (1 < 2) { 10 } else { 20 }", 10},
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];

        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);

        auto expected_value_type = std::type_index(expected_value.type());
        if (expected_value_type == std::type_index(typeid(int)))
        {
            test_integer_object(*evaluated, any_cast<int>(expected_value));
        }

        if (expected_value_type == std::type_index(typeid(nullptr)))
        {

            test_null_object(evaluated.get());
        }
    }
}

TEST_CASE("TestReturnStatements")
{

    vector<tuple<string, int64_t>> tests = {
        {"return 10;", 10},
        {"return 10; 9;", 10},
        {"return 2 * 5; 9;", 10},
        {"9; return 2 * 5; 9;", 10},
        {"if (10 > 1) { return 10; }", 10},
        {
            R"(
if (10 > 1) {
  if (10 > 1) {
    return 10;
  }

  return 1;
}
)",
            10,
        },
        {
            R"(
let f = fn(x) {
  return x;
  x + 10;
};
f(10);)",
            10,
        },
        {
            R"(
let f = fn(x) {
   let result = x + 10;
   return result;
   return 10;
};
f(10);)",
            20,
        },
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);
        test_integer_object(*evaluated, expected_value);
    }
}

TEST_CASE("test error handling")
{
    vector<tuple<string, string>> tests = {
        {
            "5 + true;",
            "type mismatch: INTEGER + BOOLEAN",
        },
        {
            "5 + true; 5;",
            "type mismatch: INTEGER + BOOLEAN",
        },
        {
            "-true",
            "unknown operator: -BOOLEAN",
        },
        {
            "true + false;",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            "true + false + true + false;",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            "5; true + false; 5",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            "if (10 > 1) { true + false; }",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            R"( "Hello" - "World" )",
            "unknown operator: STRING - STRING",
        },
        {
            "foobar",
            "identifier not found: foobar",
        },
        {
            R"(
        if (10 > 1) {
          if (10 > 1) {
            return true + false;
          }

          return 1;
        })",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);

        auto err_obj = dynamic_cast<Error *>(evaluated.get());

        REQUIRE(err_obj->m_message == expected_value);
    }
}
TEST_CASE("TestLetStatements")
{
    vector<tuple<string, int64_t>> tests = {
        {"let a = 5; a;", 5},
        {"let a = 5 * 5; a;", 25},
        {"let a = 5; let b = a; b;", 5},
        {"let a = 5; let b = a; let c = a + b + 5; c;", 15},
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);
        test_integer_object(*evaluated, expected_value);
    }
}

TEST_CASE("TestFunctionObject")
{
    auto input = "fn(x) { x + 2; };";

    auto evaluated = test_eval(input);

    auto fn = static_cast<Function *>(evaluated.get());

    REQUIRE((*fn->m_parameters).size() == 1);
    REQUIRE((*fn->m_parameters)[0]->to_string() == "x");
    REQUIRE(fn->m_body->to_string() == "(x + 2)");
}

TEST_CASE("TestFunctionApplication")
{
    vector<tuple<string, int64_t>> tests = {
        {"let identity = fn(x) { x; }; identity(5);", 5},
        {"let identity = fn(x) { return x; }; identity(5);", 5},
        {"let double = fn(x) { x * 2; }; double(5);", 10},
        {"let add = fn(x, y) { x + y; }; add(5, 5);", 10},
        {"let add = fn(x, y) { x + y; }; add(5 + 5, add(5, 5));", 20},
        {"fn(x) { x; }(5)", 5},
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);
        test_integer_object(*evaluated, expected_value);
    }
}
TEST_CASE("TestEnclosingEnvironments")
{
    auto input = R"(
let first = 10;
let second = 10;
let third = 10;

let ourFunction = fn(first) {
  let second = 20;

  first + second + third;
};

ourFunction(20) + first + second;
	)";

    auto evaluated = test_eval(input);
    test_integer_object(*evaluated, 70);
}

TEST_CASE("TestClosures") {}
TEST_CASE("TestStringLiteral")
{
    auto input = R"("Hello World!")";

    auto evaluated = test_eval(input);
    auto str = static_cast<String *>(evaluated.get());

    REQUIRE(str->m_value == "Hello World!");
}

TEST_CASE("TestStringConcatenation")
{
    auto input = R"("Hello" + " " + "World!")";

    auto evaluated = test_eval(input);
    auto str = static_cast<String *>(evaluated.get());

    REQUIRE(str->m_value == "Hello World!");
}

TEST_CASE("TestBuiltinFunctions")
{
    vector<tuple<string, any>> tests = {
        {R"(len(""))", 0},
        {R"(len("four"))", 4},
        {R"(len("hello world"))", 11},
        {R"(len(1))", string("argument to `len` not supported, got INTEGER")},
        {R"(len("one", "two"))",
         string("wrong number of arguments. got=2, want=1")},
        {R"(  len([1, 2, 3]) )", 3},
        {R"(  len([]) )", 0},
        {R"( puts("hello", "world!") )", nullptr},

        {R"(first([ 1, 2, 3 ]))", 1},
        {R"(first([]))", nullptr},
        {R"(first(1))", "argument to `first` must be ARRAY, got INTEGER"},
        {R"(last([ 1, 2, 3 ]))", 3},
        {R"(last([]))", nullptr},
        {R"(last(1))", "argument to `last` must be ARRAY, got INTEGER"},
        {R"(rest([ 1, 2, 3 ]))", vector<int>{2, 3}},
        {R"(rest([]))", nullptr},
        {R"(push([], 1))", vector<int>{1}},
        {R"(push(1, 1))", "argument to `push` must be ARRAY, got INTEGER"},
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);

        auto t_index = std::type_index(expected_value.type());

        if (t_index == std::type_index(typeid(int)))
        {
            auto expected_p = std::any_cast<int>(expected_value);
            test_integer_object(*evaluated, expected_p);
        }
        else if (t_index == std::type_index(typeid(string)))
        {
            auto err = static_cast<Error *>(evaluated.get());
            auto expected_cast = std::any_cast<string>(expected_value);
            REQUIRE(err->m_message == expected_cast);
        }
        else if (t_index == std::type_index(typeid(nullptr)))
        {
            auto cast_node = static_cast<Null *>(evaluated.get());
            test_null_object(evaluated.get());
        }
        else if (t_index == std::type_index(typeid(vector<int>)))
        {
            auto cast_node = static_cast<Array *>(evaluated.get());
            auto cast_expected = any_cast<vector<int>>(expected_value);

            REQUIRE(cast_node->m_elements.size() == cast_expected.size());

            for (int i = 0; i < cast_expected.size(); i++)
            {
                test_integer_object(*cast_node->m_elements[i],
                                    cast_expected[i]);
            }
        }
    }
}

TEST_CASE("TestArrayLiterals")
{
    string input = "[1, 2 * 2, 3 + 3]";

    auto evaluated = test_eval(input);

    auto result = dynamic_cast<Array *>(evaluated.get());
    REQUIRE(result->m_elements.size() == 3);

    test_integer_object(*result->m_elements[0], 1);
    test_integer_object(*result->m_elements[1], 4);
    test_integer_object(*result->m_elements[2], 6);
}

TEST_CASE("TestArrayIndexExpressions")
{
    vector<tuple<string, any>> tests = {
        {
            "[1, 2, 3][0]",
            1,
        },
        {
            "[1, 2, 3][1]",
            2,
        },
        {
            "[1, 2, 3][2]",
            3,
        },
        {
            "let i = 0; [1][i];",
            1,
        },
        {
            "[1, 2, 3][1 + 1];",
            3,
        },
        {
            "let myArray = [1, 2, 3]; myArray[2];",
            3,
        },
        {
            "let myArray = [1, 2, 3]; myArray[0] + myArray[1] + myArray[2];",
            6,
        },
        {
            "let myArray = [1, 2, 3]; let i = myArray[0]; myArray[i]",
            2,
        },
        {
            "[1, 2, 3][3]",
            0,
        },
        {
            "[1, 2, 3][-1]",
            0,
        },
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected = get<1>(test);

        auto evaluated = test_eval(input);
        auto integer = any_cast<int>(expected);
        if (integer)
        {
            test_integer_object(*evaluated, integer);
        }
        else
        {
            test_null_object(evaluated.get());
        }
    }
}

TEST_CASE("TestDriving Arrays")
{
    {
        string input = R"(
	let map = fn(arr, f) {
		let iter = fn(arr, accumulated) { 
			if (len(arr) == 0) {
				accumulated
			} else {
				iter(rest(arr), push(accumulated, f(first(arr))));
			} 
		};
		iter(arr, []);
	};

	let a = [1, 2, 3, 4];
	let double = fn(x) { x * 2 }; 
	map(a, double);
		)";

        auto evaluated = test_eval(input);
        cout << __func__ << " $$$ map a = " << evaluated->Inspect() << " $$$ "
             << endl;
    }

    {
        auto input = R"(
	let reduce = fn(arr, initial, f) { 
		let iter = fn(arr, result) {
			if (len(arr) == 0) { 
				result
			} else {
				iter(rest(arr), f(result, first(arr)));
			} 
		};
		iter(arr, initial);
	};

	let sum = fn(arr) {
		reduce(arr, 0, fn(initial, el) { initial + el });
	};

	sum([1, 2, 3, 4, 5]);
	)";

        auto evaluated = test_eval(input);
        cout << __func__ << " $$$ sum =" << evaluated->Inspect() << " $$$ "
             << endl;
    }
}

TEST_CASE("TestHashLiterals")
{
    string input = R"(
	let two = "two";
		{
			"one": 10 - 9,
			two: 1 + 1,
			"thr" + "ee": 6 / 2,
			4: 4,
			true: 5,
			false: 6
		}

		)";

    auto evaluated = test_eval(input);
    auto result = static_cast<Hash *>(evaluated.get());

    map<HashKey, int64_t> expected = {
        {((make_shared<String>("one"))->hash_key()), 1},
        {(make_shared<String>("two"))->hash_key(), 2},
        {(make_shared<String>("three"))->hash_key(), 3},
        {(make_shared<Integer>(4))->hash_key(), 4},
        {(make_shared<object::Boolean>(true))->hash_key(), 5},
        {(make_shared<object::Boolean>(false))->hash_key(), 6},
    };

    REQUIRE(result->m_pairs->size() == expected.size());

    auto it = expected.begin();
    while (it != expected.end())
    {
        auto pairs = *(*result).m_pairs;
        auto hash_key = it->first;
        auto pair = pairs[hash_key];
        test_integer_object(*pair->m_value, it->second);
        it++;
    }
}

TEST_CASE("TestHashIndexExpressions")
{

    vector<tuple<string, any>> tests = {
        {
            R"({"foo": 5}["foo"])",
            5,
        },
        {
            R"({"foo" : 5}["bar"])",
            nullptr,
        },
        {
            R"(let key = "foo"; {"foo": 5}[key])",
            5,
        },
        {
            R"({}["foo"])",
            nullptr,
        },
        {
            R"({5: 5}[5])",
            5,
        },
        {
            R"({true: 5}[true])",
            5,
        },
        {
            R"({false: 5}[false])",
            5,
        },
    };

    for (int i = 0; i < tests.size(); i++)
    {
        auto test = tests[i];
        auto input = get<0>(test);
        auto expected_value = get<1>(test);

        auto evaluated = test_eval(input);

        try
        {
            auto integer = any_cast<int>(expected_value);

            test_integer_object(*evaluated, integer);
        }
        catch (const std::bad_any_cast &e)
        {
            // std::cout << e.what() << '\n';
            test_null_object(evaluated.get());
        }
    }
}

#endif /* TEST_EVALUATOR_HPP */
