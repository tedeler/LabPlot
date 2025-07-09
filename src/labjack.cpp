#include "labjack.h"

#include <QDebug>
#include <QMessageBox>

bool Labjack::ErrorHandler(LJ_ERROR lngErrorcode, long lngLineNumber, long lngIteration)
{
    char err[255];

    if(lngErrorcode != LJE_NOERROR)
    {
        ErrorToString(lngErrorcode,err);
        set_device_state(BaseDevice::EDS_ERROR, err);

        qDebug() << "Error number = " << lngErrorcode;
        qDebug() << "Error string = " << err;
        qDebug() << "Source line number" << lngLineNumber;
        qDebug() << "Iteration = " << lngIteration;
        if(lngErrorcode > LJE_MIN_GROUP_ERROR)
        {
            return true;
        }

    }
    return false;

}


void Labjack::dev_connect()
{
    qDebug() << "Connect Device";
    if(is_connected())
    {
        set_device_state(EDS_ERROR);
        return;
    }

    LJ_ERROR lngErrorcode;

    lngErrorcode = OpenLabJack(LJ_dtU6, LJ_ctUSB, "1", 1, &labjack_handle);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    lngErrorcode = ePut(labjack_handle, LJ_ioPUT_CONFIG, LJ_chAIN_RESOLUTION, 0, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    //Configure the analog input range on channels 2 and 3 for bipolar gain=1.
    lngErrorcode = ePut(labjack_handle,  LJ_ioPUT_AIN_RANGE, 2, LJ_rgBIP10V, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    lngErrorcode = ePut(labjack_handle,  LJ_ioPUT_AIN_RANGE, 3, LJ_rgBIP10V, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    //Now we add requests to write and read I/O.  These requests
    //will be processed repeatedly by go/get statements in every
    //iteration of the while loop below.

    //Request AIN2 and AIN3.
    for(int i=0; i<4; i++)
    {
        lngErrorcode = AddRequest(labjack_handle, LJ_ioGET_AIN, i, 0, 0, 0);
        if (ErrorHandler(lngErrorcode, __LINE__, 0))
            return;
    }

    m_timer.setInterval(20);
    m_timer.start();
    m_starttime = QDateTime::currentDateTime();

    BaseDevice::dev_connect();
    qDebug() << labjack_handle;

    m_led_timer.setInterval(250);
    m_led_timer.start();
}

void Labjack::dev_disconnect()
{
    qDebug() << "Disconnect Device";
    m_timer.stop();
    qDebug() << "m_timer stopped";
    m_led_timer.stop();

    qDebug() << "Switch m-led off";
    m_led_state = false;
    toggle_measurement_led();

    BaseDevice::dev_disconnect();


    LJ_ERROR lngErrorcode;
    lngErrorcode = eDO(labjack_handle, 0, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;
}



Labjack::Labjack(QObject *parent)
    :BaseDevice(parent), m_led_state(false)
{
    QTimer::connect(&m_timer, SIGNAL(timeout()), this, SLOT(generate_data()));
    QTimer::connect(&m_led_timer, SIGNAL(timeout()), this, SLOT(toggle_measurement_led()));

}

void Labjack::toggle_measurement_led()
{
    qDebug() << m_led_state;

    LJ_ERROR lngErrorcode;
    lngErrorcode = eDO(labjack_handle, 0, (int) m_led_state);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    m_led_state = !m_led_state;

}

void Labjack::generate_data()
{
    if(!is_connected())
    {
        return;
    }

    QDateTime now = QDateTime::currentDateTime();
    float t_ms = m_starttime.msecsTo(now);

    LJ_ERROR lngErrorcode = 0;
    long lngIOType = 0, lngChannel = 0;
    double dblValue = 0;
    double Values[4];
    long lngGetNextIteration = 0;

    lngErrorcode = GoOne(labjack_handle);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;
    lngErrorcode = GetFirstResult(labjack_handle, &lngIOType, &lngChannel, &dblValue, 0, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    bool ValueOverflow = false;
    while(lngErrorcode < LJE_MIN_GROUP_ERROR)
    {
        switch(lngIOType)
        {
            case LJ_ioGET_AIN :
                Values[lngChannel] = dblValue;
                if(dblValue > 10.0)
                        ValueOverflow = true;
        }

        lngErrorcode = GetNextResult(labjack_handle, &lngIOType, &lngChannel, &dblValue, 0, 0);
        if(lngErrorcode != LJE_NO_MORE_DATA_AVAILABLE)
        {
            ErrorHandler(lngErrorcode, __LINE__, lngGetNextIteration);
        }

    }

    float channel1 = Values[2]-Values[3];
    float channel0 = Values[1]-Values[0];

    data_t d;
    d.time = t_ms/1000;
    d.channel0 = channel0;
    d.channel1 = channel1;
    d.overflow = ValueOverflow;
    data.enqueue(d);
//    qDebug() << "Time: " << d.time << " channel1: " << d.channel1 << " channel2: " << d.channel2;
//    qDebug() << data.size() << "Datapoints available";

    emit new_data();
}

