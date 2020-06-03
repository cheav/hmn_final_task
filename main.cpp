#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "game_model.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType <GameModel> ("com.harman.game", 1, 0, "GameModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
