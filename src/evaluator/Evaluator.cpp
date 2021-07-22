#include "Evaluator.hpp"
#include "../ast/Boolean.hpp"
#include "../object/Boolean.hpp"
#include "Array.hpp"
#include "ArrayLiteral.hpp"
#include "BlockStatement.hpp"
#include "Builtins.hpp"
#include "CallExpression.hpp"
#include "Constant.hpp"
#include "Environment.hpp"
#include "Error.hpp"
#include "Expression.hpp"
#include "ExpressionStatement.hpp"
#include "Function.hpp"
#include "FunctionLiteral.hpp"
#include "Hash.hpp"
#include "HashKey.hpp"
#include "HashLiteral.hpp"
#include "HashPair.hpp"
#include "Hashable.hpp"
#include "IfExpression.hpp"
#include "IndexExpression.hpp"
#include "InfixExpression.hpp"
#include "Integer.hpp"
#include "IntegerLiteral.hpp"
#include "LetStatement.hpp"
#include "Null.hpp"
#include "Object.hpp"
#include "PrefixExpression.hpp"
#include "Program.hpp"
#include "ReturnStatement.hpp"
#include "ReturnValue.hpp"
#include "String.hpp"
#include "StringLiteral.hpp"
#include <algorithm>
#include <any>
#include <cstddef>
#include <iostream>
#include <memory>

using namespace object;
using namespace std;

using t_object_Boolean = object::Boolean;
using t_ast_Boolean = ast::Boolean;

shared_ptr<Error> new_error(string format, ...) {
    auto ret = make_shared<Error>();

    va_list argptr;
    va_start(argptr, format.size());

    string str = "";

    for (int i = 0; i < format.size(); i++) {
        auto c = format[i];
        switch (c) {
        case 'c': {
            char c = (char)va_arg(argptr, int);
            str += c;
            break;
        }
        case 'd': {
            int d = va_arg(argptr, int);
            str += to_string(d);
            break;
        }
        case 's': {
            char *s = va_arg(argptr, char *);
            str += s;
            break;
        }
        }
    }

    va_end(argptr);


    ret->m_message = str;

    return ret;
}

shared_ptr<Object> Evaluator::eval(Node *node, Environment *env) {

    if (auto cast_node = dynamic_cast<Program *>(node)) {

        return eval_program(*cast_node, *env);
    }

    if (auto cast_node = dynamic_cast<BlockStatement *>(node)) {
        return eval_block_statement(cast_node, env);
    }

    if (auto cast_node = dynamic_cast<ExpressionStatement *>(node)) {
        return eval(cast_node->m_expression.get(), env);
    }

    if (auto cast_node = dynamic_cast<ReturnStatement *>(node)) {
        auto val = eval(cast_node->m_return_value.get(), env);
        if (is_error(val)) {
            return val;
        }

        return make_shared<ReturnValue>(val);
    }

    if (auto cast_node = dynamic_cast<LetStatement *>(node)) {
        auto val = eval(cast_node->m_value.get(), env);
        if (is_error(val)) {
            return val;
        }


        env->set(cast_node->m_name->m_value, val);
    }

    if (auto cast_node = dynamic_cast<IntegerLiteral *>(node)) {
        return make_shared<Integer>(cast_node->m_value);
    }

    if (auto cast_node = dynamic_cast<t_ast_Boolean *>(node)) {
        return native_bool_to_boolean_object(cast_node->m_value);
    }

    if (auto cast_node = dynamic_cast<StringLiteral *>(node)) {
        return make_shared<String>(cast_node->m_value);
    }

    if (auto cast_node = dynamic_cast<PrefixExpression *>(node)) {
        auto right = eval(cast_node->m_right.get(), env);
        if (is_error(right)) {
            return right;
        }

        return eval_prefix_expression(cast_node->m_operator, right.get());
    }

    if (auto cast_node = dynamic_cast<InfixExpression *>(node)) {
        auto left = eval(cast_node->m_left.get(), env);
        if (is_error(left)) {
            return left;
        }

        auto right = eval(cast_node->m_right.get(), env);
        if (is_error(right)) {
            return right;
        }

        return eval_infix_expression(cast_node->m_operator, left.get(),
                                     right.get());
    }

    if (auto cast_node = dynamic_cast<IfExpression *>(node)) {

        return eval_if_expression(cast_node, env);
    }

    if (auto cast_node = dynamic_cast<Identifier *>(node)) {
        return eval_identifier(cast_node, env);
    }

    if (auto cast_node = dynamic_cast<FunctionLiteral *>(node)) {
        auto params = cast_node->m_parameters;
        auto body = cast_node->m_body;
        return make_shared<Function>(params, body, env);
    }

    if (auto cast_node = dynamic_cast<CallExpression *>(node)) {
        auto funciton = eval(cast_node->m_function.get(), env);
        if (is_error(funciton)) {
            return funciton;
        }

        auto args = eval_expressions(*cast_node->m_arguments, env);
        if (args.size() == 1 && is_error(args[0])) {
            return args[0];
        }

        return apply_function(funciton, args);
    }

    if (auto cast_node = dynamic_cast<ArrayLiteral *>(node)) {

        auto elements = eval_expressions(*(cast_node->m_elements), env);

        if (elements.size() == 1 && is_error(elements[0])) {
            return elements[0];
        }

        return make_shared<Array>(elements);
    }

    if (auto cast_node = dynamic_cast<IndexExpression *>(node)) {
        auto left = eval(cast_node->m_left.get(), env);
        if (is_error(left)) {
            return left;
        }

        auto index = eval(cast_node->m_index.get(), env);
        if (is_error(index)) {
            return index;
        }

        return eval_index_expression(left.get(), index.get());
    }

    if (auto cast_node = dynamic_cast<HashLiteral *>(node)) {

        return eval_hash_literal(cast_node, env);
    }

    return nullptr;
}

