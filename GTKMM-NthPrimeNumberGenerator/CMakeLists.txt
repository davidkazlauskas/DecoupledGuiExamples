cmake_minimum_required( VERSION 2.7 )
project( NthPrimeGen CXX )

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS} $ENV{TEMPLATIOUS_DIR})
link_directories(${GTKMM_LIBRARY_DIRS})

ADD_DEFINITIONS(
    -std=c++11
)

file(COPY ui.glade DESTINATION ${CMAKE_BINARY_DIR})

add_executable( generator main.cpp domain.cpp )
target_link_libraries( generator ${GTKMM_LIBRARIES} pthread )
