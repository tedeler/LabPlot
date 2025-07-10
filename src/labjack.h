#ifndef LABJACK_H
#define LABJACK_H

#include "basedevice.h"

extern "C" {
#include "labjackusb.h"
#include "u6.h"
}


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
    HANDLE labjack_handle;
    u6CalibrationInfo caliInfo;

    int packetCounter;

    int ConfigIO();
    int StreamConfig();
    int StreamStart();
    int StreamData(u6CalibrationInfo *caliInfo);
    int StreamStop();

//    bool ErrorHandler(LJ_ERROR lngErrorcode, long lngLineNumber, long lngIteration);

private slots:
    void generate_data();
    void toggle_measurement_led();

};

#endif // LABJACK_H
