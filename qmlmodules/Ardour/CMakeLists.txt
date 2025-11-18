find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

#qt_add_library(Ardour SHARED)
qt_add_qml_module(Ardour
    URI Ardour
    VERSION 1.0
	NO_GENERATE_PLUGIN_SOURCE
	NO_PLUGIN_OPTIONAL
    QML_FILES SessionControls.qml
	DEPENDENCIES Qt6::Quick ardourlib
    SOURCES Session.cpp Session.h
	SOURCES QtBridgeUi.h QtBridgeUi.cpp
	SOURCES Ardour.h Ardour.cpp
	SOURCES Route.h Route.cpp
	SOURCES RouteModel.h RouteModel.cpp
	SOURCES Track.h Track.cpp
	SOURCES Playlist.h Playlist.cpp
	SOURCES Region.h Region.cpp
	SOURCES Stateful.h Stateful.cpp
	SOURCES TimePos.h
	SOURCES TimeCount.h
)

set_target_properties(Ardour PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(Ardour
	PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
	QT_NO_KEYWORDS
)
target_link_libraries(Ardour PRIVATE
	Qt6::Quick
	ardourlib
)

target_include_directories(Ardour PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Plugin:
target_sources(Ardourplugin PRIVATE
	ArdourPlugin.h ArdourPlugin.cpp
)
target_link_libraries(Ardourplugin PRIVATE
	Qt6::Quick
	ardourlib
)
target_compile_definitions(Ardourplugin PRIVATE
	QT_NO_KEYWORDS
)
