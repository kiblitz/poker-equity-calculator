project(ranker)

file(GLOB_RECURSE RANKER_SRC "*.cpp")

add_library(${PROJECT_NAME} ${RANKER_SRC})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)
