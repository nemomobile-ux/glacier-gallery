/*
 * Copyright (C) 2022 Chupligin Sergey <neochapay@gmail.com>
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

import QtDocGallery 5.0

Item{
    id: downListView
    property alias model: littleImagesListView.model
    property int currentIndex: -1

    Rectangle{
        id: background
        anchors.fill: parent
        color: Theme.backgroundColor
        opacity: 0.5
    }

    ListView{
        id: littleImagesListView
        anchors.fill: parent

        orientation: ListView.Horizontal
        currentIndex: imageController.parameterIndex

        delegate: Item{
            height: Theme.itemHeightLarge
            width: height

            Image{
                asynchronous: true
                anchors.centerIn: parent
                sourceSize.width: Theme.itemHeightLarge
                sourceSize.height: Theme.itemHeightLarge
                source: gallery.isVideo(url) ? "file:///usr/share/glacier-gallery/images/DefaultVideoThumbnail.jpg" :  url
                fillMode: Image.PreserveAspectFit
                height: index === currentIndex ? parent.width : parent.width*0.8
                width: parent.height

                MouseArea{
                    anchors.fill: parent
                    onClicked: currentIndex = index
                }

                Behavior on width{
                    NumberAnimation { duration: 100 }
                }

                Behavior on height{
                    NumberAnimation { duration: 100 }
                }
            }
        }
    }
}
