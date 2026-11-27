import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    Rectangle {
        id : rect1
        color: "blue"
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#28ee92";
            }
            GradientStop {
                position: 1.00;
                color: "#ffffff";
            }
        }
    }

    Rectangle{
        id: rect2
        width: 400
        height: 200
        color: "#dark blue"
        x: parent.width/2 - width/2
        y: parent.height/2 - height/2


        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#ca2020";
            }
            GradientStop {
                position: 1.00;
                color: "#ffffff";
            }
        }

        border.width: 2
        border.color: "gray"
        radius:20
    }

    Rectangle {
        id: rect3
        width: 360
        height: 160
        color: "purple"
        anchors.centerIn: rect2
        radius: 20
        opacity : 0.5

        Text {
            text : "Hello World"
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 16
            padding: 20
            anchors.fill: parent
        }
    }

    // Relative Positionierung
    Rectangle{
        id: slider
        width: rect2.width
        height: 50
        color: "lightgray"
        anchors.top: rect2.bottom
        anchors.left: rect2.left
        anchors.topMargin: 10


        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#ca2020";
            }
            GradientStop {
                position: 1.00;
                color: "#ffffff";
            }
        }

        border.width: 2
        border.color: "gray"
        radius:20

        RowLayout{
            anchors.fill: parent;
            anchors.leftMargin: 20;
            anchors.rightMargin: 20;

            Slider{
                id: slider1
                from: 1
                to: 100
                value :0
                stepSize: 1
                Layout.preferredWidth: parent.width* 0.75
            }
            Text{
                font.family: "Consolas"
                font.pixelSize: 30
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text : slider1.value
                Layout.preferredWidth: parent.width * 0.25
                Layout.fillHeight: true
            }
        }
    }
}
