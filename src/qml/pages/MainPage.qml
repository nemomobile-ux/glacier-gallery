/*
 * Copyright (C) 2012 Andrea Bernabei <and.bernabei@gmail.com>
 * Copyright (C) 2017-2025 Chupligin Sergey <neochapay@gmail.com>
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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Nemo
import Nemo.Controls

import Glacier.Gallery

import org.nemomobile.sortfiltermodel 1.0

Page {
    id: mainPage
    headerTools: mainTools

    SortFilterModel{
        id: gallerySorted
        sourceModel: GalleryModel {
            id: gallery
        }
        sortRole: "modified"
        sortOrder: Qt.DescendingOrder
        property alias loading: gallery.loading
    }


    GalleryView {
        anchors.fill: parent
        model: gallerySorted

        delegate: GalleryDelegate {
            MouseArea {
                anchors.fill: parent
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("ImagePage.qml"), {parameterIndex: index, galleryModel: gallery} )
            }
        }

        Label {
            text: qsTr("No photo has been taken yet")
            anchors.centerIn: parent
            visible: (gallery.count === 0) && !gallery.loading
        }

        Label {
            text: qsTr("Error loading images")
            anchors.centerIn: parent
            visible: gallery.error
        }

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
        ListElement {
            name: qsTr("None");
            sortProperty: "none";
        }
        ListElement {
            name: qsTr("Name");
            sortProperty: "name";
        }
        ListElement {
            name: qsTr("Modified");
            sortProperty: "time";
        }
        ListElement {
            name: qsTr("Size");
            sortProperty: "size";
        }
    }

    HeaderToolsLayout {
        id: mainTools
        title: qsTr("Gallery")

        drawerLevels: [
            Button {
                Layout.alignment: Qt.AlignHCenter
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
                    text: qsTr("Filter:")
                }
                ButtonRow {
                    id: filterButtons
                    model: filterModel

                    Component.onCompleted: {
                        filterButtons.currentIndex = 0
                    }

                    onCurrentIndexChanged: {
                        switch (filterButtons.currentIndex) {
                        case 0:
                            gallery.filter = GalleryModel.AllFiles
                            break
                        case 1:
                            gallery.filter = GalleryModel.OnlyVideo
                            break
                        case 2:
                            gallery.filter = GalleryModel.OnlyImages
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
                    text: qsTr("Sort:")
                }
                ButtonRow {
                    id: sortButtons
                    model: sortModel

                    Component.onCompleted: {
                        sortButtons.currentIndex = 0
                    }

                    onCurrentIndexChanged: {
                        if(sortModel.get(sortButtons.currentIndex).sortProperty == "none") {
                            gallery.sortMode = GalleryModel.Unsorted;
                        }
                        if(sortModel.get(sortButtons.currentIndex).sortProperty == "name") {
                            gallery.sortMode = GalleryModel.SortByName;
                        }
                        if(sortModel.get(sortButtons.currentIndex).sortProperty == "size") {
                            gallery.sortMode = GalleryModel.SortBySize;
                        }
                        if(sortModel.get(sortButtons.currentIndex).sortProperty == "time") {
                            gallery.sortMode = GalleryModel.SortByTime;
                        }
                    }
                }
            }
        ]
    }
}
