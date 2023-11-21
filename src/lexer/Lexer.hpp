#ifndef LEXER_HPP
#define LEXER_HPP

#include <memory>
#include <vector>
#include "Token.hpp"

using namespace std;

namespace mirror
{
	class Lexer
	{
	private:
		vector<char> m_input;

		int m_position = 0;		// current position in input (points to current char)
		int m_readPosition = 0; // current reading position in input (after current char)
		char m_ch = 0;			// current char under examination
	public:
		Lexer() = default;
		Lexer(std::string &str);
		~Lexer();

	public:
		void read_char(); // read one char , store in ch
		char peed_char();
		void skip_whitespace();

		unique_ptr<Token> next_token();
		string capture_char(int number = 1);

		bool is_letter(char ch);
		bool is_digit(char ch);
		string read_identifier();
		string read_number();
		string read_string();
	};
}
#endif /* LEXER_HPP */
