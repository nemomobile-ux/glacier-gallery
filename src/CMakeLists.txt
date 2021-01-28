set(SRC
	main.cpp
	gallery.cpp
	gallery.h)

add_executable(glacier-gallery ${SRC})

target_link_libraries(glacier-gallery
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
	Glacier::App
	PkgConfig::LIBRESOURCEQT)

install(TARGETS glacier-gallery RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY qml
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glacier-gallery
	PATTERN "api" EXCLUDE)
install(DIRECTORY qml/api/
	DESTINATION
	${CMAKE_INSTALL_LIBDIR}/qt5/qml/org/nemomobile/gallery)
