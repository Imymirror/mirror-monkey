#include "Token.hpp"
#include "Common.hpp"
#include "Constant.hpp"
#include <iostream>

using namespace mirror;
using namespace mirror::token;

using namespace std;

map<string, TOKEN_TYPE> mirror::Token::m_keywords = {
    {"fn", TOKEN_TYPE::FUNCTION},  {"let", TOKEN_TYPE::LET},
    {"true", TOKEN_TYPE::TRUE_},   {"false", TOKEN_TYPE::FALSE_},
    {"if", TOKEN_TYPE::IF},        {"else", TOKEN_TYPE::ELSE},
    {"return", TOKEN_TYPE::RETURN}};

Token::Token(TOKEN_TYPE token_type, string literal)
    : m_type(token_type), m_literal(literal) {}
Token::~Token() {}

std::ostream &operator<<(ostream &os, const Token &t) {
    os << "{ ";
    os << "type : " << static_cast<underlying_type<TOKEN_TYPE>::type>(t.m_type)
       << ", literal : " << t.m_literal.c_str();
    os << " }";
    return os;
}

TOKEN_TYPE Token::lookup_ident(string ident) {
    auto it = m_keywords.find(ident);
    if (it != m_keywords.end()) {
        return it->second;
    }
    return TOKEN_TYPE::IDENT;
}

string Token::token_type_value(TOKEN_TYPE t) {
    return TOKEN_TYPE_VALUE[common::as_integer(t)];
};