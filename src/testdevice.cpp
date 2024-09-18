#include "testdevice.h"
#include <QDebug>
#include <math.h>

TestDevice::TestDevice(QObject *parent)
    : BaseDevice{parent}
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TestDevice::tick);
    timer->start(10);
}

void TestDevice::dev_connect()
{
    BaseDevice::dev_connect();
    m_starttime = QDateTime::currentDateTime();
}

void TestDevice::tick()
{
    QDateTime now = QDateTime::currentDateTime();
    float t_ms = m_starttime.msecsTo(now);

    data_t d;
    d.time = t_ms;
    d.channel0 = sin( t_ms/1000 * 2*3.14*1 );
    d.channel1 = cos( t_ms/1000 * 2*3.14*.3 );
    d.overflow = false;
    data.enqueue(d);

    emit new_data();

}
