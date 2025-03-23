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
    gallerymodel.cpp
    filesystemworker.cpp)

set(HEADERS
    plugin.h
    gallerymodel.h
    filesystemworker.h)

set(QML
    qml/GalleryDelegate.qml
    qml/GalleryView.qml)

add_library(glaciergalleryplugin SHARED ${SRC})

qt6_add_qml_module(glaciergalleryplugin
    URI Glacier.Gallery
    VERSION 1.0
    PLUGIN_TARGET glaciergalleryplugin
    NO_GENERATE_PLUGIN_SOURCE
    SOURCES
        ${SOURCES}
        ${HEADERS}
    QML_FILES
        ${QML}
    OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/Glacier/Gallery
        RESOURCES qml/qmldir
        SOURCES ${SOURCES}
)

target_link_libraries(glaciergalleryplugin PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick)

FindQtInstallQml()

install(TARGETS glaciergalleryplugin
    RUNTIME DESTINATION "${QT_INSTALL_QML}/Glacier/Gallery"
    BUNDLE DESTINATION "${QT_INSTALL_QML}/Glacier/Gallery"
    LIBRARY DESTINATION "${QT_INSTALL_QML}/Glacier/Gallery"
)

install(DIRECTORY qml/
    DESTINATION ${QT_INSTALL_QML}/Glacier/Gallery)
