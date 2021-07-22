add_library(lexer "")

target_link_libraries(lexer
PRIVATE
  token
)

target_sources(lexer
  PRIVATE
    Lexer.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Lexer.hpp
  )

target_include_directories(lexer
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
