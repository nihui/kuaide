
set(applets_SRCS
    applets/dclock.cpp
    applets/kbutton.cpp
    applets/mnpager.cpp
    applets/sctasks.cpp
    applets/systray.cpp
)

set(kuaipanel_SRCS
    applet.cpp
    container.cpp
    kuaipanel.cpp
    main.cpp
    ${applets_SRCS}
)

kde4_add_executable(kuaipanel ${kuaipanel_SRCS})

target_link_libraries(kuaipanel
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    kworkspace
    taskmanager
    ${X11_X11_LIB}
)

install(TARGETS kuaipanel ${INSTALL_TARGETS_DEFAULT_ARGS})
# install(FILES kuaipanel.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kuaipanelapplet.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
