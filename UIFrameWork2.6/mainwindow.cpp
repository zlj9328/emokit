#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msgbox.h"
#include "IcoHelper/iconhelper.h"


#define SKIN_NUMBER "/Skin_Number"
#define LANGUAGE_NUMBER "/Language_Number"
#define IP_PORT_STATE "/IP_Port_state"
#define SERIAL_PORT_STATE "/Serial_Port_state"
bool IP_Port_state = false;
bool Serial_Port_state = false;

/*AF3,AF4,F7,F3,F4,F8,Fc5,Fc6,T7,T8,CMS,DRK,
P7,P8,O1,O2*/
int Cannel[16] = {2,0,2,2,1,2,1,2,2,3,0,2,2,3,3,2};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

    /*Setting Init*/
    Settings = new QSettings("Settings.ini");
    uint skin = Settings->value(SKIN_NUMBER).toUInt();
    int langu = Settings->value(LANGUAGE_NUMBER).toInt();
    /*title init*/
    ui->titleWidget->titleInit(tr("Home"), true,false, skin, langu);
    connect(ui->titleWidget,SIGNAL(SkinChangedSignal(uint)),this,SLOT(SkinChangedSlot(uint)));
    connect(ui->titleWidget,SIGNAL(LanguageChangedSignal(int)),this,SLOT(LanguageChangedSlot(int)));

    /*IP & Serial Port Init*/
    IPPortSettingInit();
    SerialPortSettingInit();

    /*Statebar init*/
    stateBar = new QStatusBar(ui->mainWidget);
    ui->statuVerticalLayout->addWidget(stateBar);

    SkinChangedSlot(skin);
//    LanguageChangedSlot(langu);

    /*tabWidget init*/
    IconHelper::Instance()->SetIcon(ui->tabWidget,0, QChar(HOME_ICON), 30);
    IconHelper::Instance()->SetIcon(ui->tabWidget,1, QChar(CONNECTQUALITY_ICON), 30);
    IconHelper::Instance()->SetIcon(ui->tabWidget,2, QChar(SETTING_ICON), 30);
    ui->tabWidget->setCurrentIndex(0);

    /*LineChart Init*/
    LineChartInit();

    /*Battery init*/
    ui->BatteryProgressBar->setRange(0,100);
    ui->BatteryProgressBar->setTextVisible(false);
    SetBattery(10);

    SetConnectQuality();

    stateBar->showMessage("Start...",2000);
    emotivProcThread = new EmotivProcessThread;
}


MainWindow::~MainWindow()
{
    delete emotivProcThread;
    delete Settings;
    delete ComBoxItemDelegate;
    delete stateBar;
    delete ui;   
}

/**
 * @brief MainWindow::IPPortSettingInit
 */
void MainWindow::IPPortSettingInit()
{
    bool state;
    ui->IPLabel->setText(tr("IP:"));
    ui->PortLabel->setText(tr("Port:"));
    ui->PortLineEdit->setValidator(new QIntValidator(0, 65535, this));

    if((state = Settings->value(IP_PORT_STATE).toBool()) == NULL)
    {
        Settings->setValue(IP_PORT_STATE,IP_Port_state);
        ui->IPPortRadioButton->setChecked(false);
        ui->IP_PortSureButton->hide();
        ui->IPLabel->hide();
        ui->IPLineEdit->hide();
        ui->PortLabel->hide();
        ui->PortLineEdit->hide();
        ui->IPPortRadioButton->setText(tr("Close"));
    }
    else
    {
        if(state)
        {
            ui->IPPortRadioButton->setChecked(true);
        }
        else
        {
            ui->IPPortRadioButton->setChecked(false);
            ui->IP_PortSureButton->hide();
            ui->IPLabel->hide();
            ui->IPLineEdit->hide();
            ui->PortLabel->hide();
            ui->PortLineEdit->hide();
            ui->IPPortRadioButton->setText(tr("Close"));
        }
    }

}

/**
 * @brief MainWindow::SerialPortSettingInit
 */
