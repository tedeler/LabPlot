#include "labjack.h"

#include <stdio.h>
#include <QDebug>
#include <QMessageBox>
#include <stdio.h>

#include "LJUD_DynamicLinking.h"

bool LoadLabJackUD(void)
{
    //Now try and load the DLL.
    if( (hDLLInstance = LoadLibraryA("labjackud.dll")) )
    {
        //If successfully loaded, get the address of the functions.
        m_pListAll = (tListAll)::GetProcAddress(hDLLInstance,"ListAll");
        m_pOpenLabJack = (tOpenLabJack)::GetProcAddress(hDLLInstance,"OpenLabJack");
        m_pAddRequest = (tAddRequest)::GetProcAddress(hDLLInstance,"AddRequest");
        m_pGo = (tGo)::GetProcAddress(hDLLInstance,"Go");
        m_pGoOne = (tGoOne)::GetProcAddress(hDLLInstance,"GoOne");
        m_peGet = (teGet)::GetProcAddress(hDLLInstance,"eGet");
        m_pePut = (tePut)::GetProcAddress(hDLLInstance,"ePut");
        m_pGetResult = (tGetResult)::GetProcAddress(hDLLInstance,"GetResult");
        m_pGetFirstResult = (tGetFirstResult)::GetProcAddress(hDLLInstance,"GetFirstResult");
        m_pGetNextResult = (tGetNextResult)::GetProcAddress(hDLLInstance,"GetNextResult");
        m_peAIN = (teAIN)::GetProcAddress(hDLLInstance,"eAIN");
        m_peDAC = (teDAC)::GetProcAddress(hDLLInstance,"eDAC");
        m_peDI = (teDI)::GetProcAddress(hDLLInstance,"eDI");
        m_peDO = (teDO)::GetProcAddress(hDLLInstance,"eDO");
        m_peAddGoGet = (teAddGoGet)::GetProcAddress(hDLLInstance,"eAddGoGet");
        m_peTCConfig = (teTCConfig)::GetProcAddress(hDLLInstance,"eTCConfig");
        m_peTCValues = (teTCValues)::GetProcAddress(hDLLInstance,"eTCValues");
        m_pResetLabJack = (tResetLabJack)::GetProcAddress(hDLLInstance,"ResetLabJack");
        m_pDoubleToStringAddress = (tDoubleToStringAddress)::GetProcAddress(hDLLInstance,"DoubleToStringAddress");
        m_pStringToDoubleAddress = (tStringToDoubleAddress)::GetProcAddress(hDLLInstance,"StringToDoubleAddress");
        m_pStringToConstant = (tStringToConstant)::GetProcAddress(hDLLInstance,"StringToConstant");
        m_pErrorToString = (tErrorToString)::GetProcAddress(hDLLInstance,"ErrorToString");
        m_pGetDriverVersion = (tGetDriverVersion)::GetProcAddress(hDLLInstance,"GetDriverVersion");
        m_pTCVoltsToTemp = (tTCVoltsToTemp)::GetProcAddress(hDLLInstance,"TCVoltsToTemp");
        return true;
    }
    else
    {
        printf("\nFailed to load DLL\n");
        return false;
    }
    // m_pOpenLabJack now holds a pointer to the OpenLabJack function.  The compiler
    // automatically recognizes m_pOpenLabJack as a pointer to a function and
    // calls the function with the parameters given.  If we created another
    // variable of type tOpenLabJack and simply put "pNewVar = m_pOpenLabJack",
    // then the compiler might not know to call the function.
}

bool Labjack::ErrorHandler(LJ_ERROR lngErrorcode, long lngLineNumber, long lngIteration)
{
    char err[255];

    if(lngErrorcode != LJE_NOERROR)
    {
        m_pErrorToString(lngErrorcode,err);
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

    lngErrorcode = m_pOpenLabJack(LJ_dtU6, LJ_ctUSB, "1", 1, &labjack_handle);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    lngErrorcode = m_pePut(labjack_handle, LJ_ioPUT_CONFIG, LJ_chAIN_RESOLUTION, 0, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    //Configure the analog input range on channels 2 and 3 for bipolar gain=1.
    lngErrorcode = m_pePut(labjack_handle,  LJ_ioPUT_AIN_RANGE, 2, LJ_rgBIP10V, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    lngErrorcode = m_pePut(labjack_handle,  LJ_ioPUT_AIN_RANGE, 3, LJ_rgBIP10V, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;

    //Now we add requests to write and read I/O.  These requests
    //will be processed repeatedly by go/get statements in every
    //iteration of the while loop below.

    //Request AIN2 and AIN3.
    for(int i=0; i<4; i++)
    {
        lngErrorcode = m_pAddRequest(labjack_handle, LJ_ioGET_AIN, i, 0, 0, 0);
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
    lngErrorcode = m_peDO(labjack_handle, 0, 0);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;
}



Labjack::Labjack(QObject *parent)
    :BaseDevice(parent), m_led_state(false)
{
    if (!LoadLabJackUD())
    {
        QMessageBox msgBox;
        QString s;
        s = s.asprintf("Datei labjackud.dll wurde nicht gefunden. Programm wird beendet!");
        msgBox.setText(s);
        msgBox.exec();
        exit(1);
    }
    else
    {
        QTimer::connect(&m_timer, SIGNAL(timeout()), this, SLOT(generate_data()));
        QTimer::connect(&m_led_timer, SIGNAL(timeout()), this, SLOT(toggle_measurement_led()));
    }

}

void Labjack::toggle_measurement_led()
{
    qDebug() << m_led_state;

    LJ_ERROR lngErrorcode;
    lngErrorcode = m_peDO(labjack_handle, 0, (int) m_led_state);
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
    static int count=0;

    LJ_ERROR lngErrorcode = 0;
    long lngIOType = 0, lngChannel = 0;
    double dblValue = 0;
    double Values[4];
    long lngGetNextIteration = 0;

    lngErrorcode = m_pGoOne(labjack_handle);
    if (ErrorHandler(lngErrorcode, __LINE__, 0))
        return;
    lngErrorcode = m_pGetFirstResult(labjack_handle, &lngIOType, &lngChannel, &dblValue, 0, 0);
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

        lngErrorcode = m_pGetNextResult(labjack_handle, &lngIOType, &lngChannel, &dblValue, 0, 0);
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
    count++;

    emit new_data();
}

