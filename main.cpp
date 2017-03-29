#include <QGuiApplication>
#include <QCursor>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QCanBus>
#include <QCanBusDevice>
#include "e46canbusframe.h"
#include "canframeid.h"

QCanBusDevice::Filter setCanFilter(const unsigned short &id)
{
    QCanBusDevice::Filter filter;

    filter.frameId = id;
    filter.frameIdMask = 0x7FFu; // Compare against all 11-bits of frame id.
    filter.format = QCanBusDevice::Filter::MatchBaseFormat;
    filter.type = QCanBusFrame::DataFrame;

    return filter;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // Hide mouse curser.
    QGuiApplication::setOverrideCursor(QCursor(Qt::BlankCursor));

    // Load gauge UI.
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/mainDemo.qml")));
    QObject *object = component.create();
    //delete object;

    // Create CAN bus device and connect to can0 via SocketCAN plugin.
    QCanBusDevice *device;

    if(QCanBus::instance()->plugins().contains("socketcan"))
    {
        device = QCanBus::instance()->createDevice("socketcan", "can0");
        device->connectDevice();
    }

    // Set filters for needed data frames from the CAN bus device.
    if(device->state() == QCanBusDevice::ConnectedState)
    {
        // Apply filters to CAN Bus device.
        QList<QCanBusDevice::Filter> filterList;

        filterList.append(setCanFilter(E46_ENGINE_RPM));
        //filterList.append(setCanFilter(E46_VEHICLE_SPEED));
        filterList.append(setCanFilter(E46_FUEL_LEVEL));
        filterList.append(setCanFilter(E46_COOLANT_TEMP));
        filterList.append(setCanFilter(E46_OIL_TEMP));

        device->setConfigurationParameter(QCanBusDevice::RawFilterKey, QVariant::fromValue(filterList));

        // Read frames and push decoded data to appropriate gauge for display.
        while(device->framesAvailable() > 0 && device->state() == QCanBusDevice::ConnectedState)
        {
            E46CanBusFrame canFrame(device->readFrame().frameId(), device->readFrame().payload());

            if(canFrame.isValid())
            {
                switch(canFrame.frameId())
                {
                case E46_ENGINE_RPM:
                    object->setProperty("rpmValue", canFrame.decodeEngineRpm());
                    break;
                /*case VEHICLE_SPEED:
                    object->setProperty("speedValue", canFrame.decodeVehicleSpeed());
                    break;*/
                case E46_FUEL_LEVEL:
                    object->setProperty("fuelValue", canFrame.decodeFuelLevel());
                    break;
                case E46_COOLANT_TEMP:
                    object->setProperty("coolantValue", canFrame.decodeCoolantTempC());
                    break;
                case E46_OIL_TEMP:
                    object->setProperty("oilValue", canFrame.decodeOilTempC());
                    break;
                default:
                    break;
                }
            }
        }

        delete device;
    }

    return app.exec();
}
