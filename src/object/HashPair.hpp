#ifndef HASHPAIR_HPP
#define HASHPAIR_HPP

#include "Object.hpp"

using namespace std;

namespace mirror {
class HashPair : public Object {
  public:
    std::shared_ptr<Object> m_key;
    std::shared_ptr<Object> m_value;

    HashPair(shared_ptr<Object> key, shared_ptr<Object> value)
        : m_key(key), m_value(value){};
};
}
#endif /* HASHPAIR_HPP */
