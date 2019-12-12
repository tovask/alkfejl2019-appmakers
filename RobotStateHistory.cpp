#include "RobotStateHistory.h"
#include "RobotState.h"

RobotStateHistory::RobotStateHistory()
    : QObject(nullptr), currentState(nullptr)
{
}

void RobotStateHistory::Add(const RobotState& state)
{
    std::unique_ptr<RobotState> newState = std::unique_ptr<RobotState>(new RobotState());
    newState->CopyFrom(state);
    stateList.append(newState.get());
    currentState = (RobotState*)newState.get(); // This has to be done before moving ownership
    container.push_back(std::move(newState));



    // Összeállítjuk a grafikon számára az adatszerkezetet.
    // Ezek a listák QList<int> típusú listák, amiket direkt arra hozunk létre, hogy a QML elemek
    //  ezekből olvassák ki az adatokat, mivel erre a fenti container nem alkalmas. (Az std::vector
    //  nem támogatja a Qt metaobject rendszerét.)
    graphTimestamps.clear();
    graphVelocities.clear();
    graphCarHeights.clear();
    int graphStateNumber = stateList.size() < shownStateNumber ? stateList.size() : shownStateNumber;
    auto it = container.end()-graphStateNumber;
    qDebug() << container.size();
    for(;it!=container.end();++it)
    {
        RobotState *currentState = it->get();
        graphTimestamps.append(currentState->timestamp());
        graphVelocities.append(currentState->v());
        graphCarHeights.append(currentState->height());
        actualPressures = currentState->pressures();
        carCurrentHeight = currentState->height();
    }

    // Jelezzük a history változását.
    emit historyChanged();
}

void RobotStateHistory::AddOnlyForHeight(const RobotState& state)
{
    std::unique_ptr<RobotState> newState = std::unique_ptr<RobotState>(new RobotState());
    newState->CopyFrom(state);
    currentState = (RobotState*)newState.get(); // This has to be done before moving ownership
    RobotState* currentStateToAdd = (RobotState*)newState.get(); // This has to be done before moving ownership
    graphCarHeights[graphCarHeights.length() - 1] = currentStateToAdd->height();
    carCurrentHeight = currentStateToAdd->height();

    // Jelezzük a history változását.
    emit heightChanged();
}
