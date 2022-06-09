/*
 * Copyright (C) 2022 Chupligin Sergey <neochapay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

import QtQuick 2.6

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0
import QtQuick.Controls.Styles.Nemo 1.0

import org.glacier.imageeditor 1.0

Page {
    id: imageEditor

    width: parent.width;
    height: parent.height;

    property alias source: editableImage.source
    property bool edited: false

    headerTools: HeaderToolsLayout {
        showBackButton: true
        title: qsTr("Edit")

        tools: [
            ToolButton {
                iconSource: "image://theme/download"
                visible: edited
                onClicked: editableImage.save()
            }
        ]

        drawerLevels: [
            Row {
                id: editToolsRow
                width: childrenRect.width
                height: Theme.itemHeightMedium

                ToolButton {
                    id: rotateLeftButton
                    iconSource: "image://theme/rotate-left"
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        editableImage.rotateLeft()
                        edited = true
                    }
                }
                ToolButton {
                    id: rotateRightButton
                    iconSource: "image://theme/rotate-right"
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        editableImage.rotateRight()
                        edited = true
                    }
                }
                ToolButton {
                    id: flipHorizontalyButton
                    iconSource: "file:///usr/share/glacier-gallery/images/flip-horizontaly.svg"
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        editableImage.flipHorizontaly()
                        edited = true
                    }
                }
                ToolButton {
                    id: flipVerticalyButton
                    iconSource: "file:///usr/share/glacier-gallery/images/flip-verticaly.svg"
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        editableImage.flipVetricaly()
                        edited = true
                    }
                }
                ToolButton {
                    id: cropButton
                    iconSource: "image://theme/crop"
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        editableImage.cropping = !editableImage.cropping;
                        appWindow.header.closeDrawer();
                    }
                }
            }
        ]
    }

    EditableImage{
        id: editableImage
        anchors.fill: parent

        Button{
            text: qsTr("Make crop")
            onClicked: editableImage.makeCrop();
            visible: editableImage.cropping

            anchors{
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
