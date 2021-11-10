import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

import MobileNetworkOperator.CustomType 1.0

ApplicationWindow {
    id: rootWindow
    width: rectScreen.width
    height: rectScreen.height
    x: rectScreen.x
    y: rectScreen.y
    visible: true

    TreeView {
        id: treeViewID
        anchors.fill: parent
        model: treeModel
        itemDelegate: Item {
            RowLayout {
                Image {
                    property real widthIcon: labelName.height * 0.8

                    id: imageIcon
                    width: 2
                    height: 2
                    Layout.fillWidth: true
                    Layout.minimumWidth: widthIcon
                    Layout.maximumWidth: widthIcon
                    Layout.fillHeight: true
                    Layout.minimumHeight: widthIcon
                    Layout.maximumHeight: widthIcon
                    source: styleData.value ? styleData.value.icon : ""
                }
                Label {
                    id: labelName
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: styleData.value ? styleData.value.name : ""
                }
                Label {
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: "(" + styleData.value.mcc + ", " + styleData.value.mnc + ")"
                    visible: styleData.value && styleData.value.typeData === CustomType.OperatorDataType
                }
                Button{
                    Layout.maximumWidth: imageIcon.width
                    Layout.maximumHeight: parent.height
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    text: "+"
                    visible: mouseArea.containsMouse && styleData.value.typeData === CustomType.OperatorDataType
                    onClicked: {
                        let customType = styleData.value;
                        providerQml.emptySlot(customType.mcc, customType.mnc)
                    }
                }
            }
            MouseArea{
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                propagateComposedEvents: true
                onPressed: mouse.accepted = false
                onReleased: mouse.accepted = false
            }
        }

        TableViewColumn {
            role: "customTypeRole"
        }

        onDoubleClicked: {
            let customType = treeModel.getCustomType(index);
            if(customType.typeData === CustomType.OperatorDataType){
                console.log("DoubleClicked:", customType.icon, customType.name, customType.mcc, customType.mnc, customType.typeData);
                dialogEditor.showDialog(treeModel.getCountryIcon(customType.mcc), customType.icon, customType.name, customType.mcc, customType.mnc, false, index)
            }
        }
    }


    QmlDialog{
        id: dialogEditor
        visible: false
    }

    Button{
        width: parent.width / 15
        height: width
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: width
        anchors.bottomMargin: width
        text: "+"
        onClicked: dialogEditor.showDialog("", "", "", "", "", true)
    }
}
