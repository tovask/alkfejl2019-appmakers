#pragma once
#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H
#include <QDataStream>
#include <QString>

/**
 * @brief A robot teljes állapotleírása le egy adott időpillanatban.
 *
 * A kommunikáció során ilyen objektumokat küldünk át a robot és a kliens között.
 * A robottól érkezve az aktuális állapotot képviseli, míg az ellenkező irányba küldve
 *  parancsként a kívánt állapotot írja le.
 *
 * A history ilyen objektumok sorozata.
 */
class RobotState : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief A robot állapota
     */
    enum class Status
    {
        /** Alapállapot. A robot tartja az aktuális gyorsulást. */
        Default = 0,
        /** Reset. Parancsként használjuk. */
        Reset = 1,
        /** Stop parancs. Ez az állapot akkor is érvényes marad, ha megállt a robot. */
        Stopping = 2,
        /** Gyorsítási parancs a robotnak. A gyorsítás mértékét a robot
         * átveszi a RobotState::a tulajdonságból. */
        Accelerate = 3,
        /** Önteszt. A robot működésének tesztelése.
         * A parancsok letiltásra kerülnek alatta. */
        SelfTest = 4,
        /** Magasság állítása parancs a robotnak. */
        HeightAdjust = 5
    };

    /**
     * @brief Konstruktor
     */
    RobotState();

    /**
     * @brief Konstruktor adott értékekkel.
     * @param status    Robot állapot
     * @param timestamp Időbélyeg
     * @param x Pozíció
     * @param v Sebesség
     * @param light Robot lámpájának állapota
     * @param height Magasság
     * @param pressures Légrugók nyomása
     * @param onlyHeightAdjust Csak a magasság változott
     */
    RobotState(Status status, qint64 timestamp,
        float x, float v, qint8 light, qint32 height, QList<double> pressures, bool onlyHeightAdjust);

    ~RobotState() = default;

    // A NOTIFY signalokat nem implementáljuk, mert nincs rájuk szükség.

    /** Állapot (vagy parancs) */
    Q_PROPERTY(Status status READ status WRITE setStatus MEMBER _status NOTIFY statusChanged)
    Status status() const { return _status; }
    void setStatus(const Status status) { _status = status; }

    /** Időbélyeg (ms) */
    Q_PROPERTY(float timestamp READ timestamp WRITE setTimestamp MEMBER _timestamp NOTIFY timestampChanged)
    qint64 timestamp() const { return _timestamp; }
    void setTimestamp(const qint64 timestamp) { _timestamp = timestamp; }

    /** Pozíció (méter) */
    Q_PROPERTY(float x READ x WRITE setX MEMBER _x NOTIFY xChanged)
    float x() const { return _x; }
    void setX(float x) { _x = x; }

    /** Sebesség (m/s) */
    Q_PROPERTY(float v READ v WRITE setV MEMBER _v NOTIFY vChanged)
    float v() const { return _v; }
    void setV(float v) { _v = v; }

    /** A robot lámpájának állapota. */
    Q_PROPERTY(bool light READ light WRITE setLight MEMBER _light NOTIFY lightChanged)
    float light() const { return _light; }
    void setLight(float light) { _light = light; }

    /** Magasság (m) */
    Q_PROPERTY(qint32 height READ height WRITE setHeight MEMBER _height NOTIFY heightChanged)
    qint32 height() const { return _height; }
    void setHeight(qint32 height) { _height = height; }

    /** Csak magasság állítás bool */
    Q_PROPERTY(bool onlyHeightAdjust READ onlyHeightAdjust WRITE setOnlyHeightAdjust MEMBER _onlyHeightAdjust NOTIFY onlyHeightChaned)
    bool onlyHeightAdjust() const { return _onlyHeightAdjust; }
    void setOnlyHeightAdjust(bool onlyHeightAdjust) { _onlyHeightAdjust = onlyHeightAdjust; }

    /** Légrugók nyomása (bar) */
    Q_PROPERTY(QList<double> pressures READ pressures WRITE setPressures MEMBER _pressures NOTIFY pressuresChanged)
    QList<double> pressures() const { return _pressures; }
    void setPressures(QList<double> pressures) { _pressures = pressures; }

    /** Az aktuális állapot QStringként. */
    // In QML, it will be accessible as model.statusName
    Q_PROPERTY(QString statusName READ getStatusName NOTIFY statusChanged)

    /**
     * @brief Sorosítja az objektumot a megadott streambe.
     * @param stream Az írandó adatfolyam.
     */
    void WriteTo(QDataStream& stream) const;

    /**
     * @brief Beolvassa az objektumot a streamből.
     * @param stream Az olvasandó adatfolyam
     */
    void ReadFrom(QDataStream& stream);

    /**
     * @brief Másolat készítés.
     *
     * Erre azért van szükség, mert a QObject-ek másolására speciális szabályok vonatkoznak.
     * @param other A másolandó RobotState
     */
    void CopyFrom(const RobotState& other);



    /**
     * @brief Olvaható formában visszaadja az állapotot.
     * @return Az állapot neve
     */
    QString getStatusName() const;

signals:
    void statusChanged(); /** Állapot változása. (Nincs használva) */
    void timestampChanged(); /** Időbélyeg változása. (Nincs használva) */
    void xChanged(); /** Pozíció változása. (Nincs használva) */
    void vChanged(); /** Sebesség változása. (Nincs használva) */
    void lightChanged(); /** Világítás változása. (Nincs használva) */
    void heightChanged(); /** Magasság változása. */
    void pressuresChanged(); /** Nyomás változása. (Nincs használva) */
    void onlyHeightChaned(); /** Csak a magasság változása. (Nincs használva) */

private:
    Status _status; /** Állapot (vagy parancs) */
    qint64 _timestamp; /** Időbélyeg (s) */
    float _x;       /** Pozíció (m) */
    float _v;       /** Sebesség (m/s) */
    qint8 _light;   /** Lámpa (on/off) */
    qint32 _height; /** Magasság (m) */
    QList<double> _pressures = QList<double>() << 4 << 4; /** Légrugók nyomása, első/hátsó tengely (bar) */
    bool _onlyHeightAdjust = false; /** Ha csak magasságot állítunk akkor true */

    /** Az állapotok és szöveges verziójuk közti megfeleltetés.
     * A getStatusName() használja. */
    static std::map<int,QString> statusNames;

    /** Beállítja a statusNames értékeit. A konstruktor hívja. */
    void initStatusNames();
};

/** Beburkolja a RobotState.WriteTo metódust. */
QDataStream &operator<<(QDataStream &, const RobotState &);

/** Beburkolja a RobotState.ReadFrom metódust. */
QDataStream &operator>>(QDataStream &, RobotState &);

#endif // ROBOTSTATE_H
