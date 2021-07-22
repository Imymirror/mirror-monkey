#ifndef HASHABLE_HPP
#define HASHABLE_HPP

#include "HashKey.hpp"
#include <memory>

class Hashable {
	public:
    virtual HashKey hash_key()=0;
};

#endif /* HASHABLE_HPP */
