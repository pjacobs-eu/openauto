/*
*  This file is part of openauto project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  openauto is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  openauto is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with openauto. If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QtBluetooth>
#include <f1x/openauto/Common/Log.hpp>
#include <f1x/openauto/btservice/AndroidBluetoothService.hpp>
#include <f1x/openauto/btservice/AndroidBluetoothServer.hpp>

namespace btservice = f1x::openauto::btservice;

int main(int argc, char *argv[]) {
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth*=true"));
    QApplication qApplication(argc, argv);

    QBluetoothLocalDevice localDevice;
    const QBluetoothAddress address = localDevice.address();
    const uint16_t portNumber = 5000;

    // Turn Bluetooth on
    localDevice.powerOn();
    // Make it visible to others
    //localDevice.setHostMode(QBluetoothLocalDevice::HostConnectable);

    btservice::AndroidBluetoothServer androidBluetoothServer;
    if (!androidBluetoothServer.start(address, portNumber)) {
        OPENAUTO_LOG(error) << "[btservice] Server start failed.";
        return 2;
    }

    OPENAUTO_LOG(info) << "[btservice] Listening for connections, address: " << address.toString().toStdString()
                       << ", port: " << portNumber;

    btservice::AndroidBluetoothService androidBluetoothService(portNumber);
    if (!androidBluetoothService.registerService(address)) {
        OPENAUTO_LOG(error) << "[btservice] Service registration failed.";
        return 1;
    } else {
        OPENAUTO_LOG(info) << "[btservice] Service registered, port: " << portNumber;
    }

    QApplication::exec();

    OPENAUTO_LOG(info) << "stop";
    androidBluetoothService.unregisterService();

    return 0;
}
