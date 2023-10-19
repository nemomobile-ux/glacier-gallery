/*
 * Copyright (C) 2012 Andrea Bernabei <and.bernabei@gmail.com>
 * Copyright (C) 2023 Chupligin Sergey <neochapay@gmail.com>
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

import Nemo
import Nemo.Controls


Page {

    id: imageEditor

    width: parent.width;
    height: parent.height;

    headerTools: HeaderToolsLayout {
        showBackButton: true
        title: qsTr("Details")

    }

    property variant galleryModel
    property int visibleIndex

    ListModel {
        id: videoElements;
        ListElement { title: qsTr("Filename: %1"); field: "fileName" }
        ListElement { title: qsTr("Full path: %1"); field: "url" }
        ListElement { title: qsTr("Last modified: %1"); field: "lastModified" }
//        ListElement { title: qsTr("Size: %1 B"); field: "fileSize" }
        ListElement { title: qsTr("Duration: %1"); field: "duration";
            pretty: function(input) {
                var seconds = input % 60;
                return Math.floor(input/60) + ":" + ((seconds < 10) ? "0" : "") + seconds ;
            }
        }
        ListElement { title: qsTr("Frame rate: %1 fps"); field: "frameRate" }
    }

    ListModel {
        id: imageElements;
        ListElement { title: qsTr("Filename: %1"); field: "fileName" }
        ListElement { title: qsTr("Full path: %1"); field: "url" }
        ListElement { title: qsTr("Last modified: %1"); field: "lastModified" }
//        ListElement { title: qsTr("Date taken: %1"); field: "dateTaken"}
//        ListElement { title: qsTr("Size: %1 B"); field: "fileSize" }
        ListElement { title: qsTr("Orientation: %1˚"); field: "orientation"}
        ListElement { title: qsTr("Exposure time: %1/1000"); field: "exposureTime"; pretty: function(input) { return input*1000; } }
        ListElement { title: qsTr("fNumber: %1"); field: "fNumber"}
        ListElement { title: qsTr("Flash Enabled: %1 "); field: "flashEnabled"; pretty: function(input) { return String(input).replace(/[^#]*#/g, "");}}
        ListElement { title: qsTr("Focal Lenght: %1 mm"); field: "focalLength"}
        ListElement { title: qsTr("Metering mode: %1"); field: "meteringMode"; pretty: function(input) { return String(input).replace(/[^#]*#/g, "");} }
        ListElement { title: qsTr("White balance: %1"); field: "whiteBalance"; pretty: function(input) { return String(input).replace(/[^#]*#/g, "");}}
        ListElement { title: qsTr("Camera Manufacturer: %1"); field: "cameraManufacturer"}
        ListElement { title: qsTr("Camera Model: %1"); field: "cameraModel"}
        ListElement { title: qsTr("Latitude: %1"); field: "latitude"}
        ListElement { title: qsTr("Longitude: %1"); field: "longitude"}
        ListElement { title: qsTr("Altitude: %1 m"); field: "altitude"; pretty: function(input) { return Math.round(input); } }
    }

    ListView {
        anchors.fill: parent
        anchors.margins: Theme.itemSpacingMedium
        spacing: Theme.itemSpacingSmall

        model: galleryModel.isVideo(visibleIndex) ? videoElements : imageElements;
        delegate: Text {

            color: Theme.textColor
            font.pixelSize: Theme.fontSizeLarge

            wrapMode: Text.Wrap
            width: (parent != null) ?  parent.width : Theme.itemWidthLarge

            property var unformattedValue: galleryModel.get(visibleIndex)[field]
            property string value: (pretty !== undefined) ? pretty(unformattedValue) : unformattedValue;
            text: String(title).arg(value)
        }
    }

}
