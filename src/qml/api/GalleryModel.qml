/*
 * Copyright (C) 2012 Andrea Bernabei <and.bernabei@gmail.com>
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
import QtQml.Models 2.15
import QtDocGallery 5.0

ListModel {
    id: mainModel;

    function sourceModelsChanged() {
        copyReady = false;
        var i, item;
        mainModel.clear()
        for (i = 0; i < pictureGallery.count; i++) {
            item = pictureGallery.get(i)
            item.url = (String)(item.url);
            mainModel.append(item);
        }

        for (i = 0; i < videoGallery.count; i++) {
            item = videoGallery.get(i)
            item.url = (String)(item.url);
            mainModel.append(item);
        }
        copyReady = true;
    }

    //destroying the old filter before the new one is assigned makes the gallery model misbehave!
    function assignNewDestroyCurrent(newFilter) {
        var old = gallery.filter
        gallery.filter = newFilter
    }

    //this is to create single filters dynamically
    function createFilter(parentItem, name, filterType, keyToFilter, value){
        var myFilter = Qt.createQmlObject('import QtDocGallery 5.0;' + filterType + '{property: "' +keyToFilter + '"; value: "' + value + '" }',
                                          parentItem, name);
        return myFilter
    }

    //this is to create group filters, such as union and intersection ones
    function createFiltersArray(parentItem, name, filterType, filtersArray){
        var myFilter = Qt.createQmlObject('import QtDocGallery 5.0;' + filterType + '{ }',
                                          parentItem, name);
        myFilter.filters = filtersArray
        return myFilter
    }

    //this is to know if the item at index "index" is a video
    function isVideo(index) {
        //elements have index == -1 when the gallery model is being rebuilt (e.g. filters are changed)
        //In that case, we must not access model data
        //NOTE: this will still return a value, best thing would be to add a check in the other components
        //every time before using this method!
        if (index !== -1)
        {
            var mimeString = get(index).mimeType.toString()
            return (mimeString.substring(0,5) === "video")
        }
    }


    property variant sortProperties: []
    property variant filter;
    property bool copyReady: true;
    property bool loading: pictureGallery.status != DocumentGalleryModel.Finished || videoGallery.status != DocumentGalleryModel.Finished || !copyReady;
    property bool error: DocumentGalleryModel.Error

    property variant pictures:
        DocumentGalleryModel {
        id: pictureGallery;
        properties: [
            "fileName",
            "fileSize",
            "lastModified",
            "mimeType",
            "url",
            "orientation",
            "latitude",
            "longitude",
            "altitude",
            "dateTaken",
            "created",
            "exposureTime",
            "fNumber",
            "flashEnabled",
            "focalLength",
            "meteringMode",
            "whiteBalance",
            "cameraManufacturer",
            "cameraModel",
        ]

        autoUpdate: true
        rootType: DocumentGallery.Image
        onCountChanged: {
            sourceModelsChanged();
        }

    }

    property variant videos: DocumentGalleryModel {
        id: videoGallery;
        properties: [
            "fileName",
            "fileSize",
            "lastModified",
            "mimeType",
            "url",
            "duration",
            "frameRate",
            "created",
        ]
        autoUpdate: true
        rootType: DocumentGallery.Video
        onCountChanged: {
            sourceModelsChanged();
        }
    }


    onSortPropertiesChanged: {
        pictureGallery.sortProperties = sortProperties;
        videoGallery.sortProperties = sortProperties;
    }

    onFilterChanged: {
        pictureGallery.filter = filter;
        videoGallery.filter = filter;
    }
}
