#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include "Object.hpp"
#include "../object/Constant.hpp"
#include <functional>
#include <iostream>


using namespace std;
using namespace object;

class Any_Function {
  public:
    virtual ~Any_Function() {}
};

template <typename Ret, typename... Args>
class FunctionCommon : public Any_Function, public std::function<Ret(Args...)> {
  public:
    template <typename T>
    FunctionCommon(T &f) : std::function<Ret(Args...)>(f) {}
};

class Builtin : public Object {
  public:
    shared_ptr<Any_Function> m_fn;

    Builtin(shared_ptr<Any_Function> f):m_fn(f){};

  public:
    OBJECT_TYPE type() { return OBJECT_TYPE::BUILTIN_OBJ; };
    string Inspect() { return "builtin function"; };
};

#endif /* BUILTIN_HPP */
