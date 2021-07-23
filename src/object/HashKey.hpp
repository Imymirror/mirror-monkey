#ifndef HASHKEY_HPP
#define HASHKEY_HPP

#include "./Constant.hpp"
#include "Object.hpp"
#include <memory>
namespace mirror {
class HashKey : public Object {
  public:
    object::OBJECT_TYPE m_type;
    uint64_t m_value;

  public:
    HashKey() = default;
    HashKey(std::shared_ptr<HashKey> &other) { *this = other; };
    HashKey(object::OBJECT_TYPE type, uint64_t value)
        : m_type(type), m_value(value){};

    bool operator<(const HashKey &c1) const { return c1.m_value < m_value; };
    bool operator==(const HashKey &other) const {
        return other.m_value == m_value && m_type == other.m_type;
    };
    bool operator!=(const HashKey &other) const {
        return other.m_value != m_value || m_type != other.m_type;
    };
};
}
#endif /* HASHKEY_HPP */
