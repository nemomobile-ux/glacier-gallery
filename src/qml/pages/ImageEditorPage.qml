import QtQuick 2.6

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0
import QtQuick.Controls.Styles.Nemo 1.0


Page {
    id: imageEditor

    width: parent.width;
    height: parent.height;

    headerTools: HeaderToolsLayout {
        showBackButton: true
        title: qsTr("Edit")
    }
}
