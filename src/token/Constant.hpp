#ifndef TOKEN_CONSTANT_HPP
#define TOKEN_CONSTANT_HPP

#include <string>
using namespace std;

namespace mirror {

namespace token {
enum class TOKEN_TYPE {
    ILLEGAL, // "ILLEGAL"
    EOF_,    // "EOF"

    // Identifiers + literals
    IDENT,  // "IDENT" // add, foobar, x, y, ...
    INT,    // "INT"   // 1343456
    STRING, // "foobar"

    // Operators
    ASSIGN,   // "="
    PLUS,     // "+"
    MINUS,    // "-"
    BANG,     // "!"
    ASTERISK, // "*"
    SLASH,    // "/"

    LT, // "<"
    GT, // ">"

    EQ,     // "=="
    NOT_EQ, // "!="

    // Delimiters
    COMMA,     // ","
    SEMICOLON, // ";"
    COLON,     // ":"

    LPAREN,   // "("
    RPAREN,   // ")"
    LBRACE,   // "{"
    RBRACE,   // "}"
    LBRACKET, // "["
    RBRACKET, // "]"
    // Keywords
    FUNCTION, // "FUNCTION"
    LET,      // "LET"
    TRUE_,    // "TRUE"
    FALSE_,   // "FALSE"
    IF,       // "IF"
    ELSE,     // "ELSE"
    RETURN,   // "RETURN"
};

const string TOKEN_TYPE_VALUE[] = {
    "ILLEGAL", "EOF",

    // Identifiers + literals
    "IDENT",  // add, foobar, x, y, ...,
    "INT",    // 1343456,
    "STRING", // "foobar"

    // Operators
    "ASSIGN",   // "="
    "PLUS",     // "+"
    "MINUS",    // "-"
    "BANG",     // "!"
    "ASTERISK", // "*"
    "SLASH",    // "/"

    "LT", // "<"
    "GT", // ">"

    "EQ",     // "=="
    "NOT_EQ", // "!="

    // Delimiters
    "COMMA",     // ","
    "SEMICOLON", // ";"
    "COLON",      //= ":"
    "LPAREN",    // "("
    "RPAREN",    // ")"
    "LBRACE",    // "{"
    "RBRACE",    // "}"
    "LBRACKET",  // "["
    "RBRACKET",  // "]"

    // Keywords
    "FUNCTION", // "FUNCTION"
    "LET",      // "LET"
    "TRUE_",    // "TRUE"
    "FALSE_",   // "FALSE"
    "IF",       // "IF"
    "ELSE",     // "ELSE"
    "RETURN",   // "RETURN"
};

} // namespace token
}
#endif /* TOKEN_CONSTANT_HPP */