void MainWindow::SerialPortSettingInit()
{
    bool state;
    QString str;
    int i;
    ComBoxItemDelegate = new QStyledItemDelegate(this);
    ui->COMcomboBox->setItemDelegate(ComBoxItemDelegate);
    ui->BaudcomboBox->setItemDelegate(ComBoxItemDelegate);

    ui->COMlabel->setText(tr("COM:"));
    ui->Baudlabel->setText(tr("Baud Rate:"));

    for (i = 0; i < 6; i++)
    {
        str.clear();
        str.append("COM");
        str.append(QString::number(i));
        ui->COMcomboBox->addItem(str);
    }

    ui->BaudcomboBox->addItem(QString::number(4800));
    ui->BaudcomboBox->addItem(QString::number(9600));
    ui->BaudcomboBox->addItem(QString::number(19200));
    ui->BaudcomboBox->addItem(QString::number(38400));
    ui->BaudcomboBox->addItem(QString::number(57600));
    ui->BaudcomboBox->addItem(QString::number(115200));

    if((state = Settings->value(SERIAL_PORT_STATE).toBool()) == NULL)
    {
        Settings->setValue(IP_PORT_STATE,Serial_Port_state);
        ui->SerialportBtton->hide();
        ui->COMcomboBox->hide();
        ui->COMlabel->hide();
        ui->BaudcomboBox->hide();
        ui->Baudlabel->hide();
        ui->SerialPortRadioButton->setText(tr("Close"));
    }
    else
    {
        if(state)
        {
            ui->SerialPortRadioButton->setChecked(true);
        }
        else
        {
            ui->SerialPortRadioButton->setChecked(false);
            ui->SerialportBtton->hide();
            ui->COMcomboBox->hide();
            ui->COMlabel->hide();
            ui->BaudcomboBox->hide();
            ui->Baudlabel->hide();
            ui->SerialPortRadioButton->setText(tr("Close"));
        }
    }
}


/**
 * @brief MainWindow::SkinChangedSlot
 * @param skinKey
 */
void MainWindow::SkinChangedSlot(uint skinKey)
{
    QPalette pe;
    QPen pen;
    skinColor = skinKey;

    IconHelper::Instance()->SetIcon(ui->LineChartGroupBox, QChar(LINECHART_ICON),tr("Line Chart"), 12);
    IconHelper::Instance()->SetIcon(ui->StateGroupBox, QChar(STATE_ICON), tr("State"),12);
    IconHelper::Instance()->SetIcon(ui->ControlGroupBox, QChar(CONTROL_ICON),tr("Control") ,12);
    IconHelper::Instance()->SetIcon(ui->BatteryGroupBox, QChar(BATTERY_ICON),tr("Battery"), 12);
    IconHelper::Instance()->SetIcon(ui->ConnectQualityGroupBox, QChar(CONNECTQUALITY_ICON),tr("Connect Quality"), 12);
    IconHelper::Instance()->SetIcon(ui->ConnectQualityGroupBox_2, QChar(CONTROL_ICON),tr("Connect Quality"), 12);
    IconHelper::Instance()->SetIcon(ui->IP_Port_groupBox, QChar(IPPORT_ICON),tr("IP Port Setting"), 12);
    IconHelper::Instance()->SetIcon(ui->SerialPortGroupBox, QChar(SERIALPORT_ICON),tr("Serial Port Setting"), 12);

    pe.setColor(QPalette::WindowText,QColor(skinKey));
    pen.setColor(QColor(skinKey));
    ui->LineChartGroupBox->setPalette(pe);
    ui->StateGroupBox->setPalette(pe);
    ui->ControlGroupBox->setPalette(pe);
    ui->BatteryGroupBox->setPalette(pe);
    ui->ConnectQualityGroupBox->setPalette(pe);
    ui->ConnectQualityGroupBox_2->setPalette(pe);
    ui->IP_Port_groupBox->setPalette(pe);
    ui->SerialPortGroupBox->setPalette(pe);

    ui->LineChartWidget->xAxis->setBasePen(pen);
    ui->LineChartWidget->yAxis->setBasePen(pen);
    ui->LineChartWidget->xAxis->setTickPen(pen);
    ui->LineChartWidget->yAxis->setTickPen(pen);
    ui->LineChartWidget->xAxis->setSubTickPen(pen);
    ui->LineChartWidget->yAxis->setSubTickPen(pen);
    ui->LineChartWidget->xAxis->setTickLabelColor(QColor(skinKey));
    ui->LineChartWidget->yAxis->setTickLabelColor(QColor(skinKey));
    ui->LineChartWidget->replot();

    IconHelper::Instance()->SetIcon(ui->StartButton, QChar(START_ICON),tr("Start"), 14);
    IconHelper::Instance()->SetIcon(ui->StopButton, QChar(STOP_ICON), tr("Stop"),14);
    stateBar->setPalette(pe);

    Settings->setValue(SKIN_NUMBER,skinKey);
}

/**
 * @brief MainWindow::LanguageChangedSlot
 * @param value
 */
void MainWindow::LanguageChangedSlot(int value)
{
    Settings->setValue(LANGUAGE_NUMBER, value);
}

