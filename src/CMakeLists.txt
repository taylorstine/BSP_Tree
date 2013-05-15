CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(CAD)

include(build/CMakeLists.txt)

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${SDL_INCLUDE_DIR}
  ${SDLTTF_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIRS}
)

add_subdirectory(bsptree)
add_subdirectory(main)
add_subdirectory(math)
#add_subdirectory(glew)

if(APPLE)
  add_subdirectory(SDLmain)
endif()

