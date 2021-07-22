add_executable(mirror main.cpp)

add_subdirectory(common)
add_subdirectory(token)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(ast)
add_subdirectory(repl)
add_subdirectory(object)
add_subdirectory(evaluator)

target_link_libraries(mirror
  PRIVATE
  	common
  	object
	evaluator
	repl
  )
