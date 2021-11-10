import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Dialog {
    function showDialog(countryIcon, operatorIcon, operatorName, operatorMcc, operatorMnc, enabledEditingMccMnc = false, index = null){
        itemContent.currentIndex            = index
        itemContent.countryIcon             = countryIcon
        itemContent.operatorIcon            = operatorIcon
        itemContent.operatorName            = operatorName
        itemContent.operatorMcc             = operatorMcc
        itemContent.operatorMnc             = operatorMnc
        itemContent.enabledEditingMccMnc    = enabledEditingMccMnc
        itemContent.changeEnabledButtons()
        visible = true
        if(enabledEditingMccMnc){
            itemContent.clearAllTextFields()
            itemContent.updateIcons("", "")
        }
    }

    id: dialog
    anchors.centerIn: parent
    implicitWidth: parent.width / 1.5
    implicitHeight: parent.height / 2
    closePolicy: Popup.NoAutoClose
    standardButtons: Dialog.Save | Dialog.Cancel
    modal: true

    background: Rectangle{
        anchors.fill: parent
    }
    contentItem: Item {
        property variant currentIndex
        property string countryIcon: ""
        property string operatorIcon: ""
        property string operatorName: ""
        property string operatorMcc: ""
        property string operatorMnc: ""
        property bool enabledEditingMccMnc: false

        function changeEnabledButtons(){
            if(enabledEditingMccMnc && textFieldName.text.length !== 0 && textFieldMcc.text.length !== 0 && textFieldMnc.text.length !== 0){
                dialog.standardButton(Dialog.Save).enabled = true
            }
            else if(!enabledEditingMccMnc && textFieldName.text.length !== 0){
                dialog.standardButton(Dialog.Save).enabled = true
            }
            else{
                dialog.standardButton(Dialog.Save).enabled = false
            }
        }

        function clearAllTextFields(){
            textFieldName.clear()
            textFieldMcc.clear()
            textFieldMnc.clear()
        }

        function updateIcons(mcc, mnc){
            itemContent.countryIcon             = treeModel.getCountryIcon(mcc)
            itemContent.operatorIcon            = treeModel.getOperatorIcon(mcc, mnc)
        }

        id: itemContent
        implicitWidth: parent.width
        implicitHeight: parent.height

        property real minWidthDelegate: 11
        ColumnLayout {

            width: parent.width
            RowLayout{
                Layout.minimumHeight: parent.height / 4
                Item {
                    Layout.minimumWidth: itemContent.minWidthDelegate
                    Image {
                        anchors.centerIn: parent
                        source: itemContent.operatorIcon
                    }
                }
                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    text: "Operator editor"
                    font.bold: true
                }
            }
            RowLayout{
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: itemContent.minWidthDelegate
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: "Name"
                    Component.onCompleted: { itemContent.minWidthDelegate = width; }
                }
                TextField {
                    id: textFieldName
                    Layout.fillWidth: true
                    selectByMouse: true
                    text: itemContent.operatorName
                    onTextChanged: itemContent.changeEnabledButtons()
                }
            }
            RowLayout{
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: itemContent.minWidthDelegate
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: "MCC"
                }
                TextField {
                    id: textFieldMcc
                    Layout.fillWidth: true
                    selectByMouse: true
                    enabled: itemContent.enabledEditingMccMnc
                    text: itemContent.operatorMcc
                    onTextChanged: {
                        itemContent.changeEnabledButtons()
                        if(itemContent.enabledEditingMccMnc){
                            itemContent.updateIcons(textFieldMcc.text, textFieldMnc.text)
                        }
                    }
                }
                Item {
                    Layout.minimumWidth: itemContent.minWidthDelegate
                    Image {
                        anchors.centerIn: parent
                        source: itemContent.countryIcon
                    }
                }
            }
            RowLayout{
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: itemContent.minWidthDelegate
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: "MNC"
                }
                TextField {
                    id: textFieldMnc
                    Layout.fillWidth: true
                    selectByMouse: true
                    enabled: itemContent.enabledEditingMccMnc
                    text: itemContent.operatorMnc
                    onTextChanged: {
                        itemContent.changeEnabledButtons()
                        if(itemContent.enabledEditingMccMnc){
                            itemContent.updateIcons(textFieldMcc.text, textFieldMnc.text)
                        }
                    }
                }
            }
        }
    }
    onAccepted: {
        if(itemContent.enabledEditingMccMnc){
            providerQml.signalCreateNewOperator(textFieldMcc.text, textFieldMnc.text, textFieldName.text);
        }
        else{
            providerQml.signalChangeOperatorName(itemContent.currentIndex, textFieldMcc.text, textFieldMnc.text, itemContent.operatorName, textFieldName.text);
        }
    }
}
