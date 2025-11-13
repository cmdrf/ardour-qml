add_library(lua
	${L}/lua/lua.cc
	${L}/lua/luastate.cc
)

target_include_directories(lua PUBLIC
	${L}/lua
)

target_link_libraries(lua PUBLIC
	PkgConfig::SIGCPP
)
