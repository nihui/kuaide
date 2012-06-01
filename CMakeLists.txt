project(kuaide)

find_package(KDE4 REQUIRED 4.5)
include(KDE4Defaults)
include(MacroLibrary)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${QT_INCLUDES} ${KDE4_INCLUDES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(desktop)
add_subdirectory(panel)
