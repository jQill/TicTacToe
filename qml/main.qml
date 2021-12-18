import QtQuick.Window 2.12
import QtQuick 2.12

Window {
    property bool is_android: Qt.platform.os === "android"
    property int cell_size: is_android ? (Screen.width > Screen.height ? Screen.height : Screen.width) /
                                         gameModel.rowCount() * 1.65 : Screen.width / 40

    property int text_size: 20
    property int button_height: text_size * 2
    property int center: gameModel.rowCount() / 2
    property int board_size: gameModel.rowCount() * cell_size

    property int dead_end: -3
    property int cross_winner: -2
    property int zero_winner: -1
    property int game_beginning: 0
    property int empty: 0
    property int zero: 1
    property int cross: 2
    property int player1_wins: 0
    property int player2_wins: 0
    property int player_wins: 0
    property int bot_wins: 0

    property string lightred: "#F68D93"
    property string lightgreen: "#8CD6AD"
    property string lightblue: "#085873"
    property string light_lightblue: "#0A6482"
    property string darkblue: "#26303A"
    property string cross_color: "#1E252D"
    property string zero_color: "#F2EBD3"

    property bool gomoku_style: false
    property bool pressed_cancel: false
    property bool pressed_clear: false
    property bool pressed_minus: false
    property bool pressed_plus: false
    property bool moves_first: true

    visible: true
    color: darkblue

    width: cell_size * 15
    height: cell_size * 15 + status_bar.height + game_status_qml.height

    Column{
        width: parent.width
        height: parent.height

        Row{
            id: status_bar
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: gameModel.game_status === game_beginning
            Item {
                width: 5
                height: 5
            }

            Rectangle{
                id: player1
                width: player1_text.width + text_size * 3
                height: button_height
                border.width: 2
                border.color: cross_color
                color: moves_first ? cross_color : darkblue
                radius: height / 8
                Text {
                    id: player1_text
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: text_size / 2
                    text: gameModel.vs_bot ? "you" : "player 1"
                    font.pixelSize: text_size
                    color: zero_color
                }
                Text {
                    id: player1_wins_text
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: text_size
                    text: "—"
                    font.pixelSize: text_size
                    color: zero_color
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: if (!moves_first){
                                   moves_first = true
                                   gameModel.setBotPlayer(zero)
                               }
                }
            }
            Item {
                width: Window.width - 2 * menu_button.width - player2.width - player1.width > text_size ?
                           text_size : Window.width - 2 * menu_button.width - player2.width - player1.width
                height: button_height
            }
            Rectangle{
                id: player2
                width: player2_text.width + text_size * 3
                height: button_height
                color: !moves_first ? cross_color : darkblue
                border.width: 2
                border.color: cross_color
                radius: height / 8
                Text {
                    id: player2_text
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: text_size / 2
                    text: gameModel.vs_bot ? "bot" : "player 2"
                    font.pixelSize: text_size
                    color: zero_color
                }
                Text {
                    id: player2_wins_text
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: text_size
                    text: "—"
                    font.pixelSize: text_size
                    color: zero_color
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:if (moves_first){
                                  moves_first = false
                                  if (gameModel.vs_bot){
                                      gameModel.setBotPlayer(cross)
                                      gameModel.botMove()
                                  }
                              }
                }
            }
        }

        Item {
            width: 1
            height: 1
        }

        Rectangle{
            id: game_status_qml
            anchors.horizontalCenter: parent.horizontalCenter
            width: text_size * 12
            height: button_height
            enabled: gameModel.vs_bot && gameModel.game_status === game_beginning
            color: darkblue
            radius: width / 40

            Text {
                id: game_status_text
                anchors.centerIn: parent
                visible: gameModel.game_status === game_beginning ||
                         gameModel.game_status === cross_winner ||
                         gameModel.game_status === zero_winner ||
                         gameModel.game_status === dead_end

                text: switch (gameModel.game_status){
                      case game_beginning:
                          return gameModel.vs_bot ? "First move is yours" : "Choose who's first"
                      case cross_winner:
                          if (moves_first){
                              player1_wins_text.text = gameModel.vs_bot ? ++player_wins : ++player1_wins
                              return gameModel.vs_bot ? "You won" : "Player 1 won"
                          }
                          else {
                              player2_wins_text.text = gameModel.vs_bot ? ++bot_wins : ++player2_wins
                              return gameModel.vs_bot ? "Bot won" : "Player 2 won"
                          }
                      case zero_winner:
                          if (!moves_first){
                              player1_wins_text.text = gameModel.vs_bot ? ++player_wins : ++player1_wins
                              return gameModel.vs_bot ? "You won" : "Player 1 won"
                          }
                          else {
                              player2_wins_text.text = gameModel.vs_bot ? ++bot_wins : ++player2_wins
                              return gameModel.vs_bot ? "Bot won" : "Player 2 won"
                          }
                      case dead_end:
                          return "Draw"
                      default:
                          return "Error"
                      }

                font.pixelSize: text_size
                color: zero_color
            }

            Item{
                visible: gameModel.game_status === cross || gameModel.game_status === zero
                anchors.centerIn: parent
                height: button_height
                width: button_height + turn_text.width + button_height / 10

                Item{
                    visible: !gomoku_style
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    width: button_height
                    height: button_height

                    Rectangle {
                        visible: gameModel.game_status === zero
                        anchors.centerIn: parent
                        implicitWidth: button_height / 1.5
                        implicitHeight: button_height / 1.5
                        color: zero_color
                        radius: width / 2
                        Rectangle{
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.margins: parent.implicitHeight / 7
                            color: darkblue
                            radius: width
                        }
                    }

                    Rectangle{
                        visible: gameModel.game_status === cross
                        anchors.centerIn: parent
                        width: button_height / 1.5
                        height: button_height / 10
                        color: zero_color
                        rotation: -45

                    }
                    Rectangle{
                        visible: gameModel.game_status === cross
                        anchors.centerIn: parent
                        width: button_height / 1.5
                        height: button_height / 10
                        color: zero_color
                        rotation: 45
                    }
                }
                Item{
                    visible: gomoku_style
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    width: button_height
                    height: button_height

                    Rectangle {
                        visible: gameModel.game_status === cross
                        anchors.centerIn: parent
                        implicitWidth: button_height / 1.5
                        implicitHeight: button_height / 1.5
                        color: cross_color
                        radius: width
                    }

                    Rectangle {
                        visible: gameModel.game_status === zero
                        anchors.centerIn: parent
                        implicitWidth: button_height / 1.5
                        implicitHeight: button_height / 1.5
                        color: zero_color
                        radius: width
                    }
                }
                Text {
                    id: turn_text
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    text: "turn"
                    font.pixelSize: text_size
                    color: zero_color
                }
            }
        }

        Item{
            height: parent.height - status_bar.height - game_status_qml.height - board_size > 0 ?
                        (parent.height - status_bar.height - game_status_qml.height - board_size) / 4 : 1
            width: 1
        }

        TableView {
            visible: gomoku_style
            clip: true
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height - status_bar.height - game_status_qml.height > board_size ?
                        board_size : parent.height - status_bar.height - game_status_qml.height
            width: parent.width > board_size ?
                       board_size : parent.width

            model: gameModel

            contentX: (contentWidth - width) / 2;
            contentY: (contentHeight - height) / 2;

            delegate: Rectangle{
                implicitWidth: cell_size
                implicitHeight: cell_size
                color: lightblue
                Rectangle{
                    //up
                    visible: row > 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    implicitWidth: 2
                    implicitHeight: parent.implicitHeight / 2
                    color: gameModel.game_status === game_beginning && row === center && column === center ?
                               lightgreen : darkblue
                }
                Rectangle{
                    //left
                    visible: column > 0
                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: parent.implicitWidth / 2
                    implicitHeight: 2
                    color: gameModel.game_status === game_beginning && row === center && column === center ?
                               lightgreen : darkblue
                }
                Rectangle{
                    //bottom
                    visible: row < gameModel.rowCount() - 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    implicitWidth: 2
                    implicitHeight: parent.implicitHeight / 2
                    color: gameModel.game_status === game_beginning && row === center && column === center ?
                               lightgreen : darkblue
                }
                Rectangle{
                    //right
                    visible: column < gameModel.columnCount() - 1
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    implicitWidth: parent.implicitWidth / 2
                    implicitHeight: 2
                    color: gameModel.game_status === game_beginning && row === center && column === center ?
                               lightgreen : darkblue
                }

                Rectangle{
                    anchors.centerIn: parent
                    visible: model.cell !== empty || model.dangerous
                    radius: implicitWidth / 2
                    implicitWidth: parent.implicitWidth / 1.15
                    implicitHeight: parent.implicitHeight / 1.15
                    border.width: 2
                    border.color: model.cell === cross_winner || model.cell === zero_winner ?
                                      lightgreen : (model.dangerous ? lightred : "transparent")

                    color: switch (model.cell){
                           case cross: return cross_color
                           case zero: return zero_color
                           case cross_winner: return cross_color
                           case zero_winner: return zero_color
                           default:
                               return "transparent"
                           }

                    Text {
                        visible: model.cell !== empty
                        anchors.centerIn: parent
                        font.pixelSize: text_size
                        color: model.cell === cross || model.cell === cross_winner ? zero_color : cross_color
                        text: model.number
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: if (gameModel.setData(gameModel.index(row, column), true) && gameModel.vs_bot)
                                   gameModel.botMove()
                }
            }
        }


        TableView {
            visible: !gomoku_style
            clip: true
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height - status_bar.height - game_status_qml.height > board_size ?
                        board_size : parent.height - status_bar.height - game_status_qml.height
            width: parent.width > board_size ?
                       board_size : parent.width

            model: gameModel

            contentX: (contentWidth - width) / 2;
            contentY: (contentHeight - height) / 2;

            delegate: Rectangle{
                implicitWidth: cell_size
                implicitHeight: cell_size
                border.color: model.cell === cross_winner || model.cell === zero_winner ||
                              gameModel.game_status === game_beginning && row === center && column === center ?
                              lightgreen : model.dangerous ? lightred : darkblue

                border.width: 1
                color: model.color ? light_lightblue : lightblue
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (gameModel.setData(gameModel.index(row, column), true))
                            if (gameModel.vs_bot)
                                gameModel.botMove()
                        if (menu.visible)
                            menu.visible = false
                    }
                }

                Rectangle {
                    visible: model.cell === zero || model.cell === zero_winner
                    anchors.centerIn: parent
                    implicitHeight: parent.implicitHeight / 1.65
                    implicitWidth: parent.implicitWidth / 1.65
                    color: zero_color
                    radius: width
                    Rectangle{
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.margins: parent.implicitHeight / 6
                        color: model.color ? light_lightblue : lightblue
                        radius: width
                    }
                }

                Item {
                    visible: model.cell === cross || model.cell === cross_winner
                    anchors.centerIn: parent
                    Rectangle{
                        anchors.centerIn: parent
                        width: cell_size / 3 * 2
                        height: cell_size / 10
                        color: cross_color
                        rotation: -45

                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: cell_size / 3 * 2
                        height: cell_size / 10
                        color: cross_color
                        rotation: 45
                    }
                }
            }
        }
    }

    MouseArea{
        id: windowMouseArea
        anchors.fill: parent
        enabled: false
        onClicked: {
            menu.visible = false
            enabled = false
        }
    }

    Rectangle {
        id: menu_button
        width: button_height / 1.2
        height: button_height
        radius: height / 8
        color: menu.visible ? cross_color : darkblue

        MouseArea{
            anchors.fill: parent
            onClicked: {
                menu.visible = !menu.visible
                windowMouseArea.enabled = !windowMouseArea.enabled
            }
        }

    }

    Item {
        x: menu_button.width / 2
        y: menu_button.height / 2.2
        Text {
            anchors.centerIn: parent
            font.pixelSize: text_size * 7 / 4
            color: zero_color
            text: "⋮"
        }
    }

    Rectangle{
        id: menu
        visible: false
        y: menu_button.height + 1
        width: text_size * 10
        height: 5 * button_height + 20
        color: darkblue
        border.width: 1
        border.color: cross_color
        radius: width / 30

        MouseArea { anchors.fill: parent }

        Column{
            anchors.fill: parent
            Item {
                width: 10
                height: 10
            }
            Rectangle{
                anchors.horizontalCenter: parent.horizontalCenter
                width: menu.width - 2 * menu.border.width
                height: button_height
                color: pressed_clear ? cross_color : darkblue
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width / 8
                    text: "Restart"
                    font.pixelSize: text_size
                    color: zero_color
                }
                MouseArea{
                    anchors.fill: parent
                    onPressedChanged: pressed_clear = !pressed_clear
                    onClicked: {
                        gameModel.clear()
                        if (gameModel.vs_bot){
                            gameModel.setBotPlayer(zero)
                            moves_first = true
                        }
                    }
                }
            }
            Rectangle{
                anchors.horizontalCenter: parent.horizontalCenter
                width: menu.width - 2 * menu.border.width
                height: button_height
                color: pressed_cancel ? cross_color : darkblue
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width / 8
                    text: "Cancel move"
                    font.pixelSize: text_size
                    color: zero_color
                }
                MouseArea{
                    anchors.fill: parent
                    onPressedChanged: pressed_cancel = !pressed_cancel
                    onClicked: if (gameModel.game_status === 1 || gameModel.game_status === 2){
                                   gameModel.undoMove()
                                   if (gameModel.vs_bot){
                                       gameModel.undoMove()
                                       if (gameModel.game_status === game_beginning){
                                           moves_first = true
                                           gameModel.setBotPlayer(zero)
                                       }
                                   }
                               }
                }
            }
            Rectangle{
                anchors.horizontalCenter: parent.horizontalCenter
                width: menu.width - 2 * menu.border.width
                height: button_height
                color: gomoku_style ? cross_color : darkblue
                enabled: gameModel.game_status === game_beginning
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width / 8
                    text: "Gomoky style"
                    font.pixelSize: text_size
                    color: zero_color
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: gomoku_style = !gomoku_style
                }
            }
            Rectangle{
                anchors.horizontalCenter: parent.horizontalCenter
                width: menu.width - 2 * menu.border.width
                height: button_height
                color: darkblue
                enabled: gameModel.game_status === game_beginning
                Text {
                    id: text_button_size
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width / 8
                    text: "Size: " + gameModel.rowCount()
                    font.pixelSize: text_size
                    color: zero_color
                }
                Rectangle{
                    id: minus_button
                    anchors.left: text_button_size.right
                    anchors.leftMargin: parent.width / 12
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height / 1.1
                    height: width
                    color: pressed_minus ? cross_color : darkblue
                    Text {
                        anchors.centerIn: parent
                        text: "-"
                        font.pixelSize: text_size
                        color: zero_color
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressedChanged: pressed_minus = !pressed_minus
                        onClicked: if (gameModel.resizeBoard(-2)){
                                       board_size -= 2 * cell_size
                                       --center;
                                       text_button_size.text = "Size: " + gameModel.rowCount()
                                   }
                    }
                }
                Rectangle{
                    anchors.left: minus_button.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height / 1.1
                    height: width
                    color: pressed_plus ? cross_color : darkblue
                    Text {
                        anchors.centerIn: parent
                        text: "+"
                        font.pixelSize: text_size
                        color: zero_color
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressedChanged: pressed_plus = !pressed_plus
                        onClicked: if (gameModel.game_status === game_beginning && gameModel.resizeBoard(2)){
                                       board_size += 2 * cell_size
                                       ++center
                                       text_button_size.text = "Size: " + gameModel.rowCount()
                                   }
                    }
                }

            }
            Rectangle{
                anchors.horizontalCenter: parent.horizontalCenter
                width: menu.width - 2 * menu.border.width
                height: button_height
                color: gameModel.vs_bot ? cross_color : darkblue
                enabled: gameModel.game_status === game_beginning
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width / 8
                    text: "Bot"
                    font.pixelSize: text_size
                    color: zero_color
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        gameModel.vs_bot = !gameModel.vs_bot
                        if (gameModel.vs_bot){
                            moves_first = true
                            player1_wins_text.text = player_wins > 0 ? player_wins : '—'
                            player2_wins_text.text = bot_wins > 0 ? bot_wins : '—'
                        }
                        else{
                            player1_wins_text.text = player2_wins > 0 ? player2_wins : '—'
                            player2_wins_text.text = player1_wins > 0 ? player1_wins : '—'
                        }
                    }
                }
            }
        }
    }
}
