#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickView>

#include "game_model.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType <GameModel> ("com.harman.game", 1, 0, "GameModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    //
    //GameModel gameModel(6, 10, 1, 9);

    //QQuickView view;
    //QQmlContext *ctxt = view.rootContext();
    //ctxt->setContextProperty("gameModel", &gameModel);
    //view.setSource(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    //view.show();

    //QQmlContext *context = new QQmlContext(engine.rootContext());
    //context->setContextProperty("gameModel", &gameModel);
    //
    //QQmlComponent component(&engine);
    //component.setData("import QtQuick 2.7\nGridView { model: gameModel }", QUrl());
    //component.setData("gameModel", QUrl(QStringLiteral("qrc:/ui/GameField.qml")));
    // //QUrl(QStringLiteral("qrc:/ui/GameField.qml"))
    //component.create(context);
    //

    return app.exec();
}
