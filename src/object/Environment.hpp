#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <memory>
#include <string>

#include "Object.hpp"

class Object;

using namespace std;

class Environment : public enable_shared_from_this<Environment> {
  public:
    map<string, shared_ptr<Object>> m_store;
    shared_ptr<Environment> m_outer;

  public:
    shared_ptr<Environment> get_shared() { return shared_from_this(); }
    shared_ptr<Object> set(string name, shared_ptr<Object> val) {
        m_store[name] = val;
        return val;
    };
    shared_ptr<Object> get(string name) {
        shared_ptr<Object> ret;
        auto it = m_store.find(name);
        if (it != m_store.end()) {
     
            ret = it->second;
        } else {
            
            if (m_outer)
                ret = m_outer->get(name);
        }
        return ret;
    };
};

namespace n_env {

inline shared_ptr<Environment> new_environment() {
    return make_shared<Environment>();
}

inline shared_ptr<Environment>
new_enclosed_environment(shared_ptr<Environment> outer) {
    shared_ptr<Environment> env = new_environment();

    env->m_outer = outer;

    return env;
}
} // namespace n_env

#endif /* ENVIRONMENT_HPP */
