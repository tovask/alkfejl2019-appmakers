import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    width: 500
    height: 500
    anchors.fill: parent

    // Signalok, melyek a kiadott parancsokat jelzik és a nyomógombok
    //  eseménykezelői aktiválják őket.
    signal resetCommand;
    signal accelerateCommand;
    signal stopCommand;
    signal selfTestCommand;
    signal carHeightCommand(real height);


    Item {
        id: graphGB
        // Két oldalra kitölti a szülőt, felfele a szülőhoz illeszkedik, lefele a currentValuesGB-hez.
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: currentValuesGB.top
        anchors.bottomMargin: 20

        HistoryGraph {
            id: historyGraph
            // Kitölti a szülőt
            anchors.fill: parent
            // Az objectName akkor jó, ha C++ oldalról kell megkeresnünk egy QML oldalon definiált
            //  objektumot a findChild metódus rekurzív hívásaival.
            objectName: "historyGraph"

            // Ezek pedig a HistoryGraph tulajdonságai, amiket majd ott definiálunk,
            //  itt pedig értéket adunk nekik. Az alábbi változókat (pl. historyGraphTimestamps)
            //  szintén a MainWindowsEventHandling::historyChanged metódus teszi elérhetővé
            //  a QML oldal számára.
            // Ezek az értékek C++ oldalon folyamatosan változnak. Minden változás esetén
            //  lefut a MainWindowsEventHandling::historyChanged és ezeket újraregisztrálja a QML
            //  oldal számára, így frissülni fog a HistoryGraph tulajdonság is.
            graphTimestamps: historyGraphTimestamps
            graphVelocities: historyGraphVelocity
            graphAccelerations: historyGraphAcceleration
            carHeight: historyCarHeight
        }
    }


    GroupBox {
        id: currentValuesGB
        title: "Pillanatnyi értékek"
        // Balra illeszkedik a szülőhöz, lent a commandsGB-hez. Szélessége és magassága fix.
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: commandsGB.top
        anchors.bottomMargin: 10
        width: parent.width/2

        ColumnLayout {
            // Felfelé, lefelé és balra a szülő széléhez illeszkedik. Jobbra nem, mert
            //  széthúzni felesleges őket.
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            // Sima szövegek (Text elemek), amiknek az értéke egy a C++ oldalon definiált currentState
            //  értékétől függ. (Ha az értéke null, akkor "?" jelenik meg.)
            // A currentState-et a MainWindowsEventHandling::historyChanged metódus regisztrálja be, hogy
            //  látható legyen a QML oldalról is. (Hivatkozás a RobotStateHistory::currentState-re.)
            Text { text: " Állapot:\t" + (currentState!=null ? currentState.statusName : "?") }
            Text { text: " Idő:\t" + (currentState!=null ? currentState.timestamp : "?") }
            Text { text: " X:\t" + (currentState!=null ? currentState.x.toFixed(3) : "?") }
            Text { text: " V:\t" + (currentState!=null ? currentState.v.toFixed(3) : "?") }
            Text { text: " A:\t" + (currentState!=null ? currentState.a.toFixed(3) : "?") }
            Text { text: " Lámpa:\t" + (currentState!=null ? currentState.light.toString() : "?") }
        }

    }


    GroupBox {
        id: logGB
        title: qsTr("Log")
        anchors.top: currentValuesGB.top
        anchors.topMargin: currentValuesGB.topMargin
        anchors.bottom: currentValuesGB.bottom
        anchors.bottomMargin: currentValuesGB.bottomMargin
        anchors.left: currentValuesGB.right
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 10

        ScrollView {
            anchors.fill: parent
            // A scrollohzató elem igazítása a szölő RowLayouthoz.
            // Itt a ScrollViewon belül adjuk meg, hogy a RowLayoutban
            //  mik legyenek a rá (ScrollViewra) vonatkozó méret beállítások,
            //  mert ezeket a RowLayout kezeli ebben az esetben.

            // Itt jön a tényleges lista.
            ListView {
                id: stateHistoryList
                // A model az, ahonnan az adatokat vesszük.
                // A historyModel változót a MainWindowsEventHandling::historyChanged metódus teszi
                //  elérhetővé a QML oldalon is.
                //  Típusa QList<QObject*>, a tárolt pointerek valójában RobotState-ekre mutatnak.
                model: historyModel
                // A delegate megadása, vagyis hogy egy listaelem hogyan jelenjen meg.
                //  (Már fentebb definiáltuk.)
                delegate: stateDelegate

                // Eseménykezelő, az elemek darabszámának változása esetén a kijelölést
                //  a legalsó elemre viszi. Ezzel oldja meg, hogy folyamatosan scrollozódjon
                //  a lista és a legutoló elem mindig látható legyen.
                onCountChanged: {
                    stateHistoryList.currentIndex = stateHistoryList.count - 1;
                }
            }
        }
    }

    GroupBox {
        id: commandsGB
        anchors.left : parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        title: "Parancsok"

        RowLayout {
            id: commandsRow
            spacing: 30
            anchors.left : parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5

            // A nyomógombokat oszlopba rendezzük
            ColumnLayout {
                id: commandButtons

                // Reset nyomógomb. Oldal irányba kitöltik a szülőt, 0 pixel margó kihagyásával.
                //  Megnyomása esetén (Button.Clicked signal) meghívja a resetCommand signalt. (Ez
                //  a signal látható innen, mivel a Button egyik ősében definiáltuk.)
                Button {
                    id: resetBtn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Reset")
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    onClicked: resetCommand()
                }

                Button {
                    id: accelerateBtn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Gyorsítás")
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    onClicked: accelerateCommand()
                }
            }

            ColumnLayout {
                id: heightSlider
                Layout.fillWidth: true

                Text {
                    id: heightSliderText
                    text: "Magasság:"
                    font.pixelSize: 12
                }

                Slider{
                    id: ySlider
                    Layout.fillWidth: true
                    minimumValue: 1
                    maximumValue: 24
                    visible: true
                    value: 12
                    onValueChanged: {
                        carHeightCommand(value)
                    }
                }


                RowLayout {
                    id: heightLabels
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    Label {
                        id: heightLabelslabelLow
                        text: qsTr("Alacsony")
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }

                    Label {
                        id: heightLabelslabelHight
                        text: qsTr("Magas")
                        anchors.rightMargin: 0
                        anchors.right: parent.right
                    }
                }
            }

            // A nyomógombokat oszlopba rendezzük
            ColumnLayout {
                id: commandButtons2

                Button {
                    id: stopBtn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Stop")
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    onClicked: stopCommand()
                }
                Button {
                    id: selfTestBtn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Self test")
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    onClicked: selfTestCommand()
                }
            }
        }
    }


    Component {
        // Delegate: this is the appearance of a list item
        // A későbbi history listának szüksége van egy delegate-re. Minden lista elem ennek a komponensnek egy
        //  példánya lesz.

        // ID, hogy tudjuk a listánál hivatkozni erre, mint a lista delegatejére.
        id: stateDelegate
        Row {
            // Itt a model az, ami a list egyik eleme. (Bármi is legyen majd az.)
            Text { text: model.statusName }
            Text { text: " X: " + model.x.toFixed(3) }
            Text { text: " V: " + model.v.toFixed(3) }
            Text { text: " A: " + model.a.toFixed(3) }
            Text { text: " H: " + model.height }
        }
    }

}
