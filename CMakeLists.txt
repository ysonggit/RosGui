cmake_minimum_required(VERSION 2.8)
project(rosgui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

find_package(PkgConfig)
find_package(Boost COMPONENTS system filesystem REQUIRED)

pkg_check_modules(GTKMM gtkmm-3.0)

#add_subdirectory(src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

link_directories(
  ${GTKMM_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIR})

include_directories(
  ${GTKMM_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR})

add_executable(rosgui main.cpp rosgui.cpp rosgui.h launchfile.cpp launchfile.h popwindow.cpp popwindow.h pose.h)

target_link_libraries(rosgui ${GTKMM_LIBRARIES} ${Boost_LIBRARIES} )
