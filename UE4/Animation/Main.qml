import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Rectangle{
        id: rect1
        width: 400
        height: 200
        color: "darkblue"
        x: parent.width/2 - width/2
        y: parent.height/2 - height/2

        border.width: 2
        border.color: "gray"
        radius:20
    }

    MouseArea{
        id: area1
        hoverEnabled: true
        anchors.fill: rect1
        onClicked:{
            console.log("area clicked: " + rect1.color);

            if(rect1.color == "#000000"){
                rect1.color = "darkblue"
            }
            else{
                rect1.color = "black"
            }
        }

        onDoubleClicked: {
            console.log("double clicked");
        }

        onEntered:{
            console.log("enter");
        }
        onExited: console.log("exit");
    }


    Rectangle {
            id: rect2
            width: 360
            height: 160
            color: "purple"

            // Anchors: mit centerIn kann man zentrieren
            anchors.centerIn: rect1
            //x: rect1.x + rect1.radius
            //y: rect1.y + rect1.radius

            radius: rect1.radius
            opacity: 0.5

            Text {
                text: "Hello World!"
                padding: 20

                color: "white"
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 16
                anchors.fill: parent
            }
        }

        // Anchors: Relative Positionierung,  Hilfe: "Positioning with Anchors"
        // Zusätzliches graues Rechteck unter rect1 mit Abstand mittels "anchors" positionieren
        Rectangle {
            id: rect3
            width: rect1.width
            height: 50
            color: "lightgrey"
            radius: 20
            border.width: 2
            border.color: "grey"

            anchors.top:  rect1.bottom
            anchors.left: rect1.left
            anchors.topMargin: 10
            // Beachte: Hier haben wir keine absolute Positionierung mit x und y

            //Innerhalb des Rectangles wollen wir nebeneinander 2 Komponenten plazieren
            //Dafür gibt es 2 Möglichkeiten: Mit "Row" oder "RowLayout"
            //   Row ist ein Positioner der nur die Position vorgibt aber nicht die Größe. Hilfe: "Item Positioners"
            //   RowLayout ist ein Layout, das Position und Größe der beinhalteten Elemente definiert. Hilfe: "Qt Quick Layouts Overview"
            //   RowLayout: import QtQuick.Layouts

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 5
                //Slider import QtQuick.Controls
                Slider {
                    id: slider1
                    from:0
                    to:360
                    stepSize: 1
                    Layout.preferredWidth: parent.width/2
                }
                Text {
                    text: slider1.value
                    font.family: "Consolas"
                    font.pointSize: 30
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    Layout.preferredWidth: parent.width/2
                    Layout.fillHeight: true
                }
            }
        }

    Image {
        id: image1
        source: "cat-icon.png"
        anchors.fill: rect2
        fillMode: Image.PreserveAspectFit
        rotation: slider1.value
    }

    Timer{
        running: false
        interval:10
        repeat: true
        onTriggered:{
            image1.rotation +=1
        }
    }

    PropertyAnimation{
        id: imageAnimation
        target: image1
        duration: 5000
        from: 0
        to: 360
        running: true
        loops: Animation.Infinite
        properties: "rotation"

    }

}
