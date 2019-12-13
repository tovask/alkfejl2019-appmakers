#include <QDebug>
#include <QQmlContext>
#include "DCApplication.h"

int main(int argc, char *argv[])
{
    DCApplication app(argc, argv);
    return app.exec();
}
