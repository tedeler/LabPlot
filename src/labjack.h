#ifndef LABJACK_H
#define LABJACK_H

#include "basedevice.h"

#include <windows.h>
#include <LabJackUD.h>


class Labjack : public BaseDevice
{
    Q_OBJECT
public:
    Labjack(QObject *parent = 0);
    void dev_connect();
    void dev_disconnect();

private:
    QTimer m_timer;
    QTimer m_led_timer;
    bool m_led_state;
    QDateTime m_starttime;
    LJ_HANDLE labjack_handle;

    bool ErrorHandler(LJ_ERROR lngErrorcode, long lngLineNumber, long lngIteration);

private slots:
    void generate_data();
    void toggle_measurement_led();

};

#endif // LABJACK_H
