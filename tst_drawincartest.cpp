#include <QString>
#include <QtTest>

#include "RobotStateHistory.h"

// tetszőleges értékek, ezek vannak beállítva és később ellenőrizve
#define TEST_STATE_VALUE RobotState::Status::Stopping
#define TEST_HEIGHT_VALUE 12
#define TEST_SPEED_VALUE 1

/**
 * @brief Az alkalmazást tesztelő osztály.
 */
class DrawinCarTest : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor
     *
     * A RobotStateHistory history példányosítása.
     */
    DrawinCarTest();

private Q_SLOTS:
    /**
     * @brief Tesztek inicializálása
     */
    void initTestCase();

    /**
     * @brief Állapotnév stringek tesztelése.
     *
     * @warning A globális state állapotát állítgatja
     */
    void testStatusNames();

    /**
     * @brief Állapot adatfolyamba írásának és olvasásának tesztelése.
     */
    void testStatusToFromStream();

    /**
     * @brief History méretének tesztelése.
     *
     * Kiindulási méret tesztelése.
     * History növekedésének tesztelése új RobotState hozzáadása hatására.
     */
    void testHistorySize();

    /**
     * @brief Legutóbbi állapot értékeinek tesztelése.
     *
     * @warning Ennek a testHistorySize() után kell futnia
     * mert az adja hozzá az itt tesztelt állapotot!
     */
    void testCurrentState();

private:
    /**
     * @brief A tesztekhez használt history.
     *
     * @see testHistorySize(), testCurrentState()
     */
    RobotStateHistory history;

    /**
     * @brief A tesztekhez használt állapot
     */
    RobotState state;

};

DrawinCarTest::DrawinCarTest() : history(), state()
{
}

void DrawinCarTest::initTestCase()
{
    // állapot beállítása
    state.setStatus(TEST_STATE_VALUE);
    state.setHeight(TEST_HEIGHT_VALUE);
    state.setV(TEST_SPEED_VALUE);
}

void DrawinCarTest::testStatusNames()
{
    state.setStatus(RobotState::Status::Accelerate);
    QCOMPARE(state.getStatusName(), QString("Gyorsítás"));

    state.setStatus(RobotState::Status::Default);
    QCOMPARE(state.getStatusName(), QString("Alap"));

    state.setStatus(RobotState::Status::Reset);
    QCOMPARE(state.getStatusName(), QString("Reset"));

    state.setStatus(RobotState::Status::Stopping);
    QCOMPARE(state.getStatusName(), QString("Megállás"));

    state.setStatus(RobotState::Status::SelfTest);
    QCOMPARE(state.getStatusName(), QString("Önteszt"));

    state.setStatus(RobotState::Status::HeightAdjust);
    QCOMPARE(state.getStatusName(), QString("Magasság áll."));

    // visszaállítjuk az eredeti értékre
    state.setStatus(TEST_STATE_VALUE);
}

void DrawinCarTest::testStatusToFromStream(){
    QByteArray dataHolder;

    QDataStream writeStream(&dataHolder,QIODevice::WriteOnly);
    writeStream << state;

    QDataStream readStream(&dataHolder,QIODevice::ReadOnly);
    RobotState state2;
    state2.ReadFrom(readStream);

    QVERIFY2(state2.status() == TEST_STATE_VALUE, "Fail: write/read stream status incorrect");
    QVERIFY2(state2.height() == TEST_HEIGHT_VALUE, "Fail: write/read stream height incorrect");
    QVERIFY2(state2.v() == TEST_SPEED_VALUE, "Fail: write/read stream speed incorrect");
}

void DrawinCarTest::testHistorySize()
{
    QVERIFY2(history.stateList.isEmpty(), "Fail: history initially not empty");

    history.Add(state);

    QVERIFY2(history.stateList.size() == 1, "Fail: history size not increased by 1");
}

void DrawinCarTest::testCurrentState()
{
    QVERIFY2(history.currentState->status() == TEST_STATE_VALUE, "Fail: history.currentState->status() incorrect");
    QVERIFY2(history.carCurrentHeight == TEST_HEIGHT_VALUE, "Fail: history.carCurrentHeight is not equal to what set before");
    QVERIFY2(history.currentState->v() == TEST_SPEED_VALUE, "Fail: history.currentState->v() incorrect");
}


// Itt most nem készítünk main() függvényt, azt ez a makró
//  legenerálja nekünk. Lefuttatja majd a tesztet és az eredményt
//  kiírja a konzolra.
QTEST_APPLESS_MAIN(DrawinCarTest)

#include "tst_drawincartest.moc"
