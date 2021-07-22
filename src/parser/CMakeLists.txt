add_library(parser "")

target_sources(parser
	PRIVATE
		Parser.cpp
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/Parser.hpp
)

target_include_directories(parser
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(parser
PRIVATE
	lexer
	token
	ast_basic
	ast
	common
)
