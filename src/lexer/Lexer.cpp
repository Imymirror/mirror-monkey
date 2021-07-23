#include "Lexer.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#include "Constant.hpp"

using namespace mirror;
using namespace mirror::token;

using namespace std;

Lexer::Lexer(std::string &str) : m_input(str.begin(), str.end()) {
    read_char();
}

Lexer::~Lexer() {
}

void Lexer::read_char() {
    if (m_readPosition >= m_input.size()) {
        m_ch = 0;
    } else {
        m_ch = m_input[m_readPosition];
    }

    m_position = m_readPosition++;
}

void Lexer::skip_whitespace() {
    while (m_ch == ' ' || m_ch == '\t' || m_ch == '\n' || m_ch == '\r')
        read_char();
}

unique_ptr<Token> Lexer::next_token() {
    unique_ptr<Token> tok;
    skip_whitespace();

    switch (m_ch) {
    case '=':
        if (peed_char() == '=') {
            tok = make_unique<Token>(TOKEN_TYPE::EQ, capture_char(2));
        } else {
            tok = make_unique<Token>(TOKEN_TYPE::ASSIGN, string(1, m_ch));
        }
        break;
    case '+':
        tok = make_unique<Token>(TOKEN_TYPE::PLUS, string(1, m_ch));
        break;
    case '-':
        tok = make_unique<Token>(TOKEN_TYPE::MINUS, string(1, m_ch));
        break;
    case '!':
        if (peed_char() == '=') {
            tok = make_unique<Token>(TOKEN_TYPE::NOT_EQ, capture_char(2));
        } else {
            tok = make_unique<Token>(TOKEN_TYPE::BANG, string(1, m_ch));
        }
        break;
    case '/':
        tok = make_unique<Token>(TOKEN_TYPE::SLASH, string(1, m_ch));
        break;
    case '*':
        tok = make_unique<Token>(TOKEN_TYPE::ASTERISK, string(1, m_ch));
        break;
    case '<':
        tok = make_unique<Token>(TOKEN_TYPE::LT, string(1, m_ch));
        break;
    case '>':
        tok = make_unique<Token>(TOKEN_TYPE::GT, string(1, m_ch));
        break;
    case ';':
        tok = make_unique<Token>(TOKEN_TYPE::SEMICOLON, string(1, m_ch));
        break;
    case ':':
        tok = make_unique<Token>(TOKEN_TYPE::COLON, string(1, m_ch));
        break;
    case ',':
        tok = make_unique<Token>(TOKEN_TYPE::COMMA, string(1, m_ch));
        break;
    case '{':
        tok = make_unique<Token>(TOKEN_TYPE::LBRACE, string(1, m_ch));
        break;
    case '}':
        tok = make_unique<Token>(TOKEN_TYPE::RBRACE, string(1, m_ch));
        break;
    case '(':
        tok = make_unique<Token>(TOKEN_TYPE::LPAREN, string(1, m_ch));
        break;
    case ')':
        tok = make_unique<Token>(TOKEN_TYPE::RPAREN, string(1, m_ch));
        break;
    case '[':
        tok = make_unique<Token>(TOKEN_TYPE::LBRACKET, string(1, m_ch));
        break;
    case ']':
        tok = make_unique<Token>(TOKEN_TYPE::RBRACKET, string(1, m_ch));
        break;
    case '"':
        tok = make_unique<Token>(TOKEN_TYPE::STRING, read_string());
        break;
    case 0:
        tok = make_unique<Token>(TOKEN_TYPE::EOF_, string(""));
        break;
    default:
        if (is_letter(m_ch)) {
            string literal = read_identifier();
            tok = make_unique<Token>(Token::lookup_ident(literal), literal);
            return tok;
        } else if (is_digit(m_ch)) {
            string literal = read_number();
            tok = make_unique<Token>(TOKEN_TYPE::INT, literal);
            return tok;
        } else {
            tok = make_unique<Token>(TOKEN_TYPE::ILLEGAL, string(1, m_ch));
        }

        break;
    }

    read_char();
    return tok;
}

char Lexer::peed_char() {
    if (m_readPosition >= m_input.size()) {
        return 0;
    } else {
        return m_input[m_readPosition];
    }
}

string Lexer::capture_char(int number) {

    string literal(1, m_ch);

    while (--number > 0) {
        read_char();
        literal += m_ch;
    }

    return literal;
}

bool Lexer::is_letter(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

bool Lexer::is_digit(char ch) { return '0' <= ch && ch <= '9'; }

string Lexer::read_identifier() {
    vector<char> v;

    while (is_letter(m_ch)) {
        v.push_back(m_ch);
        read_char();
    }
    return string(v.begin(), v.end());
}

string Lexer::read_number() {
    vector<char> v;

    while (is_digit(m_ch)) {
        v.push_back(m_ch);
        read_char();
    }

    return string(v.begin(), v.end());
}

string Lexer::read_string() {
    // a different implementation , not like read_number, ugly
    auto position = m_position + 1;
    for (int i = 0; i < 20; i++) {
        read_char();

        if (m_ch == '"' || m_ch == 0) {
            break;
        }
    }
    auto begin = m_input.begin();
    vector<char> ret;
    copy(begin + position, begin + m_position, std::back_inserter(ret));

    return string(ret.begin(), ret.end());
}