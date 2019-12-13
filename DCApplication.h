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
    /**
     * @brief Konstruktor.
     *
     * Alapvető példányosítások és signal bekötések.
     * @param argc
     * @param argv
     */
    DCApplication(int &argc, char *argv[]);
    ~DCApplication() = default;

private:
    /**  */
    std::unique_ptr<CommunicationTcpSocketServer> serverSocket;

    /** A Simulator példánya. */
    Simulator simulator;

    /** A QQmlApplicationEngine motorja az alkalmazásnak. */
    QQmlApplicationEngine engine;

    /**
     * @brief A RobotStateHistory példányát tárolja
     *
     * Ez tartalmazza a robot múltbeli állopotát,
     * aminek segítségével a kirajzolás történik.
     */
    RobotStateHistory history;

    /** A kommunikációt lebonyolító CommunicationTcpSocketClient objektum. */
    CommunicationTcpSocketClient communication;

    /** A proxy a robot felé a RobotProxy osztállyal. */
    RobotProxy robot;

    /** MainWindowsEventHandling osztály tartalmazza az eseménykezelőket. */
    MainWindowsEventHandling handler;
};

#endif // DCAPPLICATION_H
