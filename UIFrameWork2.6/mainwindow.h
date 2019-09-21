#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QStyledItemDelegate>
#include <QSettings>
#include "Emotiv/emotivprocessthread.h"

//#include <E:/QTProject/UIFrameWork/titlewidget.h>
//#include <E:/QTProject/UIFrameWork/QCustomPlot/qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void SkinChangedSlot(uint skinKey);
    void LanguageChangedSlot(int value);

    /*鼠标操作*/
    void mousePress();
    void mouseWheel();
    void selectionChanged();

    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void on_IP_PortSureButton_clicked();
    void on_SerialportBtton_clicked();
    void on_IPPortRadioButton_clicked(bool checked);
    void on_SerialPortRadioButton_clicked(bool checked);

    void SetBattery(int value);

private:
    void LineChartInit();
    void IPPortSettingInit();
    void SerialPortSettingInit();
    void SetConnectQuality();

private:
    Ui::MainWindow *ui;
    QStatusBar *stateBar;
    uint skinColor;

    QStyledItemDelegate* ComBoxItemDelegate;
    QSettings *Settings;
    enum iconKey
    {
        HOME_ICON           = 0xf015,
        CONNECTQUALITY_ICON = 0xf127,
        SETTING_ICON        = 0xf013,
        STATE_ICON          = 0xf110,
        BATTERY_ICON        = 0xf10c,
        CONTROL_ICON        = 0xf0ad,
        START_ICON          = 0xf04b,
        STOP_ICON           = 0xf04c,
        LINECHART_ICON      = 0xf201,
        IPPORT_ICON         = 0xf09e,
        SERIALPORT_ICON     = 0xf126,
    };
    EmotivProcessThread *emotivProcThread;
};

#endif // MAINWINDOW_H
