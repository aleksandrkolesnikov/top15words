#include "Applet.h"
#include "domain/DataModel.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QFile>

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

    QObject::connect(qmlEngine, &QQmlApplicationEngine::quit, this, &Applet::quit, Qt::ConnectionType::QueuedConnection);

    qmlEngine->rootContext()->setContextProperty("dataModel", dataModel);
    qmlEngine->load(QStringLiteral("qrc:/MainWindow.qml"));
}

}
