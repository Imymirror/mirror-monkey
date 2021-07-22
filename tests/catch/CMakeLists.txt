add_library(catch main.cpp)

target_include_directories(catch
	PUBLIC	
		${CMAKE_CURRENT_LIST_DIR}
)
