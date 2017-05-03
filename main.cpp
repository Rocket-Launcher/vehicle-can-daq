#include <QGuiApplication>
#include <QCursor>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QCanBus>
#include <QCanBusDevice>
#include <QGeoPositionInfoSource>   // <-- Geo Positioning
#include "e46canbusframe.h"
#include "canframeid.h"
#include "track.h"
#include <iostream>

using namespace std;

// Global.
QObject *object;
QCanBusDevice *device;

QJsonDocument readJson(const QString &filePath) {
    // Read JSON file to QString object.
    QJsonDocument jsonDoc;
    QString contents;
    QFile file(filePath);
    if(file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        contents = file.readAll();
        file.close();
    }
    else {
        jsonDoc = QJsonDocument();
        return jsonDoc;
    }

    // Create JSON document from QString.
    jsonDoc = QJsonDocument::fromJson(contents.toUtf8());

    return jsonDoc;
}

QJsonObject toJsonObject(const QJsonDocument &jsonDoc, const QString &trackName) {

    // Create JSON object from specified item (name of track).
    QJsonObject jsonObj;

    if(!jsonDoc.isEmpty())
        jsonObj = jsonDoc.object();
    else {
        jsonObj = QJsonObject();
        return jsonObj;
    }

    if(jsonObj.contains(trackName)) {
        jsonObj = jsonObj.value(trackName).toObject();
    }
    else {
        jsonObj = QJsonObject();
    }

    return jsonObj;
}

QCanBusDevice::Filter setCanFilter(const unsigned short &id)
{
    QCanBusDevice::Filter filter;

    filter.frameId = id;
    filter.frameIdMask = 0x7FFu; // Compare against all 11-bits of frame id.
    filter.format = QCanBusDevice::Filter::MatchBaseFormat;
    filter.type = QCanBusFrame::DataFrame;

    return filter;
}

void checkFrames()
{
    // Read frames.
    while(device->framesAvailable() > 0)
    {
        object->setProperty("canFilter", "Yes");
        object->setProperty("frames", device->framesAvailable());

        QCanBusFrame frame = device->readFrame();
        E46CanBusFrame canFrame(frame.frameId(), frame.payload());

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
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // Hide mouse curser.
    QGuiApplication::setOverrideCursor(QCursor(Qt::BlankCursor));

    // Load gauge UI.
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    object = component.create();

/***************************** Lap Timing and Geolocation functionality *****************************/

    // Read JSON document from file path.
    QJsonDocument jsonDoc = readJson(QCoreApplication::applicationDirPath() + QDir::separator() + "tracks.json");

    // Create C++ JSON Object with item specified from the document.
    QJsonObject jsonObj = toJsonObject(jsonDoc, "Road Atlanta");

    // Create Track object containing all relevant racetrack information.
    Track track(jsonObj);

    if(track.trackFound()) {
        cout << "Track Found" << endl;
        cout << track.toString() << endl;
    }
    else {
        cout << "Track NOT Found" << endl;
        cout << track.toString() << endl;
    }

    /*QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(0);
    if(source)
        source->minimumUpdateInterval();

    QGeoPositionInfo gpi = source->lastKnownPosition();
    QGeoCoordinate gc = gpi.coordinate();

    cout << gc.toString().toStdString() << endl;
    cout << source->availableSources();*/

/************************************** CAN Bus functionality ***************************************/

    if(QCanBus::instance()->plugins().contains("socketcan"))
    {
        // Create CAN bus device and connect to can0 via SocketCAN plugin.
        device = QCanBus::instance()->createDevice("socketcan", "can0");

        device->connectDevice();

        // Set filters for needed data frames from the CAN bus device.
        if(device->state() == QCanBusDevice::ConnectedState)
        {
            object->setProperty("connStatus", "Connected");

            // Apply filters to CAN Bus device.
            QList<QCanBusDevice::Filter> filterList;

            filterList.append(setCanFilter(E46_ENGINE_RPM));
            //filterList.append(setCanFilter(E46_VEHICLE_SPEED));
            filterList.append(setCanFilter(E46_FUEL_LEVEL));
            filterList.append(setCanFilter(E46_COOLANT_TEMP));
            filterList.append(setCanFilter(E46_OIL_TEMP));

            device->setConfigurationParameter(QCanBusDevice::RawFilterKey, QVariant::fromValue(filterList));

            // Connect framesRecieved signal to slot function for reading frames.
            QObject::connect(device, &QCanBusDevice::framesReceived, checkFrames);
        }
    }

    return app.exec();
}
