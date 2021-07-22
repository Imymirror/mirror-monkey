#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "Builtins.hpp"
#include "Environment.hpp"
#include "Function.hpp"
#include "HashLiteral.hpp"
#include "Identifier.hpp"
#include "IfExpression.hpp"
#include "Node.hpp"
#include "Object.hpp"
#include "Program.hpp"
#include <any>

using namespace std;

extern shared_ptr<Error> new_error(string format, ...);

class Evaluator {
  public:
    Evaluator() { init_builtins(); }

  public:
    shared_ptr<Object> eval(Node *node, Environment *env);
    shared_ptr<Object> eval_program(Program &program, Environment &env);
    shared_ptr<Object> eval_prefix_expression(string op, Object *right);
    shared_ptr<Object> eval_bang_operator_expression(Object *right);
    shared_ptr<Object> eval_minus_prefix_operator_expression(Object *right);
    shared_ptr<Object> eval_infix_expression(string op, Object *left,
                                             Object *right);
    shared_ptr<Object> eval_integer_infix_expression(string op, Object *left,
                                                     Object *right);
    shared_ptr<Object> native_bool_to_boolean_object(bool input);
    shared_ptr<Object> eval_if_expression(IfExpression *ie, Environment *env);

    bool is_truthy(Object *obj);

    shared_ptr<Object> eval_block_statement(BlockStatement *block,
                                            Environment *env);

    shared_ptr<Object> eval_identifier(Identifier *node, Environment *env);

    bool is_error(shared_ptr<Object> obj);

    vector<shared_ptr<Object>>
    eval_expressions(vector<unique_ptr<Expression>> &exps, Environment *env);

    shared_ptr<Object> apply_function(shared_ptr<Object> fn,
                                      vector<shared_ptr<Object>> args);

    shared_ptr<Environment>
    extend_function_env(Function *fn, vector<shared_ptr<Object>> args);

    shared_ptr<Object> unwrap_return_value(shared_ptr<Object> obj);

    shared_ptr<Object> eval_string_infix_expression(string op, Object *left,
                                                    Object *right);
    shared_ptr<Object> eval_index_expression(Object *left, Object *index);
	shared_ptr<Object> eval_array_index_expression(Object * left, Object * index);
	shared_ptr<Object> eval_hash_literal(HashLiteral* node, Environment* env);
	shared_ptr<Object> eval_hash_index_expression(Object *hash, Object *index); 
};

#endif /* EVALUATOR_HPP */
