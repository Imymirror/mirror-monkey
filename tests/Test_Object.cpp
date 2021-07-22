#ifndef TEST_OBJECT_H
#define TEST_OBJECT_H

#include "../object/Boolean.hpp"
#include "Integer.hpp"
#include "String.hpp"
#include "catch.hpp"
#include <string>

using namespace std;

TEST_CASE("TestStringHashKey") {
    auto hello1 = make_shared<String>("Hello World");
    auto hello2 = make_shared<String>("Hello World");
    auto diff1 = make_shared<String>("My name is johnny");
    auto diff2 = make_shared<String>("My name is johnny");

    REQUIRE(hello1->hash_key() == hello2->hash_key());
    REQUIRE(diff1->hash_key() == diff2->hash_key());
    REQUIRE(hello1->hash_key() != diff1->hash_key());
}

TEST_CASE("TestBooleanHashKey") {
    auto true1 = make_shared<object::Boolean>(true);
    auto true2 = make_shared<object::Boolean>(true);
    auto false1 = make_shared<object::Boolean>(false);
    auto false2 = make_shared<object::Boolean>(false);

    REQUIRE(true1->hash_key() == true2->hash_key());
    REQUIRE(false1->hash_key() == false2->hash_key());
    REQUIRE(true1->hash_key() != false1->hash_key());
}
TEST_CASE("TestIntegerHashKey") {
    auto one1 = make_shared<Integer>(1);
    auto one2 = make_shared<Integer>(1);
    auto two1 = make_shared<Integer>(2);
    auto two2 = make_shared<Integer>(2);

    REQUIRE(one1->hash_key() == one2->hash_key());
    REQUIRE(two1->hash_key() == two2->hash_key());
    REQUIRE(one1->hash_key() != two1->hash_key());
}

#endif // TEST_OBJECT_H