#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "../object/Constant.hpp"
#include "Array.hpp"
#include "Builtin.hpp"
#include "Error.hpp"
#include "Integer.hpp"
#include "Null.hpp"
#include "Object.hpp"
#include "String.hpp"
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <string>

using namespace std;
using namespace mirror;

// namespace mirror {
extern map<string, shared_ptr<Builtin>> builtins;
shared_ptr<Error> new_error(string format, ...);

inline void init_len() {

    using len_function =
        FunctionCommon<shared_ptr<Object>, vector<shared_ptr<Object>>>;

    auto fn = make_shared<len_function>(*([](vector<shared_ptr<Object>> args) {
        if (args.size() != 1) {
            return shared_ptr<Object>(
                new_error("sds", R"(wrong number of arguments. got=)",
                          args.size(), ", want=1"));
        }

        if (auto cast_note = dynamic_cast<String *>(args[0].get())) {
            return shared_ptr<Object>(
                make_shared<Integer>(int64_t(cast_note->m_value.size())));
        }

        if (auto cast_note = dynamic_cast<Array *>(args[0].get())) {
            return shared_ptr<Object>(
                make_shared<Integer>(int64_t(cast_note->m_elements.size())));
        }

        return shared_ptr<Object>(
            new_error("ss", "argument to `len` not supported, got ",
                      Object::object_type_value(args[0]->type()).c_str()));
    }));

    builtins["len"] = make_shared<Builtin>(fn);
}

inline void init_first() {
    using len_function =
        FunctionCommon<shared_ptr<Object>, vector<shared_ptr<Object>>>;

    auto fn = make_shared<len_function>(*([](vector<shared_ptr<Object>> args) {


        if (args.size() != 1) {
            return shared_ptr<Object>(
                new_error("sds", R"(wrong number of arguments. got=)",
                          args.size(), ", want=1"));
        }

        if (args[0]->type() != object::OBJECT_TYPE::ARRAY_OBJ) {

            auto type_value = Object::object_type_value(args[0]->type());
            return shared_ptr<Object>(
                new_error("ss", R"(argument to `first` must be ARRAY, got )",
                          type_value.c_str()));
        }

        auto arr = static_cast<Array *>(args[0].get());
        if (arr->m_elements.size() > 0) {
            return arr->m_elements[0];
        }

        return shared_ptr<Object>(make_shared<Null>());
    }));

    builtins["first"] = make_shared<Builtin>(fn);
}

inline void init_last() {

    using len_function =
        FunctionCommon<shared_ptr<Object>, vector<shared_ptr<Object>>>;

    auto fn = make_shared<len_function>(*([](vector<shared_ptr<Object>> args) {
        if (args.size() != 1) {
            return shared_ptr<Object>(
                new_error("sds", R"(wrong number of arguments. got=)",
                          args.size(), ", want=1"));
        }
        if (args[0]->type() != object::OBJECT_TYPE::ARRAY_OBJ) {
            auto type_value = Object::object_type_value(args[0]->type());
            return shared_ptr<Object>(
                new_error("ss", R"(argument to `last` must be ARRAY, got )",
                          type_value.c_str()));
        }

        auto arr = static_cast<Array *>(args[0].get());
        auto length = arr->m_elements.size();
        if (length > 0) {
            return arr->m_elements[length - 1];
        }

        return shared_ptr<Object>(make_shared<Null>());
    }));

    builtins["last"] = make_shared<Builtin>(fn);
}

inline void init_rest() {
    using len_function =
        FunctionCommon<shared_ptr<Object>, vector<shared_ptr<Object>>>;

    auto fn = make_shared<len_function>(*([](vector<shared_ptr<Object>> args) {
        if (args.size() != 1) {
            return shared_ptr<Object>(
                new_error("sds", R"(wrong number of arguments. got=)",
                          args.size(), ", want=1"));
        }
        if (args[0]->type() != object::OBJECT_TYPE::ARRAY_OBJ) {
            auto type_value = Object::object_type_value(args[0]->type());
            return shared_ptr<Object>(
                new_error("ss", R"(argument to `rest` must be ARRAY, got )",
                          type_value.c_str()));
        }

        auto arr = static_cast<Array *>(args[0].get());
        auto length = arr->m_elements.size();
        if (length > 0) {
            vector<shared_ptr<Object>> new_elements;
            for (int i = 0; i < length; i++) {
                if (i != 0) {
                    new_elements.push_back(arr->m_elements[i]);
                }
            }

            return shared_ptr<Object>((make_shared<Array>(new_elements)));
        }

        return shared_ptr<Object>(make_shared<Null>());
    }));

    builtins["rest"] = make_shared<Builtin>(fn);
}

inline void init_push() {
    using len_function =
        FunctionCommon<shared_ptr<Object>, vector<shared_ptr<Object>>>;

    auto fn = make_shared<len_function>(*([](vector<shared_ptr<Object>> args) {
    
        if (args.size() != 2) {
            return shared_ptr<Object>(
                new_error("sds", R"(wrong number of arguments. got=)",
                          args.size(), ", want=2"));
        }

        if (args[0]->type() != object::OBJECT_TYPE::ARRAY_OBJ) {
            auto type_value = Object::object_type_value(args[0]->type());
            return shared_ptr<Object>(
                new_error("ss", R"(argument to `push` must be ARRAY, got )",
                          type_value.c_str()));
        }

        auto arr = static_cast<Array *>(args[0].get());
        auto length = arr->m_elements.size();
        vector<shared_ptr<Object>> new_elements;

        

        for (int i = 0; i < length; i++) {
            new_elements.push_back(arr->m_elements[i]);
        }
    

        new_elements.push_back(shared_ptr<Object>(args[1])) ;

        return shared_ptr<Object>(make_shared<Array>(new_elements));
    }));

    builtins["push"] = make_shared<Builtin>(fn);
}

inline void init_puts() {
    using len_function =
        FunctionCommon<shared_ptr<Object>, vector<shared_ptr<Object>>>;

    auto fn = make_shared<len_function>(*([](vector<shared_ptr<Object>> args) {
        for (int i = 0; i < args.size(); i++) {
            cout << args[i]->Inspect() << endl;
        }

        return shared_ptr<Object>(make_shared<Null>());
    }));

    builtins["puts"] = make_shared<Builtin>(fn);
}

inline void init_builtins() {

    init_len();
    init_first();
    init_last();
    init_rest();
    init_push();
    init_puts();
}
// }
#endif /* BUILTINS_HPP */
