
find_package(LibKonq REQUIRED)

set(kuaidesktop_SRCS
    icondelegate.cpp
    iconview.cpp
    kuaidesktop.cpp
    main.cpp
)

kde4_add_executable(kuaidesktop ${kuaidesktop_SRCS})

target_link_libraries(kuaidesktop
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KFILE_LIBS}
    ${KDE4_KIO_LIBS}
    ${LIBKONQ_LIBRARY}
)

install(TARGETS kuaidesktop ${INSTALL_TARGETS_DEFAULT_ARGS})
# install(FILES kuaidesktop.desktop DESTINATION ${SERVICES_INSTALL_DIR})
