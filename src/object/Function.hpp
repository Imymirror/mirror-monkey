#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Environment.hpp"
#include "BlockStatement.hpp"
#include <memory>
#include <string>
#include <vector>
#include "Constant.hpp"

using namespace object;

using namespace std;

class Function : public Object {
  public:
    using t_params = shared_ptr<vector<unique_ptr<Identifier>>>;
    t_params m_parameters;
    shared_ptr<BlockStatement> m_body;
    shared_ptr<Environment> m_env;

    Function(t_params params, shared_ptr<BlockStatement> block, Environment *env) {

        m_parameters = params;
        m_body = block;
		m_env = env->get_shared();
    };

  public:
    OBJECT_TYPE type() { return OBJECT_TYPE::FUNCTION_OBJ; };
    string Inspect() {
        string ret = "";
        ret += "fn";
        ret += "(";



        // for (int i = 0; i < m_parameters.size(); i++) {
        //     auto &ident = *m_parameters[i];
        //     ret += ident.to_string();

        //     if (i != m_parameters.size() - 1) {
        //         ret += ",";
        //     }
        // }

        ret += ") {\n";

        m_body->to_string();

        ret += "\n}";

        return ret;
    };
};

#endif /* FUNCTION_HPP */
