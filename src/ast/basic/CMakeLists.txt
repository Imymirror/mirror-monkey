add_library(ast_basic INTERFACE)

# target_sources(ast_basic 
# 	PRIVATE	
# 	# AstBasic.cpp
# 	PUBLIC
# 		# ${CMAKE_CURRENT_LIST_DIR}/AstBasic.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/Node.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/Statement.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/Expression.hpp
# )

target_include_directories(ast_basic 
	INTERFACE	
		${CMAKE_CURRENT_LIST_DIR}
)
