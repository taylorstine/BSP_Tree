CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(CAD)

include(build/CMakeLists.txt)

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${SDL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
)

#add_subdirectory(bsptree)
add_subdirectory(main)
add_subdirectory(math)
#add_subdirectory(glew)

