/*
 * Copyright (C) 2012 Andrea Bernabei <and.bernabei@gmail.com>
 * Copyright (C) 2017 Chupligin Sergey <neochapay@gmail.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

import QtQuick 2.6

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0
import QtQuick.Controls.Styles.Nemo 1.0

import QtQuick.Layouts 1.0

import org.nemomobile.gallery 1.0
import QtDocGallery 5.0


Page {
    id: mainPage
    width: parent.width;
    height: parent.height;
    headerTools: mainTools

    GalleryView {
        anchors.fill: parent
        model: GalleryModel {
            id: gallery
        }

        delegate: GalleryDelegate {
            MouseArea {
                anchors.fill: parent
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("ImagePage.qml"), {parameterIndex: index, galleryModel: gallery} )
            }
        }
    }


    Loader {
        id: choiceLoader
        anchors.fill: parent
    }

    property int currentFilter: 0
    ListModel {
        id: filterModel
        ListElement { name: qsTr("All") }
        ListElement { name: qsTr("Videos") }
        ListElement { name: qsTr("Images") }
    }

    property int currentSort: -1
    ListModel {
        id: sortModel
        ListElement { name: qsTr("None"); sortProperty: ""; ascending: false } // dummy
        ListElement { name: qsTr("Name"); sortProperty: "fileName"; ascending: true }
        ListElement { name: qsTr("Modified"); sortProperty: "lastModified"; ascending: true }
//        ListElement { name: qsTr("Type"); sortProperty: "mimeType"; ascending: true }
    }

    HeaderToolsLayout {
        id: mainTools
        title: qsTr("Gallery")

        drawerLevels: [
            Button {
                Layout.alignment: Qt.AlignHCenter
//                anchors.horizontalCenter: (parent==undefined) ? undefined : parent.horizontalCenter;
                text: qsTr("Slideshow")
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("ImageSlideshowPage.qml"), { visibleIndex: 0, galleryModel: gallery })
                enabled: (gallery.count > 0) && (filterButtons.currentIndex != 1)
            },
            RowLayout {
                id: filterRow
                Layout.fillWidth: true
                anchors.margins: 20
                Layout.preferredHeight: 100
                Label {
                    id: filterLabel
                    anchors.left: parent.left;
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Filter:")
                }
                ButtonRow {
                    id: filterButtons
                    model: filterModel
                    anchors{
                        left:filterLabel.right
                        verticalCenter: parent.verticalCenter
                    }

                    Component.onCompleted: {
                        filterButtons.currentIndex = 0
                    }

                    onCurrentIndexChanged: {
                        switch (filterButtons.currentIndex) {
                        case 0:
                            var videoFilter = gallery.createFilter(gallery, "videosfilter", "GalleryStartsWithFilter", "mimeType", "video/")
                            var imageFilter = gallery.createFilter(gallery, "imagesfilter", "GalleryStartsWithFilter", "mimeType", "image/")
                            var bothFilter = gallery.createFiltersArray(gallery, "arraysFilter", "GalleryFilterUnion", [videoFilter, imageFilter])
                            gallery.assignNewDestroyCurrent(bothFilter)
                            break
                        case 1:
                            var vidFilter = gallery.createFilter(gallery, "videosfilter", "GalleryStartsWithFilter", "mimeType", "video/")
                            gallery.assignNewDestroyCurrent(vidFilter)
                            break
                        case 2:
                            var imgFilter = gallery.createFilter(gallery,  "imagesfilter", "GalleryStartsWithFilter", "mimeType", "image/")
                            gallery.assignNewDestroyCurrent(imgFilter)
                            break
                        }
                    }
                }
            },
            RowLayout {
                id: sortRow
                Layout.fillWidth: true
                anchors.margins: 20
                Layout.preferredHeight: 100
                Label {
                    id: sortLabel
                    anchors.left: parent.left;
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Sort:")
                }
                ButtonRow {
                    id: sortButtons
                    model: sortModel
                    anchors{
                        left:sortLabel.right
                        verticalCenter: parent.verticalCenter
                    }

                    Component.onCompleted: {
                        sortButtons.currentIndex = 0
                    }

                    onCurrentIndexChanged: {
                        gallery.sortProperties = [ sortModel.get(sortButtons.currentIndex).sortProperty ];
                    }
                }
            }
        ]
    }



}
