#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "./Constant.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace mirror;
using namespace object;
using namespace std;

namespace mirror {
class Object {
  public:
    virtual OBJECT_TYPE type() { return OBJECT_TYPE::NULL_OBJ; };
    virtual string Inspect() { return ""; };
    virtual ~Object(){};
    static string object_type_value(OBJECT_TYPE type) {
        auto index = static_cast<std::underlying_type<OBJECT_TYPE>::type>(type);
        return OBJECT_TYPE_VALUE[index];
    }
};

}
#endif /* OBJECT_HPP */
