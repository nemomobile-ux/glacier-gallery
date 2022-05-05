set(SRC
	main.cpp
	gallery.cpp
	gallery.h)

qtquick_compiler_add_resources(RESOURCES qml/glacier-gallery.qrc)

add_subdirectory(editorplugin)

add_executable(glacier-gallery ${SRC} ${RESOURCES})

target_link_libraries(glacier-gallery
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
	Glacier::App
	PkgConfig::LIBRESOURCEQT)

install(TARGETS glacier-gallery RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY qml/api/
	DESTINATION
	${INSTALL_QML_IMPORT_DIR}/org/nemomobile/gallery)
