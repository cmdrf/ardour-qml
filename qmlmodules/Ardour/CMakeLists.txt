qt_add_library(Ardour STATIC)
qt_add_qml_module(Ardour
    URI Ardour
    VERSION 1.0
    QML_FILES SessionControls.qml
    SOURCES Session.cpp Session.h
)

set_target_properties(Ardour PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(Ardour
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(Ardour PRIVATE
    Qt6::Quick
    ardourlib
)

target_include_directories(Ardour PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

