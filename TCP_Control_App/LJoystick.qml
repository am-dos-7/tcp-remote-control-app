import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    id: lJoystick
    width: 160
    height: width
    radius: width/2
    color: "#3f658d"


    // Sources des images indiquant la touche appuyée
    property string left_source: "/Images/lJoystick_left.png"  // touche gauche
    property string right_source: "/Images/lJoystick_right.png" // touche droite
    property string none_source: "/Images/lJoystick_none.png"  // aucune touche

    /* Nombre envoyé pour chaque touche */
    property int left_number: 72     // touche gauche
    property int none_number: 74     // aucune touche
    property int right_number: 73     // touche droite

    /* Evite d'envoyer à nouveau la commande d'une touche qui n'a pas été relachée: puisque si la position change */
    property bool left_sent: false
    property bool right_sent: false


    function leftSelected(){
        joystick_image.source = left_source // On actualise l'image
        backend.data = left_number          // et on envoie la commande correspondante
        left_sent = true
        right_sent = false
    }

    function rightSelected(){
        joystick_image.source = right_source
        backend.data = right_number
        right_sent = true
        left_sent = false
    }

    function noneSelected(){
        joystick_image.source = none_source
        backend.data = none_number
        left_sent = false
        right_sent = false
    }

    Image {
        id: joystick_image // Joystick image
        anchors.fill: parent
        source: none_source // initialement, aucune touche appuyée
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent;
        onPositionChanged:{
            var temp = mouse.x - lJoystick.width/2
            if(temp > 0){
                if(!right_sent){
                    console.log("Right")
                    rightSelected()
                }
            }
            else if(temp < 0){
                if(!left_sent){
                    console.log("Left")
                    leftSelected()
                }
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
            var temp = mouse.x - lJoystick.width/2
            if(temp > 0){
                console.log("Right")
                rightSelected()
            }
            else if(temp < 0){
                console.log("Left")
                leftSelected()
            }
            else{
                console.log("Middle")
                noneSelected()
            }
        }
    }
}
