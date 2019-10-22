#ifndef QTPB_H
#define QTPB_H

#include "qtpb_global.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class QTPBSHARED_EXPORT Qtpb
{

public:
    static void initApplication(QGuiApplication &app, QQmlApplicationEngine &engine);
    static void loadMainQml(QQmlApplicationEngine &engine, QString app_name);
private:
    Qtpb();
};

#endif // QTPB_H
