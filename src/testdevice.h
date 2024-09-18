#ifndef TESTDEVICE_H
#define TESTDEVICE_H

#include "basedevice.h"
#include <QTimer>

class TestDevice : public BaseDevice
{
public:
    explicit TestDevice(QObject *parent = nullptr);
    void dev_connect();

private:
    void tick();
    QTimer *timer;
    QDateTime m_starttime;
};

#endif // TESTDEVICE_H
