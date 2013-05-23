add_executable(bsp.exe main.cpp)
add_library(mesh mesh.cpp)
add_library(bsptree bsptree.cpp)


Find_Package( SDL REQUIRED)
Find_Package( OpenGL REQUIRED )

if(NOT SDL_FOUND)
  message(FATAL_ERROR "SDL wa not found")
endif(NOT SDL_FOUND)

target_link_libraries(
  bsp.exe
  math 
  mesh
  bsptree
  ${SDL_LIBRARY} 
  ${OPENGL_LIBRARIES} 
  ${GLUT_LIBRARIES}  
  ${GLEW_LIBRARIES}
)

set(GLEW_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/glew-1.6.0)


install(TARGETS bsp.exe DESTINATION ${PROJECT_SOURCE_DIR}/..)
