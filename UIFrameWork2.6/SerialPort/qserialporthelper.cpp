#include "qserialporthelper.h"

/**
 * @brief QSerialportHelper::QSerialportHelper
 */
QSerialportHelper::QSerialportHelper()
{
    QSerialPortState = false;
    QSerialPortSettings.BaudRate = BAUD9600;
    QSerialPortSettings.DataBits = DATA_8;
    QSerialPortSettings.Parity = PAR_NONE;
    QSerialPortSettings.StopBits = STOP_1;
    QSerialPortSettings.FlowControl = FLOW_OFF;
    QSerialPortSettings.Timeout_Millisec = 2;

    QSerialPortInit();
}

/**
 * @brief QSerialportHelper::~QSerialportHelper
 */
QSerialportHelper::~QSerialportHelper()
{
    QSerialPortClose();
    delete QSerialPort;
}

/**
 * @brief QSerialportHelper::QSerialPortInit
 */
void QSerialportHelper::QSerialPortInit()
{
    QSerialPortName = "/dev/ttyAMA0";
    QSerialPort = new Posix_QextSerialPort(QSerialPortName, QextSerialBase::Polling);
}

/**
 * @brief QSerialportHelper::QSerialPortOpen
 */
void QSerialportHelper::QSerialPortOpen()
{
    if(QSerialPortState == false)
    {
        if(QSerialPort->open(QIODevice::ReadWrite))
        {
            QSerialPortState = true;
            QSerialPort->setBaudRate(QSerialPortSettings.BaudRate);
            QSerialPort->setParity(QSerialPortSettings.Parity);
            QSerialPort->setStopBits(QSerialPortSettings.StopBits);
            QSerialPort->setDataBits(QSerialPortSettings.DataBits);
        }
    }
}

/**
 * @brief QSerialportHelper::QSerialPortClose
 */
void QSerialportHelper::QSerialPortClose()
{
    if(QSerialPortState == true)
    {
        QSerialPort->close();
        QSerialPortState = false;
    }
}

/**
 * @brief QSerialportHelper::QSerialPortWriteChar
 * @param c
 */
void QSerialportHelper::QSerialPortWriteChar(char c)
{
    if(QSerialPortState == true)
    {
        QSerialPort->putChar(c);
    }
}

/**
 * @brief QSerialportHelper::QSerialPortReadChar
 * @return
 */
void QSerialportHelper::QSerialPortReadChar(char *c)
{
    if(QSerialPortState == true)
    {
        QSerialPort->getChar(c);
    }
    else
    {
        *c = 0;
    }
}

void QSerialportHelper::YTK_F_Start_Stop()
{
    char cmd[3] = {YTK_F_START_STOP, YTK_F_START_STOP, YTK_F_START_STOP};
    for(int i=0;i<3;i++)
    {
        QSerialPort->putChar(YTK_F_START_STOP);
    }
}

void QSerialportHelper::YTK_F_Bend()
{
    char cmd[3] = {YTK_F_BEND, YTK_F_BEND, YTK_F_BEND};
    QSerialPort->write(cmd,3);
}

void QSerialportHelper::YTK_F_Stretch()
{
    char cmd[3] = {YTK_F_STRETCH, YTK_F_STRETCH, YTK_F_STRETCH};
    QSerialPort->write(cmd,3);
}


