#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <memory>
#include <vector>

#include "Expression.hpp"
#include "Statement.hpp"

using namespace std;
namespace mirror
{
    class Program : public Node
    {
    public:
        vector<unique_ptr<Statement>> m_statements;

    public:
    public:
        string token_literal() override
        {
            if (m_statements.size() > 0)
            {
                return m_statements[0]->token_literal();
            }
            else
            {
                return "";
            }
        };
        string to_string() override
        {
            string ret = "";

            for (int i = 0; i < m_statements.size(); i++)
            {
                ret += m_statements[i]->to_string();
            }

            return ret;
        };
    };
} // namespace mirror
#endif /* PROGRAM_HPP */
