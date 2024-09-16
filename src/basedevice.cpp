#include "basedevice.h"
#include <math.h>
#include <QDebug>

BaseDevice::BaseDevice(QObject *parent) :
    QObject(parent), state(EDS_UNKNOWN)
{
    set_device_state(EDS_DISCONNECTED);
}

void BaseDevice::dev_connect()
{
    set_device_state(EDS_CONNECTED);
}

void BaseDevice::dev_disconnect()
{
    set_device_state(EDS_DISCONNECTED);
}

bool BaseDevice::dataAvailable()
{
    return data.size() > 0;
}

data_t BaseDevice::getData()
{
    return data.dequeue();
}

void BaseDevice::set_device_state(EDeviceState newstate, QString info_str)
{
    state_info_str = info_str;
    if (newstate != this->state)
    {
        this->state = newstate;
        emit state_changed();
    }
}
