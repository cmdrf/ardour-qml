cmake_minimum_required(VERSION 3.16)

# Add test executable
find_package(Qt6 REQUIRED COMPONENTS Core Test Qml Gui)

# Create test executable
qt_add_executable(SessionTests
    SessionTest.cpp
    SessionTest.h
)

# Set compile definitions to match Ardour module
target_compile_definitions(SessionTests PRIVATE
    QT_NO_KEYWORDS
)

# Link required libraries
target_link_libraries(SessionTests PRIVATE
    Qt6::Core
    Qt6::Test
    Qt6::Qml
    Qt6::Gui
    Ardour
    ardourlib
)

# Add test to CTest
add_test(NAME SessionTests COMMAND SessionTests)

# Enable testing
enable_testing()
