#pragma once

#include "domain/DataModel.h"
#include <QApplication>
#include <QQmlApplicationEngine>

namespace top15words
{

class Applet final : public QApplication
{
public:
    Applet(int argc, char** argv);

private:
    void initializeQmlEngine();

private:
    QQmlApplicationEngine qmlEngine;
    domain::DataModel dataModel;
};

}
