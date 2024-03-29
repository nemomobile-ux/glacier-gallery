/*
 * Copyright (C) 2012 Andrea Bernabei <and.bernabei@gmail.com>
 * Copyright (C) 2023 Chupligin Sergey <neochapay@gmail.com>
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

import Nemo
import Nemo.Controls

import QtDocGallery 5.0

import "../components"

Page {
    id: singleImagePage

    width: parent.width;
    height: parent.height;

    //filename of the element we're showing
    property alias imageSource: singleImage.imageSource

    property real swipeThreshold: 40
    //number of pixel you have to move before the Pinch Area is disabled
    property real pinchThreshold: 3

    onWidthChanged: {
        //we're assuming this is only used for images (videos are auto-played, so they won't use this page)
        //if we decide to show this page with the video thumbnail before playing the video, a check
        //has to be added here (for instance: if (!video) resetZoom())
        pinchImg.resetZoom()
    }

    ZoomController {
        id: pinchImg

        //if we decide this page is also used to show video thumbnails
        //this area will have to be disabled when showing video thumbnails

        pinchTarget: singleImage.image
        connectedFlickable: singleImage.flickableArea
        targetContainer: singleImage
    }

    Connections {
        target: singleImage
        function onClickedWhileZoomed() { fullScreenModeArea.handleClick() }
    }

    MouseArea {
        id: fullScreenModeArea
        anchors.fill: parent

        function handleClick() {
            if (toolbarTimer.running) {
                toolbarTimer.stop()
            } else {
                toolbarTimer.start()
            }
        }

        //we use this to be able to not call singleclick handlers when the user is actually doubleclicking
        Timer {
            id: toolbarTimer
            interval: 350
            onTriggered: appWindow.fullscreen = !appWindow.fullscreen
        }

        onClicked: handleClick()
    }

    ImageContainer {
        id: singleImage
        pinchingController: pinchImg
    }

    headerTools: HeaderToolsLayout {
        showBackButton: true
        title: qsTr("Image")
    }

}
