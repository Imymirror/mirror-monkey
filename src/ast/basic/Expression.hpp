#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Node.hpp"

using namespace std;

namespace mirror
{
  // All expression nodes implement this
  class Expression : public Node
  {
  public:
    virtual ~Expression(){};

  public:
    virtual string token_literal() override { return ""; };
    virtual string to_string() override { return ""; };
  };
}
#endif /* EXPRESSION_HPP */
