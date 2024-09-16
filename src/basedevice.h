#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QObject>
#include <QDateTime>
#include <QQueue>
#include <QTimer>

class data_t
{
public:
    float time;
    float channel0,channel1;
    bool overflow;
};

class BaseDevice : public QObject
{
    Q_OBJECT
public:
    enum EDeviceState {EDS_DISCONNECTED, EDS_CONNECTED, EDS_ERROR, EDS_UNKNOWN};

    explicit BaseDevice(QObject *parent = 0);
    void dev_connect();
    void dev_disconnect();
    bool is_connected(){return state==EDS_CONNECTED;}
    data_t getData();
    bool dataAvailable();
    EDeviceState get_device_state(){return state;}
    QString get_device_state_info_str(){return state_info_str;}

protected:
    void set_device_state(EDeviceState, QString info_str="");
    QQueue<data_t> data;

private:
    EDeviceState state;
    QString state_info_str;

private slots:
    void generate_data(){};

signals:
    void new_data();
    void state_changed();

public slots:
};

#endif // BASEDEVICE_H
