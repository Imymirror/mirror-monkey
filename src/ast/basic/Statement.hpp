#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "Node.hpp"

// All statement nodes implement this
using namespace std;

namespace mirror
{
  class Statement : public Node
  {
  public:
    virtual ~Statement(){};

  public:
    virtual string token_literal() = 0;
    virtual string to_string() = 0;
  };
}
#endif /* STATEMENT_HPP */
