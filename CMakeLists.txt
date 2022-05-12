cmake_minimum_required(VERSION 3.6.0)

project(glacier-gallery
	VERSION 0.2.3
	DESCRIPTION "The Glacier gallery")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)

include(FeatureSummary)
include(GNUInstallDirs)

set(QT_MIN_VERSION "5.11.0")
find_package(Qt5 ${QT_MIN_VERSION} COMPONENTS Gui Qml Quick LinguistTools REQUIRED)
find_package(Glacier 0.8 COMPONENTS App REQUIRED)
find_package(Qt5QuickCompiler)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBRESOURCEQT libresourceqt5 REQUIRED IMPORTED_TARGET)

if(LIBRESOURCEQT_FOUND)
	set_property(GLOBAL APPEND PROPERTY PACKAGES_FOUND libresourceqt5)
else()
	set_property(GLOBAL APPEND PROPERTY PACKAGES_NOT_FOUND libresourceqt5)
endif()

add_subdirectory(src)

# Translations
file(GLOB TS_FILES translations/*.ts)
qt5_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations DEPENDS ${QM_FILES})
add_dependencies(glacier-gallery translations)

install(FILES ${QM_FILES}
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glacier-gallery/translations)

install(FILES glacier-gallery.desktop glacier-gallery-openfile.desktop
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES org.nemomobile.gallery.service
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services)
install(DIRECTORY images
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glacier-gallery)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
