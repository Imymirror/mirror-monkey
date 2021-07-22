#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <memory>
#include <vector>

// #include "AstBasic.hpp"
#include "Statement.hpp"
#include "Expression.hpp"

using namespace std;

class Program : public Node
{
public:
	vector< unique_ptr<Statement>> m_statements;
public:
public:
	string token_literal() override;
	string to_string() override;
};

#endif /* PROGRAM_HPP */
