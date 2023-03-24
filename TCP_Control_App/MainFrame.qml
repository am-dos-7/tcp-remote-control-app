import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import io.qt.examples.backend 1.0

Rectangle{
    id: mainFrame
    width: 640;    height: 500
    //anchors.fill: parent
    readonly property int width_unit: width/16 // Unité suivant x
    readonly property int height_unit: height/9 // Unité suivant y
    color: "gray"

    BackEnd{
        id: backend
        onConnectionStateChanged: {
            if(connection_state){
                etat_connexion.color = etat_connexion.couleur_connectee
            }
            else
                etat_connexion.color = etat_connexion.couleur_non_connectee
        }

        onFrameAvailable: {
            /*
             * A ce niveau, le chemin pose un petit souci. Si on lance l'application depuis QtCreator, l'image est enregistrée
             * dans un repertoire avant l'exécutable (../), alors qu'une fois que l'application est déployée, l'image est enregistrée
             * dans le même dossier que l'exécutable. Il faudra donc faire en sorte de gérer les deux cas à travers un contrôle
             * (un define par exemple).
             */
            if(!image_index)
                video.source = android_build ? video.source = "file://" + applicationDirPath +"/image0.jpg" : video.source = "file:///" + applicationDirPath +"/../image0.jpg"
                //video.source = "file:///" + applicationDirPath +"/../image0.jpg"  // Pour Desktop
                //video.source = "file://" + applicationDirPath +"/image0.jpg"        // Pour Android
            else
                video.source = android_build ? video.source = "file://" + applicationDirPath +"/image1.jpg" : video.source = "file:///" + applicationDirPath +"/../image1.jpg"
                //video.source = "file:///" + applicationDirPath +"/../image1.jpg"  // Pour Desktop
                //video.source = "file://" + applicationDirPath +"/image1.jpg"        // Pour Android
        }
    }

    Item {
        anchors.fill: parent
        id: item_accueil
        visible: true

        Rectangle{ /* Ouvre le menu de connexion/configuration */
            id: boutonMenu
            //background: Rectangle{color: "#3f658d"}
            width: width_unit; height: width
            anchors.top: parent.top;    anchors.topMargin: 2
            anchors.left: parent.left;  anchors.leftMargin: 2
            Image {
                id: name
                source: "/Images/menu.png"
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                   mainFrame.state = "State_config"   // ouvrir menu de configuration
                    //console.log("Etat changé")
                }
            }

        }

        Rectangle{
            id: camera_vue  /* Sera utilisée ultérieurement pour le retour caméra; peut être désactivé pour le moment */
            color: "black"
            anchors.top: parent.top;    //anchors.topMargin: width_unit
            anchors.left: parent.left;   anchors.leftMargin: width_unit*4
            anchors.right: parent.right;  anchors.rightMargin: width_unit*4
            width: mainFrame.width
            height: mainFrame.height
//            height: height_unit*4


            Image {
                anchors.centerIn: parent
                id: video
                source: "/Images/Logo_ENSEM.gif"        //"file:///" + applicationDirPath +"/../icons/logo.jpg"
                sourceSize.width: camera_vue.width
                sourceSize.height: camera_vue.height
            }
        }

        LJoystick{
            width: width_unit*3; height: width
            //main_color: mainFrame.color
            anchors.bottom: parent.bottom
            anchors.bottomMargin: height_unit
            anchors.left: parent.left
            anchors.leftMargin: width_unit
        }

        RJoystick{
            width: width_unit*3; height: width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: height_unit
            anchors.right: parent.right
            anchors.rightMargin: width_unit
        }

        Rectangle{
            id: etat_connexion // montre l'état de la connexion: couleur verte: connexion établie, rouge sinon
            property string couleur_connectee: "green"  // couleur utilisée en cas de connexion au serveur
            property string couleur_non_connectee : "#5a1515"   // couleur si déconnecté
            color: couleur_non_connectee    // non connecté par défaut
            anchors.bottom: parent.bottom
            anchors.left: parent.left;   anchors.leftMargin: width_unit*4
            anchors.right: parent.right;  anchors.rightMargin: width_unit*4
            height: height_unit
        }
    }

    Rectangle {
        id: config_frame
        color: parent.color // même couleur que mainFrame
        anchors.fill: parent
        visible: false
        Rectangle{
            id: left_config_frame
            width: parent.width/2
            height: parent.height
            anchors.left: parent.left
            //anchors.top: parent.top
            color: "lightcyan"

            TextField {
                id: ip_addr
                width: width_unit*6
                height: height_unit
                //placeholderText: "I.P Address"
                text: qsTr("192.168.43.227")
//                text: qsTr("192.168.43.88")
//                text: qsTr("10.115.241.200")
//                text: qsTr("10.0.2.15")



                anchors.left: parent.left
                anchors.top: parent.top
                font.pixelSize: 12 //width_unit*1.5
                anchors.leftMargin: width_unit
                anchors.topMargin: height_unit
            }

            SpinBox {
                id: port_number
                //displayText: "Port"
                value: 50885
                from: 1
                to: 65535
                editable: true
                width: width_unit*3
                height: height_unit
                anchors.left: parent.left
                anchors.top: ip_addr.bottom
                font.pixelSize: 12 //width_unit*1.5
                anchors.leftMargin: width_unit
                anchors.topMargin: height_unit
            }

            Button {
                id: button_connect
                //y: 308
                text: qsTr("Connect")
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0 //width_unit
                anchors.bottomMargin: height_unit

//                width: parent.width/3.1;

                onClicked: { /* Connect to the server */
                    backend.serverAddress = ip_addr.text
                    backend.serverPort = port_number.value
                    backend.connectToServer = 1             /* Connexion au serveur */
                    mainFrame.state = ""                    /* Retour au menu principal */
                }
            }

            Button {
                id: button_cancel
                x: 236
                y: 440
                //y: 307
                text: qsTr("Cancel")
//                anchors.right: button_disconnect.left
                anchors.left: parent.left
                anchors.bottom: parent.bottom
//                anchors.rightMargin: width_unit
                anchors.leftMargin: parent.width/2 - width/2
                anchors.bottomMargin: height_unit
                anchors.centerIn: parent;

//                width: parent.width/3.1;

                onClicked: {
                    mainFrame.state = ""
                }
            }

            Button{
                id: button_disconnect
                text: qsTr("Disconnect")
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 0 //width_unit
                anchors.bottomMargin: height_unit

//                width: parent.width/3.1;

                onClicked: {
                    backend.connectToServer = 0 /* Déconnexion du serveur */
                    mainFrame.state = ""        /* Retour au menu principal */
                }

            }
        }

    }


    states: [
        State {
            name: "State_config"

            PropertyChanges {
                target: mainFrame
                visible: true
            }

            PropertyChanges {
                target: item_accueil
                visible: false
            }

            PropertyChanges {
                target: config_frame
                visible: true
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "State_config"
            SequentialAnimation{
                PropertyAnimation{
                    target: item_accueil
                    properties: "visible"
                    duration: 100   // ms
                    easing.type: Easing.OutCubic
                }

                PropertyAnimation{
                    target: config_frame
                    properties: "visible"
                    duration: 100
                    easing.type: Easing.InCubic
                }
            }
        },

        Transition {
            from: "State_config"
            to: ""
            SequentialAnimation{
                PropertyAnimation{
                    target: item_accueil
                    properties: "visible"
                    duration: 100   // ms
                    easing.type: Easing.InCubic
                }

                PropertyAnimation{
                    target: config_frame
                    properties: "visible"
                    duration: 100
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]
}
