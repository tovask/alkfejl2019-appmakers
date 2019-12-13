#pragma once
#ifndef DCAPPLICATION_H
#define DCAPPLICATION_H
#include <QApplication>
#include <QQmlApplicationEngine>
#include "Simulator/Simulator.h"
#include "MainWindowsEventHandling.h"
#include "RobotProxy.h"
#include "Communication/CommunicationTcpSocketClient.h"
#include "RobotStateHistory.h"

/**
 * @brief Alkalmazás osztály. A main() példányosítja és indítja el.
 *
 * A konstruktor elvégzi az alapvető példányosításokat és a signalok összekapcsolását.
 */
class DCApplication : public QApplication
{
public:
    /** Konstruktor. Alapvető példányosítások és signal bekötések. */
    DCApplication(int &argc, char *argv[]);
    ~DCApplication() = default;

private:
    std::unique_ptr<CommunicationTcpSocketServer> serverSocket;
    Simulator simulator;
    QQmlApplicationEngine engine;
    RobotStateHistory history;
    CommunicationTcpSocketClient communication;
    RobotProxy robot;
    MainWindowsEventHandling handler;
};

#endif // DCAPPLICATION_H
