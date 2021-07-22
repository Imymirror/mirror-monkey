#ifndef HASH_HPP
#define HASH_HPP

#include "HashKey.hpp"
#include "HashPair.hpp"
#include "Object.hpp"
#include <map>
#include <memory>

using namespace std;
using namespace object;

class Hash : public Object {
  public:
    unique_ptr<map<HashKey, shared_ptr<HashPair>>> m_pairs;

    Hash(unique_ptr<map<HashKey, shared_ptr<HashPair>>> p) : m_pairs(std::move(p)){
		auto it = m_pairs->begin();
	};
  public:
    OBJECT_TYPE type() { return OBJECT_TYPE::HASH_OBJ; };
    string Inspect() {

        string ret = "";
        ret += "{";

        auto &pairs = *m_pairs;
        auto it = pairs.begin();
        while (it != pairs.end()) {
            auto key = it->first;
            auto value = it->second.get();
            ret += key.Inspect() + ":" + value->Inspect();
            it++;
        }

        ret += "}";

        return ret;
    };
};

#endif /* HASH_HPP */
