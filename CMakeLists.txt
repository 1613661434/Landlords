cmake_minimum_required(VERSION 3.19)
project(Landloads LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(Landloads
    WIN32 MACOSX_BUNDLE
    main.cpp
    gamepanel.cpp
    gamepanel.h
    gamepanel.ui
    card.h card.cpp
    cardpanel.h cardpanel.cpp
    player.h player.cpp
)

target_link_libraries(Landloads
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(Landloads PRIVATE Qt6::Widgets)
target_link_libraries(Landloads PRIVATE Qt6::Core)

include(GNUInstallDirs)

install(TARGETS Landloads
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Landloads
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
