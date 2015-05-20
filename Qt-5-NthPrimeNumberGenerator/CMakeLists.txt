cmake_minimum_required( VERSION 2.7 )
project( QtNthGenerator CXX )

find_package( Qt4 COMPONENTS QTCORE QTGUI REQUIRED )
INCLUDE(${QT_USE_FILE})

QT4_WRAP_UI(UISrcs mainwindow.ui)
QT4_WRAP_CPP(MOCSrcs mainwindow.h)

ADD_DEFINITIONS(
    -std=c++11
)

include_directories( $ENV{TEMPLATIOUS_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
add_executable( generator mainwindow.cpp domain.cpp ${MOCSrcs} ${UISrcs})
target_link_libraries( generator ${QT_LIBRARIES} )
