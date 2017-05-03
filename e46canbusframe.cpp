#include "e46canbusframe.h"
#include "canframeid.h"

E46CanBusFrame::E46CanBusFrame()
{

}

E46CanBusFrame::E46CanBusFrame(FrameType type = DataFrame)
:
    QCanBusFrame(type)
{}

E46CanBusFrame::E46CanBusFrame(quint32 identifier, const QByteArray &data)
:
    QCanBusFrame(identifier, data)
{}

unsigned short E46CanBusFrame::decodeEngineRpm() const
{
    /*
    Unit: RPM
    CAN Id: 0x316 (790)
    Conversion: ((B4 * 256) + B3) / 6.4
    */

    if(frameId() != E46_ENGINE_RPM)
        return 0;

    unsigned short b3, b4;
    const QByteArray &payload = this->payload();

    b3 = payload[2];
    b4 = payload[3];

    return ((b4 * 256) + b3) / 6.4;
}

unsigned short E46CanBusFrame::decodeVehicleSpeed() const
{
    /*
    Unit: ?
    CAN Id: ?
    Conversion: ?
    */

    if(frameId() != E46_VEHICLE_SPEED)
        return 0;

    const QByteArray &payload = this->payload();

    return 0;
}

unsigned short E46CanBusFrame::decodeFuelLevel() const
{
    /*
    Unit: Liters -> Percent (%)
    CAN Id: 0x613 (1555)
    Conversion: B3
    Note: B3 is fuel level. Full being hex 39. Fuel light comes on at hex 8.
    */

    if(frameId() != E46_FUEL_LEVEL)
        return 0;

    const float fuelCapacity = 50.0; //62.83784;
    unsigned short b3;
    const QByteArray &payload = this->payload();

    b3 = payload[2];

    return b3; //(b3 / fuelCapacity) * 100;
}

short E46CanBusFrame::decodeCoolantTempC() const
{
    /*
    Unit: C
    CAN Id: 0x329 (809)
    Conversion: (B2 * 0.75) - 48.373
    */

    if(frameId() != E46_COOLANT_TEMP)
        return 0;

    unsigned short b2;
    const QByteArray &payload = this->payload();

    b2 = payload[1];

    return (b2 * 0.75) - 48.373;
}

short E46CanBusFrame::decodeCoolantTempF() const
{
    /*
    Unit: C
    CAN Id: 0x329 (809)
    Conversion: (decodeCoolantTempC * 1.8) + 32
    */

    return decodeCoolantTempC() * 1.8 + 32;
}

short E46CanBusFrame::decodeOilTempC() const
{
    /*
    Unit: C
    CAN Id: 0x545 (1349)
    Conversion: B5 - 48.373
    */

    if(frameId() != E46_OIL_TEMP)
        return 0;

    unsigned short b5;
    const QByteArray &payload = this->payload();

    b5 = payload[4];

    return b5 - 48.373;
}

short E46CanBusFrame::decodeOilTempF() const
{
    /*
    Unit: C
    CAN Id: 0x545 (1349)
    Conversion: (decodeOilTempC * 1.8) + 32
    */

    return decodeOilTempC() * 1.8 + 32;
}