shared_ptr<Object> Evaluator::eval_program(Program &program, Environment &env) {
    auto result = make_shared<Object>();


    for (int i = 0; i < program.m_statements.size(); i++) {

        auto statement = (program.m_statements[i]).get();
        result = eval(statement, &env);

        if (auto cast_result = dynamic_cast<ReturnValue *>(result.get())) {
            return cast_result->m_value;
        }

        if (auto cast_result = dynamic_cast<Error *>(result.get())) {
            return result;
        }
    }

    return result;
}

shared_ptr<Object> Evaluator::eval_prefix_expression(string op, Object *right) {
    if (op == "!") {
        return eval_bang_operator_expression(right);
    }

    if (op == "-") {
        return eval_minus_prefix_operator_expression(right);
    }

    return new_error("sss", "unknown operator: ", op.c_str(),
                     Object::object_type_value(right->type()).c_str());
}

shared_ptr<Object> Evaluator::eval_bang_operator_expression(Object *right) {
    if (auto cast_node = dynamic_cast<object::Boolean *>(right)) {
        return make_shared<object::Boolean>(!cast_node->m_value);
    }

    if (auto cast_node = dynamic_cast<Null *>(right)) {
        return make_shared<object::Boolean>(true);
    }

    return make_shared<object::Boolean>(false);
}

shared_ptr<Object>
Evaluator::eval_minus_prefix_operator_expression(Object *right) {
    if (right->type() != OBJECT_TYPE::INTEGER_OBJ) {
        return new_error("ss", "unknown operator: -",
                         Object::object_type_value(right->type()).c_str());
    }
    auto value = static_cast<Integer *>(right)->m_value;
    return make_shared<Integer>(-value);
}

shared_ptr<Object> Evaluator::eval_infix_expression(string op, Object *left,
                                                    Object *right) {
    if (left->type() == OBJECT_TYPE::INTEGER_OBJ &&
        right->type() == OBJECT_TYPE::INTEGER_OBJ) {
        return eval_integer_infix_expression(op, left, right);
    }

    if (left->type() == OBJECT_TYPE::STRING_OBJ &&
        right->type() == OBJECT_TYPE::STRING_OBJ) {
        return eval_string_infix_expression(op, left, right);
    }

    if (op == "==") {

        if (auto cast_left = static_cast<object::Boolean *>(left)) {

            auto cast_right = static_cast<object::Boolean *>(right);

            return native_bool_to_boolean_object(cast_left->m_value ==
                                                 cast_right->m_value);
        }
    }

    if (op == "!=") {
        return native_bool_to_boolean_object(left != right);
    }

    if (left->type() != right->type()) {

        auto err_str = Object::object_type_value(left->type()) + " " + op +
                       " " + Object::object_type_value(right->type());


        return new_error("ss", "type mismatch: ", err_str.c_str());
    }

    auto err_str = Object::object_type_value(left->type()) + " " + op + " " +
                   Object::object_type_value(right->type());
    return new_error("ss", "unknown operator: ", err_str.c_str());
}

