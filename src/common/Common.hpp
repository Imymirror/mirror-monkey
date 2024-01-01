#ifndef COMMON_HPP
#define COMMON_HPP

#include <any>
#include <iostream>
#include <type_traits>

using namespace std;

namespace mirror
{
    namespace common
    {

        template <typename Enumeration>
        auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
        {
            return static_cast<typename std::underlying_type<Enumeration>::type>(value);
        }

    } // namespace common
}
#endif /* COMMON_HPP */
