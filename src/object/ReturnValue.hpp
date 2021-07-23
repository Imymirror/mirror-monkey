#ifndef RETURNVALUE_HPP
#define RETURNVALUE_HPP

#include "Object.hpp"

using namespace mirror;
using namespace std;
using namespace object;

namespace mirror {
class ReturnValue : public Object {
  public:
    shared_ptr<Object> m_value;
    ReturnValue(shared_ptr<Object> value) : m_value(value){};

  public:
    OBJECT_TYPE type() { return OBJECT_TYPE::RETURN_VALUE_OBJ; };
    string Inspect() { return m_value->Inspect(); };
};
}
#endif /* RETURNVALUE_HPP */