shared_ptr<Object> Evaluator::eval_integer_infix_expression(string op,
                                                            Object *left,
                                                            Object *right) {
    auto left_value = static_cast<Integer *>(left)->m_value;
    auto right_value = static_cast<Integer *>(right)->m_value;

    if (op == "+") {
        return make_shared<Integer>(left_value + right_value);
    }
    if (op == "-") {
        return make_shared<Integer>(left_value - right_value);
    }

    if (op == "*") {
        return make_shared<Integer>(left_value * right_value);
    }

    if (op == "/") {
        return make_shared<Integer>(left_value / right_value);
    }

    if (op == "<") {
        return native_bool_to_boolean_object(left_value < right_value);
    }

    if (op == ">") {
        return native_bool_to_boolean_object(left_value > right_value);
    }

    if (op == "==") {
        return native_bool_to_boolean_object(left_value == right_value);
    }

    if (op == "!=") {
        return native_bool_to_boolean_object(left_value != right_value);
    }

    auto err_str = Object::object_type_value(left->type()) + " " + op + " " +
                   Object::object_type_value(right->type());

    return new_error("ss", "unknown operator: ", err_str.c_str());
}

shared_ptr<Object> Evaluator::native_bool_to_boolean_object(bool input) {
    return make_shared<object::Boolean>(input);
}

shared_ptr<Object> Evaluator::eval_if_expression(IfExpression *ie,
                                                 Environment *env) {

    auto condition = eval(ie->m_condition.get(), env);
    if (is_error(condition)) {
        return condition;
    }

    if (is_truthy(condition.get())) {
        return eval(ie->m_consequence.get(), env);
    } else if (ie->m_alternative != nullptr) {
        return eval(ie->m_alternative.get(), env);
    } else {
		return shared_ptr<Object>(make_shared<Null>());
    }
}

bool Evaluator::is_error(shared_ptr<Object> obj) {
    if (obj != nullptr) {
        return obj->type() == OBJECT_TYPE::ERROR_OBJ;
    }

    return false;
}

bool Evaluator::is_truthy(Object *obj) {

    if (auto cast_obj = static_cast<object::Boolean *>(obj)) {
        return cast_obj->m_value ? true : false;
    }

    if (auto cast_node = dynamic_cast<Null *>(obj)) {
        return false;
    }

    return true;
}

shared_ptr<Object> Evaluator::eval_block_statement(BlockStatement *block,
                                                   Environment *env) {
    auto result = make_shared<Object>();

    for (int i = 0; i < block->m_statements.size(); i++) {
        auto statement = block->m_statements[i].get();
        result = eval(statement, env);

        if (result) {
            if (result->type() == OBJECT_TYPE::RETURN_VALUE_OBJ ||
                result->type() == OBJECT_TYPE::ERROR_OBJ) {
                return result;
            }
        }
    }

    return result;
}

shared_ptr<Object> Evaluator::eval_identifier(Identifier *node,
                                              Environment *env) {

    if (auto val = env->get(node->m_value)) {
        return val;
    }

    auto it = builtins.find(node->m_value);
    if (it != builtins.end()) {
        return it->second;
    }

    return new_error("ss", "identifier not found: ", node->m_value.c_str());
}

vector<shared_ptr<Object>>
Evaluator::eval_expressions(vector<unique_ptr<Expression>> &exps,
                            Environment *env) {
    vector<shared_ptr<Object>> result;

    for (int i = 0; i < exps.size(); i++) {
        auto exp = exps[i].get();
        auto evaluated = eval(exp, env);
        if (is_error(evaluated)) {
            result.push_back(evaluated);
            return result;
        }

        result.push_back(evaluated);
    }

    return result;
}

shared_ptr<Environment>
Evaluator::extend_function_env(Function *fn, vector<shared_ptr<Object>> args) {
    auto env = n_env::new_enclosed_environment(fn->m_env);

    auto &params = *(fn->m_parameters);
    for (int i = 0; i < params.size(); i++) {
        auto param = *params[i];
        env->set(param.m_value, args[i]);
    }

    return env;
}

