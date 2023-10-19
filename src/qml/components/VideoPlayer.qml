/*
 * Copyright (C) 2012 Andrea Bernabei <and.bernabei@gmail.com>
 * Copyright (C) 2017-2023 Chupligin Sergey <neochapay@gmail.com>
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

import QtMultimedia

Page {
    id: videoContainer

    property alias videoSource: videoItem.source

    allowedOrientations: Qt.LandscapeOrientation
    height: parent.height;
    width: parent.width;

    headerTools: HeaderToolsLayout {
        showBackButton: true
        title: qsTr("Video")
    }


    NemoIcon {
        anchors.centerIn: parent;
        source: "image://theme/play"
        visible: (videoItem.playbackState === MediaPlayer.StoppedState)
        z: videoItem.z + 1
    }

    NemoIcon {
        anchors.centerIn: parent;
        source: "image://theme/pause"
        visible: (videoItem.playbackState === MediaPlayer.PausedState)
        z: videoItem.z + 1
    }

    NemoIcon {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: Theme.itemSpacingMedium
        source: "image://theme/backward"
        visible: (videoItem.playbackState === MediaPlayer.PausedState)
        z: videoItem.z + 1
    }

    NemoIcon {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: Theme.itemSpacingMedium
        source: "image://theme/forward"
        visible: (videoItem.playbackState === MediaPlayer.PausedState)
        z: videoItem.z + 1
    }

    Label {
        anchors.right: parent.right;
        anchors.top: parent.top
        anchors.margins: Theme.itemSpacingMedium
        color: Theme.textColor
        text: formatVideoPosition(videoItem.position)
        visible: (videoItem.playbackState === MediaPlayer.PausedState)
        z: videoItem.z + 1
    }

    function formatVideoPosition(ms) {
        var s = Math.floor((ms/1000)%60)
        var m = Math.floor((ms/60000))
        return ((m > 9) ? m : "0"+m)+":"+((s > 9) ? s : "0"+s)
    }

    Slider {
        minimumValue: 0
        maximumValue: videoItem.duration
        value: videoItem.position
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        visible: (videoItem.playbackState === MediaPlayer.PausedState)
        z: videoItem.z + 1
        onValueChanged: {
            videoItem.seek(value)
        }

    }

    Video {
        id: videoItem
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit

        //autoLoad: true doesn't seem to be working
        Component.onCompleted: play()

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (parent.playbackState !== MediaPlayer.PlayingState) {
                    var r = mouse.x/videoItem.width
                    if (r < 0.25) {
                        videoItem.seek(videoItem.position - 10000);
                    } else if (r < 0.75) {
                        parent.play();
                    } else {
                        videoItem.seek(videoItem.position + 10000);
                    }
                } else {
                    parent.pause()
                }
            }
        }
    }
}
