cmake_minimum_required(VERSION 3.19)
project(Landlords LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(Landlords
    WIN32 MACOSX_BUNDLE
    main.cpp
    gamepanel.cpp
    gamepanel.h
    gamepanel.ui
    card.h card.cpp
    cardpanel.h cardpanel.cpp
    player.h player.cpp
    cards.h cards.cpp
    robot.h robot.cpp
    userplayer.h userplayer.cpp
    gamecontrol.h gamecontrol.cpp
    ol_enum_char_ops.h
    scorepanel.h scorepanel.cpp scorepanel.ui
    mybutton.h mybutton.cpp
    buttongroup.h buttongroup.cpp buttongroup.ui
    res.qrc
    playhand.h playhand.cpp
    strategy.h strategy.cpp
    robotgraplord.h robotgraplord.cpp
    animationwindow.h animationwindow.cpp
    robotplayhand.h robotplayhand.cpp
)

target_link_libraries(Landlords
    PRIVATE
    Qt::Core
    Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Landlords
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Landlords
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
