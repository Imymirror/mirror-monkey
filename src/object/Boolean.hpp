#ifndef OBJECT_BOOLEAN_HPP
#define OBJECT_BOOLEAN_HPP

#include "Hashable.hpp"
#include "Object.hpp"
#include <memory>

using namespace std;
namespace mirror {
namespace object {
class Boolean : public Object, public Hashable {
  public:
    bool m_value;

  public:
    Boolean(bool b) : m_value(b){};
    bool operator==(const Boolean &o1) {
        return o1.m_value > m_value;
    };

  public:
    OBJECT_TYPE type() override { return OBJECT_TYPE::BOOLEAN_OBJ; };
    string Inspect() override { return m_value ? "true" : "false"; };

  public:
    HashKey hash_key() override {
         HashKey hk(type(), m_value ? 1 : 0);
		 return hk;
    };
};

} // namespace object
}
#endif /* OBJECT_BOOLEAN_HPP */