shared_ptr<Object> Evaluator::unwrap_return_value(shared_ptr<Object> obj) {

    auto return_val = dynamic_cast<ReturnValue *>(obj.get());
    if (return_val) {
        return return_val->m_value;
    }

    return obj;
}

shared_ptr<Object> Evaluator::apply_function(shared_ptr<Object> fn,
                                             vector<shared_ptr<Object>> args) {
    if (auto function = dynamic_cast<Function *>(fn.get())) {


        auto extendedEnv = extend_function_env(function, args);
        auto evaluated = eval(function->m_body.get(), extendedEnv.get());

        return unwrap_return_value(evaluated);
    }

    if (auto builtin = dynamic_cast<Builtin *>(fn.get())) {
        using len_function =
            FunctionCommon<shared_ptr<Object>, vector<shared_ptr<Object>>>;
        if (auto &cast_builtin_fn =
                dynamic_cast<len_function &>(*builtin->m_fn)) {
            auto ret = cast_builtin_fn(args);

            return ret;
        }
    }

    return new_error("ss", "not a function: ",
                     Object::object_type_value(fn->type()).c_str());
}

shared_ptr<Object> Evaluator::eval_string_infix_expression(string op,
                                                           Object *left,
                                                           Object *right) {

    if (op != "+") {
        auto err_str = Object::object_type_value(left->type()) + " " + op +
                       " " + Object::object_type_value(right->type());
        return new_error("ss", "unknown operator: ", err_str.c_str());
    }

    auto left_val = static_cast<String *>(left)->m_value;
    auto right_val = static_cast<String *>(right)->m_value;

    return make_shared<String>(left_val + right_val);
}

shared_ptr<Object> Evaluator::eval_index_expression(Object *left,
                                                    Object *index) {


    if (left->type() == OBJECT_TYPE::ARRAY_OBJ &&
        index->type() == OBJECT_TYPE::INTEGER_OBJ) {
        ;
        return eval_array_index_expression(left, index);
    }

    if (left->type() == OBJECT_TYPE::HASH_OBJ) {
        return eval_hash_index_expression(left, index);
    }


    return new_error("ss", "index operator not supported: ",
                     Object::object_type_value(left->type()).c_str());
}

shared_ptr<Object> Evaluator::eval_array_index_expression(Object *left,
                                                          Object *index) {
    auto array_object = static_cast<Array *>(left);
    auto idx = static_cast<Integer *>(index)->m_value;
    auto max = int64_t(array_object->m_elements.size() - 1);
    if (idx < 0 || idx > max)
		return shared_ptr<Object>(make_shared<Null>());

    return array_object->m_elements[idx];
}

shared_ptr<Object> Evaluator::eval_hash_literal(HashLiteral *node,
                                                Environment *env) {

    auto pairs = make_unique<map<HashKey, shared_ptr<HashPair>>>();

    auto &m_pairs = (*node->m_pairs);
    auto it = m_pairs.begin();

    while (it != m_pairs.end()) {
        auto key = eval(it->first.get(), env);
        if (is_error(key)) {
            return key;
        }

        auto hash_key = dynamic_cast<Hashable *>(key.get());
        if (!hash_key) {
			new_error("ss", "unusable as hash key: ", Object::object_type_value(key->type()).c_str());
        }

        auto value = eval(it->second.get(), env);
        if (is_error(value)) {
            return value;
        }

        auto hashed = hash_key->hash_key();

        (*pairs)[hashed] = make_shared<HashPair>(key, value);

        it++;
    }

    auto itp = (*pairs).begin();
    while (itp != (*pairs).end()) {
        auto ttt = (itp->second);

        auto value = ttt->m_value;
        auto key = ttt->m_key;

        itp++;
    }

    return make_shared<Hash>(std::move(pairs));
}

shared_ptr<Object> Evaluator::eval_hash_index_expression(Object *hash,
                                                         Object *index) {
    auto hash_object = static_cast<Hash *>(hash);
    auto key = dynamic_cast<Hashable *>(index);
    if (!key) {
        return new_error("ss", "unusable as hash key: ",
                         Object::object_type_value(index->type()).c_str());
    }

    auto pair = (*hash_object->m_pairs)[key->hash_key()];
    if (!pair) {
        return shared_ptr<Object>(make_shared<Null>());
    }

    return pair->m_value;
}