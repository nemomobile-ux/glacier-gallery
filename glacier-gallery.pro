PROJECT_NAME = glacier-gallery
QT += qml quick

qml.files = qml/*
qml.path = /usr/share/glacier-gallery/qml
INSTALLS += qml

# qml API we provide
qml_api.files = qml/api/*
qml_api.path = $$[QT_INSTALL_QML]/org/nemomobile/gallery
INSTALLS += qml_api

target.path = $$INSTALL_ROOT/usr/bin
INSTALLS += target

desktop.files = $${PROJECT_NAME}.desktop
desktop.path = $$INSTALL_ROOT/usr/share/applications
INSTALLS += desktop

images.files = images/*
images.path = /usr/share/glacier-gallery/images
INSTALLS += images

HEADERS += src/gallery.h
SOURCES += src/main.cpp \
    src/gallery.cpp

# do not edit below here
TEMPLATE = app
CONFIG -= app_bundle
TARGET = $$PROJECT_NAME

CONFIG += link_pkgconfig
PKGCONFIG += libresourceqt5 glacierapp

LIBS += -lglacierapp

packagesExist(qdeclarative5-boostable) {
    message("Building with qdeclarative5-boostable support")
    DEFINES += HAS_BOOSTER
    PKGCONFIG += qdeclarative5-boostable
} else {
    warning("qdeclarative5-boostable not available; startup times will be slower")
}

DISTFILES += \
    qml/api/qmldir \
    qml/api/GalleryView.qml \
    qml/api/GalleryModel.qml \
    qml/api/GalleryDelegate.qml \
    qml/ImageContainer.qml \
    qml/ImagePage.qml \
    qml/ImageSlideshowPage.qml \
    qml/VideoPlayer.qml \
    qml/SortDialog.qml \
    qml/SingleImagePage.qml \
    qml/ZoomController.qml \
    qml/MainPage.qml \
    rpm/glacier-gallery.spec \
    qml/glacier-gallery.qml
