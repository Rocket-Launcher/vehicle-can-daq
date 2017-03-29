#ifndef E46CANBUSFRAME_H
#define E46CANBUSFRAME_H

#include <QCanBusFrame>

class E46CanBusFrame : public QCanBusFrame
{
    public:
        E46CanBusFrame();
        E46CanBusFrame(FrameType);
        E46CanBusFrame(quint32, const QByteArray&);
        unsigned short decodeEngineRpm() const;
        unsigned short decodeVehicleSpeed() const;
        unsigned short decodeFuelLevel() const;
        short decodeCoolantTempC() const;
        short decodeCoolantTempF() const;
        short decodeOilTempC() const;
        short decodeOilTempF() const;
};

#endif // E46CANBUSFRAME_H
