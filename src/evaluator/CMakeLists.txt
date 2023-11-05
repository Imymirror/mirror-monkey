add_library(evaluator "")

target_link_libraries(evaluator 
PRIVATE
	ast_basic
	ast
	token
	object
	common
)

target_sources(evaluator 
  PRIVATE
    Evaluator.cpp
    Builtins.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Evaluator.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Builtins.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Builtin.hpp
  )

target_include_directories(evaluator 
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
