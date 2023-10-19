### Sets QT_INSTALL_QML to the directory where QML Plugins should be installed
function(FindQtInstallQml)
    find_program(QMAKE NAMES qmake6 qmake)
    if(NOT QMAKE)
    message(FATAL_ERROR "qmake not found")
    endif()
    execute_process(
        COMMAND ${QMAKE} -query QT_INSTALL_QML
        OUTPUT_VARIABLE PROC_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(QT_INSTALL_QML ${PROC_RESULT} PARENT_SCOPE)
endfunction()

set(SRC
    plugin.cpp
    editableimage.cpp
    editableimage.h)

add_library(glacierimageeditor SHARED ${SRC})

target_link_libraries(glacierimageeditor
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick)

FindQtInstallQml()

install(FILES qmldir DESTINATION ${QT_INSTALL_QML}/org/glacier/imageeditor/)
install(TARGETS glacierimageeditor DESTINATION ${QT_INSTALL_QML}/org/glacier/imageeditor/)
