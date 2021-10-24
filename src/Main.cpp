#include "domain/DataModel.h"
//#include <QString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

using namespace top15words::domain;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    DataModel model;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataModel", &model);
    //engine.load(QUrl(QStringLiteral("qrc:⁄MainWindow.qml")));
    const QUrl path = QUrl::fromLocalFile("C:/Users/ivche/Documents/Projects/2gis_test/src/ui/MainWindow.qml");
    engine.load(QUrl(path));

    return app.exec();
}