/**
 * @brief MainWindow::SetConnectQuality
 */
void MainWindow::SetConnectQuality()
{
    switch(Cannel[0])
    {
        case 0:ui->AF3label->setStyleSheet("image:url(://Image/AF3.png);");break;
        case 1:ui->AF3label->setStyleSheet("image:url(://Image/AF3_r.png);");break;
        case 2:ui->AF3label->setStyleSheet("image:url(://Image/AF3_y.png);");break;
        case 3:ui->AF3label->setStyleSheet("image:url(://Image/AF3_g.png);");break;
        default:break;
    }
    switch(Cannel[1])
    {
        case 0:ui->AF4label->setStyleSheet("image:url(://Image/AF4.png);");break;
        case 1:ui->AF4label->setStyleSheet("image:url(://Image/AF4_r.png);");break;
        case 2:ui->AF4label->setStyleSheet("image:url(://Image/AF4_y.png);");break;
        case 3:ui->AF4label->setStyleSheet("image:url(://Image/AF4_g.png);");break;
        default:break;
    }
    switch(Cannel[2])
    {
        case 0:ui->F7label->setStyleSheet("image:url(://Image/F7.png);");break;
        case 1:ui->F7label->setStyleSheet("image:url(://Image/F7_r.png);");break;
        case 2:ui->F7label->setStyleSheet("image:url(://Image/F7_y.png);");break;
        case 3:ui->F7label->setStyleSheet("image:url(://Image/F7_g.png);");break;
        default:break;
    }
    switch(Cannel[3])
    {
        case 0:ui->F3label->setStyleSheet("image:url(://Image/F3.png);");break;
        case 1:ui->F3label->setStyleSheet("image:url(://Image/F3_r.png);");break;
        case 2:ui->F3label->setStyleSheet("image:url(://Image/F3_y.png);");break;
        case 3:ui->F3label->setStyleSheet("image:url(://Image/F3_g.png);");break;
        default:break;
    }
    switch(Cannel[4])
    {
        case 0:ui->F4label->setStyleSheet("image:url(://Image/F4.png);");break;
        case 1:ui->F4label->setStyleSheet("image:url(://Image/F4_r.png);");break;
        case 2:ui->F4label->setStyleSheet("image:url(://Image/F4_y.png);");break;
        case 3:ui->F4label->setStyleSheet("image:url(://Image/F4_g.png);");break;
        default:break;
    }
    switch(Cannel[5])
    {
        case 0:ui->F8label->setStyleSheet("image:url(://Image/F8.png);");break;
        case 1:ui->F8label->setStyleSheet("image:url(://Image/F8_r.png);");break;
        case 2:ui->F8label->setStyleSheet("image:url(://Image/F8_y.png);");break;
        case 3:ui->F8label->setStyleSheet("image:url(://Image/F8_g.png);");break;
        default:break;
    }
    switch(Cannel[6])
    {
        case 0:ui->Fc5label->setStyleSheet("image:url(://Image/Fc5.png);");break;
        case 1:ui->Fc5label->setStyleSheet("image:url(://Image/Fc5_r.png);");break;
        case 2:ui->Fc5label->setStyleSheet("image:url(://Image/Fc5_y.png);");break;
        case 3:ui->Fc5label->setStyleSheet("image:url(://Image/Fc5_g.png);");break;
        default:break;
    }
    switch(Cannel[7])
    {
        case 0:ui->Fc6label->setStyleSheet("image:url(://Image/Fc6.png);");break;
        case 1:ui->Fc6label->setStyleSheet("image:url(://Image/Fc6_r.png);");break;
        case 2:ui->Fc6label->setStyleSheet("image:url(://Image/Fc6_y.png);");break;
        case 3:ui->Fc6label->setStyleSheet("image:url(://Image/Fc6_g.png);");break;
        default:break;
    }
    switch(Cannel[8])
    {
        case 0:ui->T7label->setStyleSheet("image:url(://Image/T7.png);");break;
        case 1:ui->T7label->setStyleSheet("image:url(://Image/T7_r.png);");break;
        case 2:ui->T7label->setStyleSheet("image:url(://Image/T7_y.png);");break;
        case 3:ui->T7label->setStyleSheet("image:url(://Image/T7_g.png);");break;
        default:break;
    }
    switch(Cannel[9])
    {
        case 0:ui->T8label->setStyleSheet("image:url(://Image/T8.png);");break;
        case 1:ui->T8label->setStyleSheet("image:url(://Image/T8_r.png);");break;
        case 2:ui->T8label->setStyleSheet("image:url(://Image/T8_y.png);");break;
        case 3:ui->T8label->setStyleSheet("image:url(://Image/T8_g.png);");break;
        default:break;
    }
    switch(Cannel[10])
    {
        case 0:ui->CMSlabel->setStyleSheet("image:url(://Image/CMS.png);");break;
        case 1:ui->CMSlabel->setStyleSheet("image:url(://Image/CMS_r.png);");break;
        case 2:ui->CMSlabel->setStyleSheet("image:url(://Image/CMS_y.png);");break;
        case 3:ui->CMSlabel->setStyleSheet("image:url(://Image/CMS_g.png);");break;
        default:break;
    }
    switch(Cannel[11])
    {
        case 0:ui->DRLlabel->setStyleSheet("image:url(://Image/DRL.png);");break;
        case 1:ui->DRLlabel->setStyleSheet("image:url(://Image/DRL_r.png);");break;
        case 2:ui->DRLlabel->setStyleSheet("image:url(://Image/DRL_y.png);");break;
        case 3:ui->DRLlabel->setStyleSheet("image:url(://Image/DRL_g.png);");break;
        default:break;
    }
    switch(Cannel[12])
    {
        case 0:ui->P7label->setStyleSheet("image:url(://Image/P7.png);");break;
        case 1:ui->P7label->setStyleSheet("image:url(://Image/P7_r.png);");break;
        case 2:ui->P7label->setStyleSheet("image:url(://Image/P7_y.png);");break;
        case 3:ui->P7label->setStyleSheet("image:url(://Image/P7_g.png);");break;
        default:break;
    }
    switch(Cannel[13])
    {
        case 0:ui->P8label->setStyleSheet("image:url(://Image/P8.png);");break;
        case 1:ui->P8label->setStyleSheet("image:url(://Image/P8_r.png);");break;
        case 2:ui->P8label->setStyleSheet("image:url(://Image/P8_y.png);");break;
        case 3:ui->P8label->setStyleSheet("image:url(://Image/P8_g.png);");break;
        default:break;
    }
    switch(Cannel[14])
    {
        case 0:ui->O1label->setStyleSheet("image:url(://Image/O1.png);");break;
        case 1:ui->O1label->setStyleSheet("image:url(://Image/O1_r.png);");break;
        case 2:ui->O1label->setStyleSheet("image:url(://Image/O1_y.png);");break;
        case 3:ui->O1label->setStyleSheet("image:url(://Image/O1_g.png);");break;
        default:break;
    }
    switch(Cannel[15])
    {
        case 0:ui->O2label->setStyleSheet("image:url(://Image/O2.png);");break;
        case 1:ui->O2label->setStyleSheet("image:url(://Image/O2_r.png);");break;
        case 2:ui->O2label->setStyleSheet("image:url(://Image/O2_y.png);");break;
        case 3:ui->O2label->setStyleSheet("image:url(://Image/O2_g.png);");break;
        default:break;
    }
}

