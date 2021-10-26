#pragma once

#include <QApplication>

class QQmlApplicationEngine;

namespace top15words::domain
{

class DataModel;

}

namespace top15words
{

class Applet final : public QApplication
{
public:
    Applet(int argc, char** argv);

private:
    void initializeQmlEngine();

private:
    QQmlApplicationEngine* qmlEngine;
    domain::DataModel* dataModel;
};

}
