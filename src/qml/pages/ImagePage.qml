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
import QtQml 2.15

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0
import QtQuick.Controls.Styles.Nemo 1.0

import QtDocGallery 5.0

import "../components"

Page {
    id: imageController
    width: parent.width;
    height: parent.height;

    property variant galleryModel
    property int parameterIndex: -1

    headerTools: HeaderToolsLayout {
        showBackButton: true
        title: currentImage.isVideo ? qsTr("Show video") : qsTr("Show image")

        tools: [
            ToolButton{
                iconSource: "image://theme/pencil-alt"
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ImageEditorPage.qml"), {visibleIndex: imageController.visibleIndex, galleryModel: imageController.galleryModel});
                }
            },
            ToolButton{
                iconSource: "image://theme/info"
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ImageInfoPage.qml"), {visibleIndex: imageController.visibleIndex, galleryModel: imageController.galleryModel});
                }
            }
        ]
    }

    clip: true

    ImageContainer {
        id: previosImage;
        anchors.right: currentImage.left
        visible: source != ""
    }

    ImageContainer {
        id: currentImage;
        source: galleryModel.get(parameterIndex).url
    }

    ImageContainer {
        id: nextImage;
        anchors.left: currentImage.right
        visible: source != ""
    }

    ZoomController {
        id: pinchImg

        //Disable the pincharea if the listview is scrolling, to avoid problems
        enabled: (currentImage.x === 0 && !currentImage.isVideo)

        pinchTarget: currentImage.image
        connectedFlickable: currentImage.flickableArea
        targetContainer: currentImage
    }

    MouseArea {
        id: listFlickable
        anchors.fill: parent

        property int firstPressX: 0
        property int pinchThreshold: 3
        property bool moving: false

        onPressed: {
            listFlickable.firstPressX = mouseX
        }

        onPositionChanged: {
            if (Math.abs(firstPressX - mouseX) > pinchThreshold && moving == false) {
                moving = true
            }

            if (moving) {
                currentImage.x = -(firstPressX - mouseX)
            }
        }

        onReleased: {
            firstPressX = 0
            moving = false

            if(Math.abs(currentImage.x) > imageController.width/4) {
                /*Swipe to next/prev*/
                if(currentImage.x < 0 && parameterIndex < galleryModel.count) {
                    //parameterIndex++
                    flickTo.to = -currentImage.width
                } else if(parameterIndex > 0 && currentImage.x > 0) {
                    //parameterIndex--
                    flickTo.to = currentImage.width
                }
            } else {
                /*swipe to current*/
                flickTo.to = 0
            }

            flickTo.start()
        }
    }

    NumberAnimation {
        id: flickTo;
        target: currentImage;
        property: "x";
        from: currentImage.x;
        duration: 300;
        easing.type: Easing.OutQuad
        onStarted: listFlickable.enabled = false
        onStopped: {
            listFlickable.enabled = true
            if(flickTo.to < 0 ) {
                parameterIndex++
            }

            if(flickTo.to > 0) {
                parameterIndex--
            }
        }
    }

    Component.onCompleted: {
        loadImages();
    }

    onParameterIndexChanged: {
        loadImages();
    }

    function loadImages() {
        if(galleryModel === undefined) {
            return
        }

        if(parameterIndex > 1) {
            previosImage.source = galleryModel.get(parameterIndex-1).url;
        } else {
            previosImage.source = "";
        }

        if(parameterIndex < galleryModel.count) {
            nextImage.source = galleryModel.get(parameterIndex+1).url;
        } else {
            nextImage.source = "";
        }

        currentImage.x = 0
    }

    function showVideoPlayer(fileName) {
        pageStack.push(Qt.resolvedUrl("../components/VideoPlayer.qml"),
                       {videoSource: fileName},
                       true)
    }
}
