#include "Applet.h"
#include <QQmlContext>

namespace top15words
{

Applet::Applet(int argc, char** argv)
    : QApplication{argc, argv}
{
    qRegisterMetaType<domain::WordSet>();

    initializeQmlEngine();
}

void Applet::initializeQmlEngine()
{
    qmlEngine.rootContext()->setContextProperty("dataModel", &dataModel);
    qmlEngine.load(QStringLiteral("qrc:/MainWindow.qml"));

    QObject::connect(&qmlEngine, &QQmlApplicationEngine::quit, this, &Applet::quit,
                     Qt::ConnectionType::QueuedConnection);
}

}
