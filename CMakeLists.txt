cmake_minimum_required(VERSION 3.10)

# set(CMAKE_CXX_STANDARD 17)
project(cppnd-capstone-deepSORT)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

# add OpenCV library
find_package(OpenCV 4.3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# add Eigen library
find_package (Eigen3 3.3 REQUIRED NO_MODULE)


# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

# Add project executable
add_executable(deepSORT ${project_SRCS})

#add_executable(deepSORT src/KalmanFilter.cpp test_kalman.cpp)


target_link_libraries(deepSORT ${OpenCV_LIBRARIES})
target_link_libraries (deepSORT Eigen3::Eigen)
