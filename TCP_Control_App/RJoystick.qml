import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    id: rJoystick
    width: 160
    height: width
    color: "#3f658d"
    radius: width/2

    // Sources des images indiquant la touche appuyée
    property string up_source: "/Images/rJoystick_up.png"  // touche gauche
    property string down_source: "/Images/rJoystick_down.png" // touche droite
    property string none_source: "/Images/rJoystick_none.png"  // aucune touche
    /* Nombre envoyé pour chaque touche */
    property int up_number: 76     // touche gauche
    property int none_number: 78     // aucune touche
    property int down_number: 77     // touche droite

    function upSelected(){
        joystick_image.source = up_source
        console.log("Up")
        backend.data = up_number
    }

    function downSelected(){
        joystick_image.source = down_source
        console.log("Down")
        backend.data = down_number
    }

    function noneSelected(){
        joystick_image.source = none_source
        console.log("None")
        backend.data = none_number
    }

    Image {
        id: joystick_image // Joystick image
        anchors.fill: parent
        source: none_source // initialement, aucune touche appuyée
    }

    Button{
        id: button_up
        width: parent.width;    height: parent.height/2
        anchors.top: parent.top
        opacity: 0
        onPressed: upSelected()
        onReleased: noneSelected()
    }

    Button{
        id: button_down
        width: parent.width;    height: parent.height/2
        anchors.bottom: parent.bottom
        opacity: 0
        onPressed: downSelected()
        onReleased: noneSelected()
    }

    /*MouseArea{
        id: mouseArea
        anchors.fill: parent;

        onPositionChanged:{
            var temp = mouse.y - rJoystick.height/2
            if(temp > 0){
                console.log("Down")
                downSelected()
            }
            else if(temp < 0){
                console.log("Up")
                upSelected()
            }
            else{
                console.log("Middle")
                noneSelected()
            }
        }

        onReleased: {
            noneSelected()
        }

        onPressed:{
            var temp = mouse.y - rJoystick.height/2
            if(temp > 0){
                console.log("Up")
                downSelected()
            }
            else if(temp < 0){
                console.log("Down")
                upSelected()
            }
            else{
                console.log("Middle")
                noneSelected()
            }
        }
    }*/
}
