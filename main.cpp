#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "game_model.h"
#include "game_logic.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType <GameModel> ("com.harman.game_model", 1, 0, "GameModel");
    qmlRegisterType <GameLogic> ("com.harman.game_logic", 1, 0, "GameLogic");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
