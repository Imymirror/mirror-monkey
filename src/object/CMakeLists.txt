add_library(object "")

target_sources(object 
  PRIVATE
    Object.cpp
    Environment.cpp
    Function.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Object.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Environment.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Function.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Constant.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Integer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Error.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Boolean.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ReturnValue.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Array.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Null.hpp
    ${CMAKE_CURRENT_LIST_DIR}/HashPair.hpp
    ${CMAKE_CURRENT_LIST_DIR}/HashKey.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Hashable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Hash.hpp
	
  )

target_include_directories(object 
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(object 
PRIVATE
 common
 ast
 ast_basic
 token 
)
