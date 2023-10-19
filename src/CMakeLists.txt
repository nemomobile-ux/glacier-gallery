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
	main.cpp
	gallery.cpp
	gallery.h)

qt_add_resources(RESOURCES qml/glacier-gallery.qrc)

add_subdirectory(editorplugin)

add_executable(glacier-gallery ${SRC} ${RESOURCES})

target_link_libraries(glacier-gallery
	Qt6::Gui
	Qt6::Qml
	Qt6::Quick
	Glacier::App
	PkgConfig::LIBRESOURCEQT)

install(TARGETS glacier-gallery RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})

FindQtInstallQml()

install(DIRECTORY qml/api/
	DESTINATION
        ${QT_INSTALL_QML}/org/nemomobile/gallery)
