add_library(token "")

target_sources(token
  PRIVATE
    Token.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Token.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Constant.hpp
  )

target_include_directories(token
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )


target_link_libraries(token
PRIVATE
  common
)
