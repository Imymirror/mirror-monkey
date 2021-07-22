
add_executable(lexer_test test.cpp)

# target_link_libraries(lexer_test catch_main)

add_test(
	NAME lexer_test 
	COMMAND $<TARGET_FILE:lexer_test> 
)