/**
 * @brief MainWindow::LineChartInit
 */
void MainWindow::LineChartInit()
{
    connect(ui->LineChartWidget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));//连接鼠标点击信号和槽
    connect(ui->LineChartWidget, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));//连接鼠标滚轮信号和槽
    connect(ui->LineChartWidget, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));//连接曲线选择信号和槽
    ui->LineChartWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |	//设置交互方式
                                       QCP::iSelectLegend | QCP::iSelectPlottables);
    QBrush qBrush(QColor(240, 240, 240));//设置背景色
    ui->LineChartWidget->setBackground(qBrush);
}

/**
 * @brief MainWindow::SetBattery
 * @param value
 */
void MainWindow::SetBattery(int value)
{
    if(value > 20)
    {
        QString buf = "QProgressBar::chunk {width: 5px; margin: 0.5px;background-color: #";
        buf.append(QString::number(skinColor));
        buf.append(";}");
        ui->BatteryProgressBar->setStyleSheet(buf);

        buf.clear();
        buf.append("QLabel#BatteryLabel{color: #");
        buf.append(QString::number(skinColor));
        buf.append(";}");
        ui->BatteryLabel->setStyleSheet(buf);
        ui->BatteryLabelPercent->setStyleSheet(buf);
    }
    else
    {
        ui->BatteryProgressBar->setStyleSheet("QProgressBar::chunk {width: 5px; margin: 0.5px;background-color: #ff0000;}");
        ui->BatteryLabel->setStyleSheet("QLabel#BatteryLabel{color: #ff0000;}");
        ui->BatteryLabelPercent->setStyleSheet("QLabel#BatteryLabelPercent{color: #ff0000;}");
    }
    ui->BatteryProgressBar->setValue(value);
    ui->BatteryLabel->setText(QString::number(value));
}

