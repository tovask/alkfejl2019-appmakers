#include "Simulator.h"

Simulator::Simulator(Communication *communication)
    : communication(communication), state()
{
    // Fogadni akarjuk a parancsokat
    connect(communication, SIGNAL(dataReady(QDataStream&)), this, SLOT(dataReady(QDataStream&)));
    // Periodikusan futnia kell a szimulációnak
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    connect(&selfTestTimer, SIGNAL(timeout()), this, SLOT(selfTestTick()));


}


void Simulator::start(float intervalSec)
{
    dt = intervalSec;
    state.setTimestamp(0);
    reset(intervalSec);
    timer.start(1000.0F);
    QList<double> resetPressures({4,4});
    state.setPressures(resetPressures);
    state.setStatus(RobotState::Status::SelfTest);
}

void Simulator::reset(float intervalSec)
{
    dt = intervalSec;
    state.setStatus(RobotState::Status::Default);
    state.setX(0.0F);
    state.setV(0.0F);
    state.setLight(0);
    state.setHeight(12);
    QList<double> resetPressures({4,4});
    state.setPressures(resetPressures);
    timer.stop();
}

void Simulator::tick()
{
    // Fizikai szimuláció
    state.setTimestamp(state.timestamp() + dt);
    state.setX(state.x() + state.v()*dt);
    state.setV(state.v());

    if (state.v()<-10.0)
    {
        state.setV( -10.0F );
    }
    if (state.v()>10.0)
    {
        state.setV( 10.0F );
    }

    state.setLight( state.v()>=10.0F ? 1.0F : 0.0F );

    // Magasabb szintű funkciók
    switch(state.status())
    {
    case RobotState::Status::Default:
        break;
    case RobotState::Status::Reset:
        qDebug() << "Simulator: Reset";
        reset(dt);
        break;
    case RobotState::Status::SelfTest:
        qDebug() << "Selftest: Starting";
        this->startSelfTest();
        timer.stop();
        break;
    case RobotState::Status::Stopping:
        if (state.v() > 1.5F)
        {
            qDebug() << "Simulator: Stop parancs, gyors lassítás";
            state.setV(state.v()-1.0F);
        }
        else if (state.v() > 0.1F)
        {
            qDebug() << "Simulator: Stop parancs, lassú lassítás";
            state.setV(state.v()-0.5F);
        }
        else if (state.v() < -1.5F)
        {
            qDebug() << "Simulator: Stop parancs, gyorsítás előre";
            state.setV(state.v()+1.0F);
        }
        else if (state.v() < -0.1F)
        {
            qDebug() << "Simulator: Stop parancs, lassú gyorsítás előre";
            state.setV(state.v()+0.05F);
        }
        else
        {
            // Majdnem megállt
            qDebug() << "Simulator: Megállt.";
            state.setStatus(RobotState::Status::Default);
            state.setV(0.0F);
            timer.stop();
        }
        currentTimerTickInterval += 60;
        timer.setInterval(currentTimerTickInterval);
        break;
    case RobotState::Status::Accelerate:
        qDebug() << "Simulator: Sebesség növelése.";
        state.setStatus(RobotState::Status::Default);
        state.setV(state.v()+1);
        currentTimerTickInterval -= 60;
        timer.setInterval(currentTimerTickInterval);
        break;
    case RobotState::Status::HeightAdjust:
        // Megjegyzés: a magasság kért értékét már a parancs fogadásakor beállítottuk
        qDebug() << "HIBA: A szimulátor nem kerülhetne a Status::HeightAdjust állapotba.";
        break;
    default:
        Q_UNREACHABLE();
    }

    qDebug() << "Simulator: tick (" << state.timestamp()
             << "): állapot=" << state.getStatusName()
             << ", x=" << state.x()
             << ", v=" << state.v()
             << ", lámpa:" << state.light()
             << ", magasság" << state.height()
             << ", első tengely nyomás: " << state.pressures().first()
             << ", hátsó tengely nyomás: " << state.pressures().last();

    // Állapot küldése
    if (communication->isConnected())
    {
        communication->send(state);
    }
}

void Simulator::dataReady(QDataStream &inputStream)
{
    QList<double> newPressures;
    RobotState receivedState;
    receivedState.ReadFrom(inputStream);
    if(receivedState.status() == RobotState::Status::HeightAdjust){
        newPressures = {4* receivedState.height() * 0.083, 4 * receivedState.height() * 0.093};
    }
    switch(receivedState.status())
    {
    case RobotState::Status::Default:
        qDebug() << "Simulator: OK parancs. Igen, minden OK, köszönöm!";
        break;
    case RobotState::Status::Reset:
        qDebug() << "Simulator: Reset parancs.";
        state.setStatus(RobotState::Status::Reset);
        break;
    case RobotState::Status::Stopping:
        qDebug() << "Simulator: Stop parancs.";
        state.setStatus(RobotState::Status::Stopping);
        break;
    case RobotState::Status::HeightAdjust:
        qDebug() << "Simulator: Height adjusting";
        state.setHeight(receivedState.height());
        state.setPressures(newPressures);
        state.setOnlyHeightAdjust(true);
        if (communication->isConnected())
        {
            communication->send(state);
        }
        state.setOnlyHeightAdjust(false);
        state.setStatus(RobotState::Status::Default);
        break;
    case RobotState::Status::SelfTest:
        qDebug() << "Simulator: Selftest";
        state.setStatus(RobotState::Status::SelfTest);
        break;
    case RobotState::Status::Accelerate:
            qDebug() << "Simulator: Accelerate";
            if(!timer.isActive()){
                timer.start(currentTimerTickInterval);
            }
            state.setStatus(RobotState::Status::Accelerate);
            break;
    default:
        Q_UNREACHABLE();
    }
}

void Simulator::startSelfTest(){
    state.setHeight(1);
    selfTestTimer.start((long)(1000.0F));

}

void Simulator::selfTestTick(){
    if(selfTestProcessCounter < 23){ // 23
        qDebug() << "Simulator: Önteszt magasság emelés és gyorsítás";
        state.setStatus(RobotState::Status::SelfTest);
        communication->send(state);
        state.setHeight(state.height()+1);
        state.setV(state.v()+0.5);
        selfTestTimer.setInterval(1000.0F-selfTestProcessCounter*20);
    }
    else if (selfTestProcessCounter <= 47){ // 47
        qDebug() << "Simulator: Önteszt magasság csökkentés és lassítás";
        state.setStatus(RobotState::Status::SelfTest);
        communication->send(state);
        state.setHeight(state.height()-1);
        if(state.v() >= 0.5){
            state.setV(state.v()-0.5);
        }
        selfTestTimer.setInterval(520+selfTestProcessCounter*5);


    }
    else{
        state.setHeight(12);
        QList<double> resetPressures({4,4});
        state.setPressures(resetPressures);
        state.setStatus(RobotState::Status::Default);
        communication->send(state);
        qDebug() << "Simulator: Önteszt vége";
        selfTestTimer.stop();
        selfTestProcessCounter = 0;

    }
    QList<double> newPressures = {4* state.height() * 0.093, 4 * state.height() * 0.083};
    state.setPressures(newPressures);
    state.setLight( state.v() >= 10.0F ? 1.0F : 0.0F );
    state.setTimestamp(state.timestamp() + dt);
    state.setX(state.x() + state.v()*dt);
    state.setV(state.v());
    selfTestProcessCounter++;

}
