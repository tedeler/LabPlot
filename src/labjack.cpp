#include "labjack.h"
extern "C" {
#include "u6.h"
}

#include <QDebug>
#include <QMessageBox>


void Labjack::dev_connect()
{
    m_timer.setInterval(20);
    m_timer.start();
    m_starttime = QDateTime::currentDateTime();

    if( (labjack_handle = openUSBConnection(-1)) == NULL ) {
        set_device_state(EDS_DISCONNECTED);
    }

    openUSBConnection(-1);

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

}



Labjack::Labjack(QObject *parent)
    :BaseDevice(parent), m_led_state(false)
{
    QTimer::connect(&m_timer, SIGNAL(timeout()), this, SLOT(generate_data()));
    QTimer::connect(&m_led_timer, SIGNAL(timeout()), this, SLOT(toggle_measurement_led()));

}

void Labjack::toggle_measurement_led()
{
}

void Labjack::generate_data()
{
}

