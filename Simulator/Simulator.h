#pragma once
#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <QObject>
#include <QTimer>
#include "Communication/CommunicationTcpSocketServer.h"
#include "RobotState.h"

/**
 * @brief A robot szimulátor.
 *
 * Van egy belső, RobotState típusú állapota, melyet egy QTimer segítségével periodikusan frissít.
 * Létrehoz egy CommunicationTcpSocketServer objektumot a kommunikációhoz, amihez lehet csatlakozni.
 * Minden szimulációs lépés után elküldi az állapotát.
 * Ha egy RobotState objektumot kap, azt parancsként értelmezi.
 */
class Simulator : public QObject
{
    Q_OBJECT

public:
    /** Konstruktor.
     * @param communication A Communication osztály egy példánya, amin a kommunikáció zajlik az alkalmazással.
    */
    explicit Simulator(Communication *communication);
    ~Simulator() = default;

    /** Elindítja a szimulátort.
     * @param intervalSec   A szimulátor periódusideje.
     */
    void start(float intervalSec);

protected:
    /** Alapértékre állítja a belső paramétereket.
     * @param intervalSec   A szimulátor periódusideje.
     */
    void reset(float intervalSec);

    /** Belső szerver a kommunikációhoz. */
    Communication *communication;

    /** Időzítő a tick() metódus periodikus hívására. */
    QTimer timer;

    /** Időzítő az önteszthez. A selfTestTick() metódust periodikusan hívja. */
    QTimer selfTestTimer;


    /** A periódus idő (másodpercben). */
    float dt;

    /** A timer aktuális tick ideje msben */
    float currentTimerTickInterval = 1000.0F;

    /** A szimulátor pillanatnyi állapota. */
    RobotState state;

    /** Számláló az önteszt állapotának követéséhez */
    int selfTestProcessCounter = 0;

    /** Elindítja az öntesztet */
    void startSelfTest();

protected slots:
    /** A timer hívja meg, meghatározza a robot
     * állapotát a következő időpillanatban. */
    void tick();

    /** A selfTestTimer hívja meg az önteszt folyamán. */
    void selfTestTick();

    /**
     * @brief Új üzenet fogadását jelzi.
     * @param inputStream A csatorna, amiben az üzenetek jöttek.
     */
    void dataReady(QDataStream& inputStream);
};

#endif // SIMULATOR_H
