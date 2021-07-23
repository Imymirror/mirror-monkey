#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "Hashable.hpp"
#include "Object.hpp"
#include <memory>

using namespace std;
using namespace mirror;
using namespace object;

namespace mirror {
class Integer : public Object, public Hashable {
  public:
    int64_t m_value;

  public:
    Integer(int64_t value) : m_value(value){};

  public:
    OBJECT_TYPE type() override{ return OBJECT_TYPE::INTEGER_OBJ; };
    string Inspect() override{ return to_string(m_value); };

  public:
    HashKey hash_key() override {
         HashKey hk(type(), uint64_t(m_value));
		 return hk;
    };
};
}
#endif /* INTEGER_HPP */
