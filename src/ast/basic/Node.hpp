#ifndef NODE_HPP
#define NODE_HPP

#include <string>

using namespace std;

// The base Node interface
class Node {
  public:
    virtual ~Node() {};

  public:
    virtual string token_literal() = 0;
    virtual string to_string() = 0;
};

#endif /* NODE_HPP */
