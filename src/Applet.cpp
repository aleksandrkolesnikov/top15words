#include "Applet.h"
#include "domain/DataModel.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace top15words
{

Applet::Applet(int argc, char** argv)
    : QApplication{argc, argv}
    , qmlEngine{new QQmlApplicationEngine{this}}
    , dataModel{new domain::DataModel{this}}
{
    qRegisterMetaType<domain::WordSet>();

    initializeQmlEngine();
}

void Applet::initializeQmlEngine()
{
    Q_ASSERT(qmlEngine);
    Q_ASSERT(dataModel);

    qmlEngine->rootContext()->setContextProperty("dataModel", dataModel);
    //qmlEngine->load(QStringLiteral("qrc:/MainWindow.qml"));
    const QUrl path = QUrl::fromLocalFile("C:/Users/ivche/Documents/Projects/2gis_test/src/ui/MainWindow.qml");
    qmlEngine->load(path);

}

}
