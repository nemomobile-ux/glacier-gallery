/*
 * Copyright (C) 2012 Andrea Bernabei <and.bernabei@gmail.com>
 * Copyright (C) 2017-2020 Chupligin Sergey <neochapay@gmail.com>
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
import QtQuick.Window 2.1

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0
import QtQuick.Controls.Styles.Nemo 1.0

import Nemo.DBus 2.0
import "./components"
import "./pages"

ApplicationWindow {
    id: appWindow

    initialPage: mainPage

    contentOrientation: Screen.orientation
    allowedOrientations:  Qt.PortraitOrientation | Qt.LandscapeOrientation | Qt.InvertedLandscapeOrientation | Qt.InvertedPortraitOrientation

    MainPage {
        id: mainPage
    }

    DBusAdaptor {
        id: openFileAdaptor
        service: "org.nemomobile.gallery"
        path: "/"
        iface: "org.nemomobile.gallery"

        xml: '  <interface name="org.nemomobile.gallery">\n' +
             '    <method name="displayFile" />\n' +
             '        <arg name="filename" type="as" direction="in"/>\n' +
             '    </method>\n' +
             '  </interface>\n'

        function displayFile(filename) {
            appWindow.displayFile(filename)
        }
    }

    function displayFile(filename) {
        console.log("displayFile:" + filename)
        appWindow.pageStack.pop(null) // Unwind to top of the stack, wherever we are

        switch (gallery.isVideo(filename)) {
        case 0:
            appWindow.pageStack.push(Qt.resolvedUrl("pages/SingleImagePage.qml"), { imageSource: filename })
            break
        case 1:
            appWindow.pageStack.push(Qt.resolvedUrl("components/VideoPlayer.qml"), { videoSource: filename })
            break
        case -1:
            console.log("displayFile: ERROR WHILE LOADING THE FILE, FILE NOT FOUND")

        }
    }

}
