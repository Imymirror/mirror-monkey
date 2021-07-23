#ifndef ERROR_HPP
#define ERROR_HPP

#include "Object.hpp"

using namespace std;
using namespace mirror;
using namespace object;

namespace mirror {
class Error : public Object {
  public:
    string m_message;

  public:
    OBJECT_TYPE type() { return OBJECT_TYPE::ERROR_OBJ; };
    string Inspect() { return "Error: " + m_message; };
};
}
#endif /* ERROR_HPP */
