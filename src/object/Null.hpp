#ifndef NULL_HPP
#define NULL_HPP

#include "Object.hpp"

using namespace std;
using namespace mirror;
using namespace object;


namespace mirror {
class Null : public Object {

  public:
    OBJECT_TYPE type() { return OBJECT_TYPE::NULL_OBJ; };
    string Inspect() { return "null"; };
};
}
#endif /* NULL_HPP */