/**
 * @brief MainWindow::on_StartButton_clicked
 */
void MainWindow::on_StartButton_clicked()
{
    int emotiv_intit_res = emotivProcThread->emotiv->emotiv_init();
    while( emotiv_intit_res > 0 )
    {
        MsgBox msgBox;
        msgBox.Waring("Emotiv Init Failed!\nEmotiv Restart Init!");
        if( msgBox.exec())
        {
            emotiv_intit_res = emotivProcThread->emotiv->emotiv_init();
        }else
        {
            emotiv_intit_res = -2;
        }
    }

    if(emotiv_intit_res == 0)
    {
        emotivProcThread->start();
    }
}

/**
 * @brief MainWindow::on_StopButton_clicked
 */
void MainWindow::on_StopButton_clicked()
{

}

/**
 * @brief MainWindow::on_IP_PortSureButton_clicked
 */
void MainWindow::on_IP_PortSureButton_clicked()
{

}

/**
 * @brief MainWindow::on_SerialportBtton_clicked
 */
void MainWindow::on_SerialportBtton_clicked()
{

}
\
/**
 * @brief MainWindow::on_IPPortRadioButton_clicked
 * @param checked
 */
void MainWindow::on_IPPortRadioButton_clicked(bool checked)
{
    if(checked == false)
    {
        ui->IP_PortSureButton->hide();
        ui->IPLabel->hide();
        ui->IPLineEdit->hide();
        ui->PortLabel->hide();
        ui->PortLineEdit->hide();
        ui->IPPortRadioButton->setText(tr("Close"));
        IP_Port_state = false;
    }
    else
    {
        ui->IP_PortSureButton->show();
        ui->IPLabel->show();
        ui->IPLineEdit->show();
        ui->PortLabel->show();
        ui->PortLineEdit->show();
        ui->IPPortRadioButton->setText(tr("Open"));
        IP_Port_state = true;
    }
    Settings->setValue(IP_PORT_STATE,IP_Port_state);
}

/**
 * @brief MainWindow::on_SerialPortRadioButton_clicked
 * @param checked
 */
void MainWindow::on_SerialPortRadioButton_clicked(bool checked)
{
    if(checked == false)
    {
        ui->SerialportBtton->hide();
        ui->COMcomboBox->hide();
        ui->COMlabel->hide();
        ui->BaudcomboBox->hide();
        ui->Baudlabel->hide();
        ui->SerialPortRadioButton->setText(tr("Close"));
        Serial_Port_state = false;
    }
    else
    {
        ui->SerialportBtton->show();
        ui->COMcomboBox->show();
        ui->COMlabel->show();
        ui->BaudcomboBox->show();
        ui->Baudlabel->show();
        ui->SerialPortRadioButton->setText(tr("Open"));
        Serial_Port_state = true;
    }
    Settings->setValue(SERIAL_PORT_STATE,Serial_Port_state);
}



/**
 * @brief MainWindow::mousePress
　*　鼠标点击
 */
void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  if (ui->LineChartWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->LineChartWidget->axisRect()->setRangeDrag(ui->LineChartWidget->xAxis->orientation());
  }
  else if (ui->LineChartWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->LineChartWidget->axisRect()->setRangeDrag(ui->LineChartWidget->yAxis->orientation());
  }
  else
  {
    ui->LineChartWidget->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
  }
}

/**
 * @brief MainWindow::mouseWheel
　*　鼠标滚轮
 */
void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  if (ui->LineChartWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->LineChartWidget->axisRect()->setRangeZoom(ui->LineChartWidget->xAxis->orientation());
  }
  else if (ui->LineChartWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->LineChartWidget->axisRect()->setRangeZoom(ui->LineChartWidget->yAxis->orientation());
  }
  else
  {
    ui->LineChartWidget->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
  }
}

/**
 * @brief MainWindow::selectionChanged
 * 曲线选择
 */
void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->LineChartWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->LineChartWidget->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->LineChartWidget->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->LineChartWidget->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->LineChartWidget->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->LineChartWidget->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->LineChartWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->LineChartWidget->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->LineChartWidget->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->LineChartWidget->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->LineChartWidget->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->LineChartWidget->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->LineChartWidget->graphCount(); ++i)
  {
    QCPGraph *graph = ui->LineChartWidget->graph(i);
    QCPPlottableLegendItem *item = ui->LineChartWidget->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}















