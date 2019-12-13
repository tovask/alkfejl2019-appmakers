#include <QString>
#include <QtTest>

#include "RobotStateHistory.h"

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
     * @brief Állapot név stringek tesztelése.
     */
    void testStatusNames();

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

};

DrawinCarTest::DrawinCarTest() : history()
{
}

void DrawinCarTest::testStatusNames()
{
    RobotState state;

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
}

#define TEST_HEIGHT_VALUE 12
#define TEST_SPEED_VALUE 1
void DrawinCarTest::testHistorySize()
{
    RobotState state;
    state.setHeight(TEST_HEIGHT_VALUE);
    state.setV(TEST_SPEED_VALUE);
    state.setStatus(RobotState::Status::Stopping);

    QVERIFY2(history.stateList.isEmpty(), "Fail: history initially not empty");

    history.Add(state);

    QVERIFY2(history.stateList.size() == 1, "Fail: history size not increased by 1");
}

void DrawinCarTest::testCurrentState()
{
    QVERIFY2(history.carCurrentHeight == TEST_HEIGHT_VALUE, "Fail: history.carCurrentHeight is not equal to what set before");
    QVERIFY2(history.currentState->v() == TEST_SPEED_VALUE, "Fail: history.currentState->v() incorrect");
    QVERIFY2(history.currentState->status() == RobotState::Status::Stopping, "Fail: history.currentState->status() incorrect");

}


// Itt most nem készítünk main() függvényt, azt ez a makró
//  legenerálja nekünk. Lefuttatja majd a tesztet és az eredményt
//  kiírja a konzolra.
QTEST_APPLESS_MAIN(DrawinCarTest)

#include "tst_drawincartesttest.moc"
