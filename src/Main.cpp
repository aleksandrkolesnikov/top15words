#include "Applet.h"

using namespace top15words::domain;

int main(int argc, char** argv)
{
    top15words::Applet applet(argc, argv);

    return applet.exec();
}
