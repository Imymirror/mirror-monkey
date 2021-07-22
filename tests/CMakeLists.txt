
# add_subdirectory(helloworld)
# target_link_libraries(cpp_test catch_main)

add_subdirectory(catch)

add_executable(test_lexer Test_Lexer.cpp)
add_test( NAME test_lexer COMMAND $<TARGET_FILE:test_lexer>)
target_link_libraries(test_lexer PRIVATE token lexer catch)

add_executable(test_ast Test_AST.cpp)
add_test( NAME test_ast COMMAND $<TARGET_FILE:test_ast>)
target_link_libraries(test_ast PRIVATE token lexer ast_basic ast catch)

add_executable(test_object Test_Object.cpp)
add_test( NAME test_object COMMAND $<TARGET_FILE:test_object>)
target_link_libraries(test_object PRIVATE  token lexer ast_basic ast parser catch object evaluator)


add_executable(test_parser Test_Parser.cpp)
add_test( NAME test_parser COMMAND $<TARGET_FILE:test_parser>)
target_link_libraries(test_parser PRIVATE  token lexer ast_basic ast parser catch)

add_executable(test_evaluator Test_Evaluator.cpp)
add_test( NAME test_evaluator COMMAND $<TARGET_FILE:test_evaluator>)
target_link_libraries(test_evaluator PRIVATE  token lexer ast_basic ast parser catch object evaluator)