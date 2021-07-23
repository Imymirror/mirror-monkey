add_library(ast INTERFACE)

add_subdirectory(basic)

# target_sources(ast
# 	PRIVATE
# 	PUBLIC
# 		${CMAKE_CURRENT_LIST_DIR}/Program.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/Identifier.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/LetStatement.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/ReturnStatement.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/IntegerLiteral.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/ExpressionStatement.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/InfixExpression.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/Boolean.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/PrefixExpression.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/StringLiteral.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/BlockStatement.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/ArrayLiteral.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/IndexExpression.hpp
# 		${CMAKE_CURRENT_LIST_DIR}/HashLiteral.hpp
		
# )

target_include_directories(ast
	INTERFACE	
		${CMAKE_CURRENT_LIST_DIR}
)

# target_link_libraries(ast
# PRIVATE
# 	token
# 	ast_basic
# )
