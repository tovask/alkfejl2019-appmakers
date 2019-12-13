#pragma once
#ifndef ROBOTSTATEHISTORY_H
#define ROBOTSTATEHISTORY_H
#include <QtCore>
#include <memory>
#include <vector>
#include "RobotState.h"

/**
 * @brief Visszamenőleg tárolja a robot állapotait,
 * valamint innen lehet lekérdezni a legutóbbi érvényes állapotot is.
 */
class RobotStateHistory : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor.
     */
    RobotStateHistory();
    virtual ~RobotStateHistory() = default;

    /**
     * Állapotok listája, melyet a QML oldalhoz lehet kötni.
     * Ez a lista csak pointereket tárol a container-ben lévő elemekre.
     *
     * @warning Ennek QObject* listának kell lennie. A leszármaztatott osztályokra
     * mutató pointereket nem ismeri fel a QML adatkötés.
     */
    QList<QObject*> stateList;

    /** Mutató a legutolsó érvényes állapotra. Az add() frissíti. */
    RobotState *currentState;

    /** Ez a tároló tárolja ténylegesen az állapotokat. (Övé az ownership.) */
    std::vector<std::unique_ptr<RobotState>> container;

    /**
     * @brief Új állapot eltárolása.
     *
     * A paraméterül kapott állapot egy másolatát a history végéhez fűzi és aktuálissá teszi.
     * @param state Az új RobotState
     */
    void Add(const RobotState& state);

    /**
     * @brief Új magasság eltárolása.
     *
     * A paraméterül kapott állapotból frissíti a magasságot a historyban, a többit változatlanúl hagyja,
     *  és aktuálissá teszi ezt az historyt.
     * @param state Az új RobotState
     */
    void AddOnlyForHeight(const RobotState& state);



    /** \addtogroup Tárolók a követlen megjelenítéshez.
     * Csak a legutolsó shownStateNumber számú elemet tárolják.
     * Az Add() frissíti.
     *  @{
     */

    /** Az időbélyeg múltbeli értékei */
    QList<int> graphTimestamps;
    /** A sebesség múltbeli értékei */
    QList<int> graphVelocities;
    /** A gyorsulás múltbeli értékei */
    QList<int> graphAccelerations;
    /** A magasság múltbeli értékei. */
    QList<int> graphCarHeights;
    /** A nyomás múltbeli értékei */
    QList<double> actualPressures = QList<double>() << 4 << 4;
    /** Az aktuális magasság */
    int carCurrentHeight = 12;

    /** @} */

    /** A megjelenítendő állapotok száma. */
    const int shownStateNumber = 200;

signals:
    /** Add() után kiadott signal. */
    void historyChanged();
    /** AddOnlyForHeight() után kiadott signal. */
    void heightChanged();
};

#endif // ROBOTSTATEHISTORY_H
