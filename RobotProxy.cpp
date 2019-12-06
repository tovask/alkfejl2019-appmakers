#include "RobotProxy.h"
#include "RobotStateHistory.h"
#include "Communication/Communication.h"
#include "RobotState.h"

RobotProxy::RobotProxy(RobotStateHistory& history, Communication &communication)
    : history(history), communication(communication)
{
    // Jelzést kérünk, ha új robot állapot (RobotState) érkezik.
    QObject::connect(&communication, SIGNAL(dataReady(QDataStream&)), this, SLOT(dataReady(QDataStream&)));
}

void RobotProxy::dataReady(QDataStream &stream)
{
    // Új robot állapot érkezett, elmentjük a historyba.
    //  (Onnan vesszük majd azt is, hogy mi az aktuális állapot.)
    RobotState state;
    state.ReadFrom(stream);
    history.Add(state);
}

void RobotProxy::reset()
{
    // Reset parancs küldése.
    RobotState newState;
    newState.setStatus(RobotState::Status::Reset);
    communication.send(newState);
    qDebug() << "Reset parancs elküldve.";
}

void RobotProxy::accelerate()
{
    RobotState newState;
    newState.setStatus(RobotState::Status::Accelerate);
    newState.setA(1);
    communication.send(newState);
    qDebug() << "Gyorsítási parancs elküldve.";
}

void RobotProxy::stop()
{
    RobotState newState;
    newState.setStatus(RobotState::Status::Stopping);
    communication.send(newState);
    qDebug() << "Stop parancs elküldve.";
}

void RobotProxy::selfTest()
{
    RobotState newState;
    newState.setStatus(RobotState::Status::SelfTest);
    communication.send(newState);
    qDebug() << "SelfTest parancs elküldve.";
}


void RobotProxy::changeHeightTo(int height)
{
    RobotState newState;
    newState.setStatus(RobotState::Status::HeightAdjust);
    newState.setHeight(height);
    communication.send(newState);
    qDebug() << "Magasság változtatás parancs elküldve.";
}
