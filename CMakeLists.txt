cmake_minimum_required(VERSION 3.15)

# Params 
set(PROJECT_NAME MovingShapes)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


project(${PROJECT_NAME}
  VERSION 0.1
  DESCRIPTION "Simple game with moving shapes"
  LANGUAGES CXX
)

# Add subdirectories
add_subdirectory(dependencies)
add_subdirectory(src)

