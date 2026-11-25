#include "widget.h"
#include <ui/ui_widget.h>
#include "vqcustomplot.h"
#include<QSerialPortInfo>

#include <QWidget>
#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>

#include <QFileInfo>
#include <QList>
#include <QStringList>
#include <QKeyEvent>
#include <iostream>
#include <string>

#include <QTextDocument>
#include "vqcustomplot.h"
#include "appconfig.h"
#include "Datapacket.h"
#include "SonicLaunchCfg.h"

#include <QtWidgets/QGesture>
#include <QtWidgets/QGestureEvent>
#include <QtCore/QDebug>
#include <QMessageBox>

#include<valarray>//模板类数组的标准头文件
#include <complex>		//模板类complex的标准头文件
using namespace std;

//自动获取IP的函数
#include<QVector>
#include<QDebug>
#include<iostream>

/* 地震文件相关 */
#include "seg_y.h"
#include "seg_2.h"
/* 地震文件相关 */

/* FFt */
#define AUDIBLE_RANGE_START 0
#define AUDIBLE_RANGE_END   20000 /* very optimistic */
#define SAMPLES_CNT 4096 //4096
//#define SAMPLE_FREQ 500000 //500000
/* FFt */


Widget::Widget(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::Widget)
    ,UartToWifiDataReadyWaitTimer(new QTimer)
    ,ContinueTriggerTimer(new QTimer)
    ,data_packet(new DataPacket)
    ,ConfigWindow(new SonicLaunchCfg)
    ,scene(new QGraphicsScene)
    ,w1(new QGraphicsProxyWidget)
    ,serial_port(new Serial_Port) /* 串口 */
    ,udp_ultrasnoic_master(new UdpData) /* UDP */
    ,udp2_data(new UdpData) /* UDP */
{
#if 1
    ui->setupUi(this);

    // widget.ui主窗口设置无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);


    /*PLOT_TYPE_E为自定义类型，确保你在使用该类型发送信号之前，调用了qRegisterMetaType函数来注册这个类型*/
    qRegisterMetaType<vQCustomPlot::PLOT_TYPE_E>("vQCustomPlot::PLOT_TYPE_E");

    this->initConfig();//初始化配置文件
    this->vRefreshTriggerParameter();//初始化配置状态栏

    initSerialPort();
    initUdp(); //要放在初始化配置文件以后才行


    grabGesture(Qt::PinchGesture);//”捏“手势
    this->setAttribute(Qt::WA_AcceptTouchEvents);


    //绑定计米轮串口转wifi服务器的UDP超时接收结束的定时器，防止UDP数据包一次发送不完整，
    connect(UartToWifiDataReadyWaitTimer, SIGNAL(timeout()), this, SLOT(UartToWifiDataReadyWaitTimerOut()));

    //绑定连续触发的定时器，
    connect(ContinueTriggerTimer, SIGNAL(timeout()), this, SLOT(ContinueTriggerTimerOut()));

    m_customPlots = {
        ui->customPlot1, ui->customPlot2, ui->customPlot3, ui->customPlot4,
        ui->customPlot5, ui->customPlot6, ui->customPlot7, ui->customPlot8,
        ui->customPlot9, ui->customPlot10, ui->customPlot11, ui->customPlot12
    };

    // 设置每个通道的名称
    for (int i = 0; i < m_customPlots.size(); ++i) {
        m_customPlots[i]->graph()->setName(QString("通道%1").arg(i+1));
    }

    // 初始化通道波形显示选择控件数组
    m_checkBoxes = {
        ui->checkBox_1, ui->checkBox_2, ui->checkBox_3, ui->checkBox_4,
        ui->checkBox_5, ui->checkBox_6, ui->checkBox_7, ui->checkBox_8,
        ui->checkBox_9, ui->checkBox_10, ui->checkBox_11, ui->checkBox_12
    };
    // 连接所有通道波形复选框信号
    for (int i = 0; i < m_checkBoxes.size(); ++i)
    {
        connect(m_checkBoxes[i], &QCheckBox::stateChanged,
                this, [this, i](int state) { handlePlotVisibility(i, state); });
    }

    // 绑定图形绘制数据传输的信号与槽
    for (int i = 0; i < m_customPlots.size(); ++i) {
        auto plot = m_customPlots[i];
        connect(this, &Widget::RxScopeAll,
               plot,
               [i, plot](int channel, const QVariant &rxData,
                       QString GraphName, double xAxisFactor,
                       vQCustomPlot::PLOT_TYPE_E ePlotType) {
                   if (channel == i + 1) {
                       plot->UltraSonicScope(rxData, GraphName, xAxisFactor, ePlotType);
                   }
               },
               Qt::QueuedConnection);
    }

    // 连接同步缩放与平移信号到槽函数
    connect(ui->radioButton, &QRadioButton::toggled, this, [=](bool checked)
    {
        if (checked) {
            // 创建一个lambda函数来处理范围变化
            auto rangeHandler = [=](const QCPRange &rangeX, const QCPRange &rangeY) {
                for (auto plot : m_customPlots)
                {
                    plot->xAxis->setRange(rangeX);
                    plot->yAxis->setRange(rangeY);
                    plot->replot();
                }
            };

            // 为每个customPlot连接信号
            for (auto plot : m_customPlots)
            {
                connect(plot, &vQCustomPlot::CustomPlotRangeChanged, rangeHandler);
            }
        }
        else
        {
            // 断开所有连接
            for (auto plot : m_customPlots)
            {
                disconnect(plot, &vQCustomPlot::CustomPlotRangeChanged, nullptr, nullptr);
            }
        }
    });
 #endif
    ui->radioButton->setChecked(AppConfig::GlobalZoom);
    if((data_packet->strTriggerMode == "连续触发")||(data_packet->strTriggerMode == "连续外部触发"))
    {
        ui->pBtTriggerStop->setEnabled(true);
    }
}

// 析构函数
Widget::~Widget()
{
    delete ui;
}

// 在第一个窗口的缩放事件中发出信号
void Widget::onCustomPlot1RangeChanged()
{
    QCPRange newRange = ui->customPlot1->xAxis->range();
    // 发出自定义信号，携带新的范围
    rangeChanged(newRange);
}

// 在第二个窗口中连接上述信号，并执行缩放操作
void Widget::connectCustomPlot2()
{
    // 连接信号到槽函数
    connect(this, &Widget::rangeChanged, [=](const QCPRange &range) {
        ui->customPlot2->xAxis->setRange(range);
        ui->customPlot2->yAxis->setRange(range);
    });
}

// 串口转Wifi数据接收时间完成处理函数
void Widget::UartToWifiDataReadyWaitTimerOut()
{
    UartToWifiDataReadyWaitTimer->stop();
    //对串口转wifi的设备数据进行解析和更新显示
    DataUartToWifiBuf.clear(); //清除本次接收的数据
}
// 连续声波触发模式超时处理
void Widget::ContinueTriggerTimerOut()
{
    udp_ultrasnoic_master->sendData(data_packet->QbDataPacket);//用于超声波信号主机，发射开机指令
    ContinueTriggerTimer->start(AppConfig::u4PulsePeriod);
}

void Widget::recv_slot()
{

}
/*************************************************串口***************************************************/
//初始化串口
void Widget::initSerialPort()
{
    // 获取可用串口列表
    QStringList ports = serial_port->getAvailablePorts();
    if (ports.isEmpty()) {
        qDebug() << "No serial ports available!";
        return;
    }
    else
    {
       ui->serialCb->addItems(ports);
    }
#if 0
    // 设置数据接收回调
    serial_port->setDataReceivedCallback([](const QByteArray &data) {
        qDebug() << "Received data:" << data.toHex();
        // 这里可以添加数据解析逻辑
    });
    // 也可以使用信号槽方式接收数据
    QObject::connect(&serial_port, &Serial_Port::dataReceived, [](const QByteArray &data) {
        qDebug() << "Data received via signal:" << data;
    });
#endif
}

//串口打开按钮槽函数
void Widget::on_open_clicked()
{
    if(serial_port->openWithUI(ui->serialCb, ui->baundrateCb,
                             ui->dateCb, ui->stopCb, ui->checkCb))
    {
        // 打开成功
        ui->textBrowser->append("串口打开成功");
    }
    else
    {
        QMessageBox::critical(this, "错误", "无法打开串口，请检查设置和连接");
    }
}

//串口内容清楚
void Widget::on_clear_clicked()
{
    ui->textBrowser->clear();//清空内容
}

void Widget::on_send_clicked()
{
    //判断串口是否打开
    if(serial_port->isOpen())
    {
        QByteArray sendData = "Hello, Serial Port!";
        serial_port->sendData(ui->lineEdit->text().toUtf8().data());
        ui->textBrowser->append(ui->lineEdit->text().toUtf8().data() );
    }else
    {
        ui->textBrowser->append("请打开串口");
    }
}

void Widget::on_close_clicked()
{
    serial_port->closePort();
    ui->textBrowser->append("已断开串口");
}
/*************************************************串口*********END******************************************/

/*************************************************对波形通道裁剪进行设置****START*******************************/
/*对单个按钮的波形显示窗口进行处理*/
void Widget::handlePlotVisibility(int index, int state)
{
    Q_ASSERT(index >= 0 && index < m_customPlots.size());

    ui->groupBox_9->layout()->invalidate();
    m_customPlots[index]->setVisible(state);
    ui->groupBox_9->layout()->activate();
    ui->groupBox_9->layout()->update();
}

/*打开所有通道的波形显示窗口*/
void Widget::on_pushButton_2_clicked()
{
    setAllPlotsVisible(true);
}

/*关闭所有通道的波形显示窗口*/
void Widget::on_pushButton_4_clicked()
{
    setAllPlotsVisible(false);
}

void Widget::setAllPlotsVisible(bool visible)
{
    ui->groupBox_9->layout()->invalidate();

    // 设置所有图表可见性
    for (auto plot : m_customPlots) {
        plot->setVisible(visible);
    }
    // 设置所有复选框状态
    for (auto checkBox : m_checkBoxes) {
        checkBox->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
    }

    ui->groupBox_9->layout()->activate();
    ui->groupBox_9->layout()->update();
}
/*************************************************对波形通道裁剪进行设置****END***********************************************/

/*进行触发激励动作执行成*/
void Widget::on_pBtTriggerEn_clicked()
{
    data_packet->QbDataPacket.clear();
    data_packet->initDataPacket();//初始化需要发送的数据包
    QString strTemp = "通道：" + data_packet->strTriggerChannelSelected + "； 方式：" + data_packet->strTriggerMode+ "； 点数：" \
                    + data_packet->strSampleCount + "； 高压：" + data_packet->strVoltageValue + "； 间隔：" + data_packet->strDecimateSpan \
                    + "ns； 硬件增益：" + data_packet->strHardwareAmplifyFactor + " 倍； 脉宽："  + data_packet->strPulseWidth + " us； 周期：" \
                    + data_packet->strPulsePeriod + "ms；";
    //更新此次发射的触发配置信息
    ui->LbTriggerParameter->setText(strTemp);
    ui->LbTriggerParameter->show();

    udp_ultrasnoic_master->sendData(data_packet->QbDataPacket);//用于超声波信号主机，发射开机指令

    if(data_packet->strTriggerMode == "连续触发")
    {
        ContinueTriggerTimer->start(AppConfig::u4PulsePeriod); //us->ms
        ui->pBtTriggerStop->setEnabled(true);
        ui->pBtTriggerEn->setEnabled(false);
    }
    else if(data_packet->strTriggerMode == "单次触发")
    {
        data_packet->QbDataPacket.clear();
        ui->pBtTriggerStop->setEnabled(false);
    }
    else if(data_packet->strTriggerMode == "连续外部触发")
    {
    }
    else
    {
        //No operation
    }
}

/* 连续触发模式下，停止连续触发*/
void Widget::on_pBtTriggerStop_clicked()
{
    ui->pBtTriggerEn->setEnabled(true);
    ContinueTriggerTimer->stop();
    data_packet->QbDataPacket.clear();

}

/*触发参数设置窗口弹出按钮*/
void  Widget::on_pBtTriggerSetting_clicked()
{
#if 0 //采用幕布的方式可以旋转
    if(!view->isVisible())
    {
        //ConfigWindow->setWindowTitle("触发参数设置");
        connect(ConfigWindow,SIGNAL(SigRefreshTriggerParameter()),this,SLOT(vRefreshTriggerParameter()));
        w1 = scene->addWidget(ConfigWindow);
        //w1->setRotation(270);
        view = new QGraphicsView(scene);

        //采用无边框设计增大使用面积，采用窗口置頂
        view->setWindowFlags(Qt::FramelessWindowHint);
        //采用窗口置頂
        //view->setWindowFlags(Qt::WindowStaysOnTopHint);
        view->setWindowTitle("触发参数设置");
        view->show();
    }
#else //采用传统的方式不可以旋转
    if(!ConfigWindow->isVisible())
    {
        ConfigWindow->setWindowTitle("触发参数设置");
        connect(ConfigWindow,SIGNAL(SigRefreshTriggerParameter()),this,SLOT(vRefreshTriggerParameter()));

        //采用无边框设计增大使用面积
        //setWindowFlags(Qt::FramelessWindowHint);
        //采用窗口置頂
        ConfigWindow->setWindowFlags(ConfigWindow->windowFlags()  | Qt::WindowStaysOnTopHint);
        ConfigWindow->show();
    }
#endif
}

//显示当前触发设置模式
void Widget::vRefreshTriggerParameter()
{
    bool ok;
    data_packet->initDataPacket();//此次初始化仅是为了更新发射配置的字符串状态，
    QString strTemp = "通道：" + data_packet->strTriggerChannelSelected + "； 触发方式：" + data_packet->strTriggerMode+ "； 采样点数：" \
                    + data_packet->strSampleCount + "； 激励电压：" + data_packet->strVoltageValue + "； 采样间隔：" + data_packet->strDecimateSpan \
                    + "ns； 硬件增益：" + data_packet->strHardwareAmplifyFactor + " 倍； 激励脉宽："  + data_packet->strPulseWidth + " us； 触发周期：" \
                    + data_packet->strPulsePeriod + "ms；";
    //更新此次发射的触发配置信息
    ui->LbTriggerParameter->setText(strTemp);
    ui->LbTriggerParameter->show();
#if 0 //适配可以旋转幕布的方式
    //if(view->isVisible())
    {
        this->view->close(); // 此句话测试中会出现程序异常结束，暂时没解决
    }
#else
    if(ConfigWindow->isVisible())
    {
        ConfigWindow->close();
    }
#endif
    xAxisToTimeFactor = (data_packet->strDecimateSpan.toDouble(&ok)) / 1000.0; //同步更新因发射参数改变导致的采样间隔改变，也即更新波形绘制时由采样序数到时间的X轴系数，ns->us
}

//初始化上次使用配置
void Widget::initConfig()
{
#if 1
    ui->tabWidget->setCurrentIndex(0);
    //波形是否显示保存
    ui->checkBox_1->setChecked(AppConfig::Waveform1DisplayEn);
    connect(ui->checkBox_1, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_2->setChecked(AppConfig::Waveform2DisplayEn);
    connect(ui->checkBox_2, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_3->setChecked(AppConfig::Waveform3DisplayEn);
    connect(ui->checkBox_3, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_4->setChecked(AppConfig::Waveform4DisplayEn);
    connect(ui->checkBox_4, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_5->setChecked(AppConfig::Waveform5DisplayEn);
    connect(ui->checkBox_5, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_6->setChecked(AppConfig::Waveform6DisplayEn);
    connect(ui->checkBox_6, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_7->setChecked(AppConfig::Waveform7DisplayEn);
    connect(ui->checkBox_7, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_8->setChecked(AppConfig::Waveform8DisplayEn);
    connect(ui->checkBox_8, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_9->setChecked(AppConfig::Waveform9DisplayEn);
    connect(ui->checkBox_9, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_10->setChecked(AppConfig::Waveform10DisplayEn);
    connect(ui->checkBox_10, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_11->setChecked(AppConfig::Waveform11DisplayEn);
    connect(ui->checkBox_11, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    ui->checkBox_12->setChecked(AppConfig::Waveform12DisplayEn);
    connect(ui->checkBox_12, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
#endif

    connect(ui->radioButton, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));

    //udp
    ui->LineEditListening_Port->setText(QString::number(AppConfig::ListenPort));
    ui->lineEdit_DestIP->setText(AppConfig::DestIP);
    //目标端口
    ui->lineEdit_DestPort->setText(QString::number(AppConfig::u4DestPort));
    connect(ui->pbtSaveIP, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
}

//保存对当前界的面配置
void Widget::saveConfig()
{
    AppConfig::Waveform1DisplayEn = ui->checkBox_1->isChecked();
    AppConfig::Waveform2DisplayEn = ui->checkBox_2->isChecked();
    AppConfig::Waveform3DisplayEn = ui->checkBox_3->isChecked();
    AppConfig::Waveform4DisplayEn = ui->checkBox_4->isChecked();
    AppConfig::Waveform5DisplayEn = ui->checkBox_5->isChecked();
    AppConfig::Waveform6DisplayEn = ui->checkBox_6->isChecked();
    AppConfig::Waveform7DisplayEn = ui->checkBox_7->isChecked();
    AppConfig::Waveform8DisplayEn = ui->checkBox_8->isChecked();
    AppConfig::Waveform9DisplayEn = ui->checkBox_9->isChecked();
    AppConfig::Waveform10DisplayEn= ui->checkBox_10->isChecked();
    AppConfig::Waveform11DisplayEn= ui->checkBox_11->isChecked();
    AppConfig::Waveform12DisplayEn= ui->checkBox_12->isChecked();
    AppConfig::Waveform12DisplayEn= ui->radioButton->isChecked();

    //主机监听端口
    AppConfig::ListenPort = ui->LineEditListening_Port->text().toInt();

    //目标IP
    AppConfig::DestIP = ui->lineEdit_DestIP->text();
    //目标端口
    AppConfig::u4DestPort = ui->lineEdit_DestPort->text().toInt();

    //波形整体缩放
    AppConfig::GlobalZoom= ui->radioButton->isChecked();
    AppConfig::writeConfig();
}


//初始化Udp
void Widget::initUdp()
{
    /**************************************UDP通信****************************************************************/
    // 初始化UDP1连接
    QStringList sLocalIp = udp_ultrasnoic_master->getLocalIpAddresses();
    ui->cBox_IP_List->addItems(sLocalIp);
    // 参数: 目标IP, 目标端口, 本地端口(0表示自动分配)
    if (!udp_ultrasnoic_master->initUdpConnection(ui->lineEdit_DestIP->text(), ui->lineEdit_DestPort->text().toInt(),
                                     ui->LineEditListening_Port->text().toInt()))
    {
        qDebug() << "Failed to initialize UDP connection";
        qDebug() << "目标IP地址:" << ui->lineEdit_DestIP->text();
        // 打印端口值
        qDebug() << "监听端口:" << ui->LineEditListening_Port->text().toInt();
        qDebug() << "目标端口:" << ui->lineEdit_DestPort->text().toInt();
        return ;
    }
#if USE_UDP2
    // 初始化UDP2连接
    // 参数: 目标IP, 目标端口, 本地端口(0表示自动分配)
    if (!udp2_data->initUdpConnection(ui->lineEdit_DestIP->text(), ui->lineEdit_DestPort->text().toInt(),
                                     ui->LineEditListening_Port->text().toInt()))
    {
        qDebug() << "Failed to initialize UDP connection";
        return ;
    }

#endif
    QByteArray datagram;

    datagram.append(char(0xAA));
    datagram.append(char(0x00));
    datagram.append(char(0x00));
    datagram.append(char(0xBB));
    datagram.append(char(0x00));
    datagram.append(char(0x00));
    datagram.append(char(0x00));
    datagram.append(char(0x06));
    datagram.append(char(0x01));
    datagram.append(char(0x03));
    datagram.append(char(0x00));
    datagram.append(char(0x01));
    datagram.append(char(0x00));
    datagram.append(char(0x10));

    udp_ultrasnoic_master->sendData(datagram);//用于超声波信号主机，发射开机指令

    // 连接信号
    QObject::connect(udp_ultrasnoic_master, &UdpData::dataReceived, [](const QByteArray &data) {
        qDebug() << "Received data (main):" << data.toHex();
        // 在这里添加你的数据解析逻辑
    });

    QObject::connect(udp_ultrasnoic_master, &UdpData::connectionStatusChanged, [](bool connected) {
        qDebug() << "Connection status changed:" << (connected ? "Connected" : "Disconnected");
    });

    QObject::connect(udp_ultrasnoic_master, &UdpData::errorOccurred, [](const QString &errorMsg) {
        qDebug() << "Error:" << errorMsg;
    });

    //在接收端连接信号
    QObject::connect(udp_ultrasnoic_master, &UdpData::dataReceivedDone,
                    this, &Widget::handleUdpDataReceived);
}


void Widget::handleUdpDataReceived(const QVector<QVector<double>>& voltages, const QByteArray &DataBuffer)
{
    /*计算声时修正偏移点数*/
    uint32_t u4ModifyCnt = static_cast<uint32_t>(AppConfig::u4ZeroModify / (AppConfig::u4DecimateSpanCH00 * 40));

    // 处理代码
    for (int i = 0; i < UdpData::MAX_CH_CNT; ++i)
    {
        ShowQVariant[i].setValue(voltages[i]);
        QVector<double> modifiedData = ShowQVariant[i].value<QVector<double>>();
        modifiedData.insert(0, u4ModifyCnt, 1600.0);
        ShowQVariant[i].setValue(modifiedData);
        vf8VoltageChs[i] = modifiedData;
        emit RxScopeAll(i+1, ShowQVariant[i], channelNames[i], xAxisToTimeFactor, vQCustomPlot::ORIGINAL);
    }

    //UdpDataWriteGramBuf = DataBuffer;
}

//关闭程序运行
void Widget::on_pBtCloseWindow_released()
{
    exit(0); //退出程序
}

//对原始波形，傅里叶变换，滤波后波形 三种波形状态进行轮番变换
void Widget::on_pB_FFT_Transform_clicked()
{
    QString name = ui->pB_FFT_Transform->text();
    QPalette palette = ui->pB_FFT_Transform->palette();

    // 清空所有结果数组
    for (int i = 0; i < UdpData::MAX_CH_CNT; ++i) {
        FFT_TransformedResults[i].clear();
        FilteredResults[i].clear();
    }

    if (name == "FFT关")
    {
        // 设置偏移量，,滤除0点直流分量对最大值干扰
        int offset = 1;

        ui->pB_FFT_Transform->setText("FFT开");
        palette.setColor(QPalette::Button, QColor(109,226,214)); //绿色

        bool ok;
        double SampleFrequency = 1000000000 / (data_packet->strDecimateSpan.toDouble(&ok)); //uint->Hz  获取用户设置的周期并转换成频率
        double freqStep = SampleFrequency / (double)SAMPLES_CNT;

        // 处理所有通道
        for (int ch = 0; ch < UdpData::MAX_CH_CNT; ch++) {
            // 执行FFT变换
            FFT_Transform(vf8VoltageChs[ch], &FFT_TransformedResults[ch]);
            ShowQVariant[ch].setValue(FFT_TransformedResults[ch]);

            // 查找最大值
            auto maxIt = std::max_element(FFT_TransformedResults[ch].begin() + offset,
                                         FFT_TransformedResults[ch].end());
            double index = std::distance(FFT_TransformedResults[ch].begin(), maxIt);
            double maxValue = *maxIt;

            // 生成标题字符串
            QString title = QString("频谱%1-主频:%2 Hz; Max:%3 mv")
                            .arg(ch + 1)
                            .arg(index * freqStep)
                            .arg(maxValue);

            // 发射统一信号
            emit RxScopeAll(ch + 1, ShowQVariant[ch], title, freqStep, vQCustomPlot::FFT);
        }
    }
    else if(name == "FFT开")
    {
       ui->pB_FFT_Transform->setText("滤波开");

       if((AppConfig::u4LowPassFilteringCH00 == 0)&&(AppConfig::u4HighPassFilteringCH00 == 0)) //显示原始数据，不进行滤波
       {
           QMessageBox::warning(this, u8"警告", u8"未正确设置滤波参数，将显示原始数据！", u8"关闭");
           // 处理所有通道
           for (int i = 0; i < 12; ++i) {
               ShowQVariant[i].setValue(vf8VoltageChs[i]);
               emit RxScopeAll(i + 1, ShowQVariant[i],QString("通道%1").arg(i + 1),xAxisToTimeFactor,vQCustomPlot::ORIGINAL);
           }
       }
       else//进行滤波
       {
           if((AppConfig::u4LowPassFilteringCH00 == 0)&&(AppConfig::u4HighPassFilteringCH00 != 0))//进行高通滤波
           {
               FilterType = Filter::HIGHPASS;
               LowerFreq = 0;
               UpperFreq = AppConfig::u4HighPassFilteringCH00;
           }
           else if((AppConfig::u4LowPassFilteringCH00 != 0)&&(AppConfig::u4HighPassFilteringCH00 == 0))//进行低通滤波
           {
               FilterType = Filter::LOWPASS;
               LowerFreq = AppConfig::u4LowPassFilteringCH00;
               //LowerFreq = 70000;
               UpperFreq = 0;
           }
           else if((AppConfig::u4LowPassFilteringCH00 != 0)&&(AppConfig::u4HighPassFilteringCH00 != 0))//进行带通滤波
           {
               FilterType = Filter::BANDPASS;
               LowerFreq = AppConfig::u4LowPassFilteringCH00;
               UpperFreq = AppConfig::u4HighPassFilteringCH00;
           }
           else
           {
               //none
           }

           // 处理所有通道
           for (int i = 0; i < 12; ++i)
           {
               if (FilterType != Filter::NONE)
               {
                   FilterProcessing(vf8VoltageChs[i], &FilteredResults[i]);
                   ShowQVariant[i].setValue(FilteredResults[i]);
               } else
               {
                   ShowQVariant[i].setValue(vf8VoltageChs[i]);
               }
               emit RxScopeAll(i + 1, ShowQVariant[i],
                              QString("通道%1%2").arg(i + 1).arg(FilterType != Filter::NONE ? "(已滤波)" : ""),
                              xAxisToTimeFactor,
                              FilterType != Filter::NONE ? vQCustomPlot::FILTERED : vQCustomPlot::ORIGINAL);
           }
       }
    }
    else if (name == "滤波开")
    {
        ui->pB_FFT_Transform->setText("FFT关");
        palette.setColor(QPalette::Button, Qt::gray);

        for (int i = 0; i < 12; ++i)
        {
            ShowQVariant[i].setValue(vf8VoltageChs[i]);
            emit RxScopeAll(i + 1, ShowQVariant[i],QString("通道%1").arg(i + 1),xAxisToTimeFactor,vQCustomPlot::ORIGINAL);
        }
    }
    else
    {
       ui->pB_FFT_Transform->setText("异常");
    }

    ui->pB_FFT_Transform->setPalette(palette);
    ui->pB_FFT_Transform->setAutoFillBackground(true);
    ui->pB_FFT_Transform->setFlat(true);
}

/*傅里叶变换处理函数*/
void Widget::FFT_Transform(QVector<double> OriginalDataIn, QVector<double>* TransformedOut)
{
    /* Set up FFT plan */
    mFftIn = (double *)fftw_malloc(sizeof( double) * SAMPLES_CNT);
    mFftOut = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (SAMPLES_CNT / 2 + 1));
    mFftPlan = fftw_plan_dft_r2c_1d(SAMPLES_CNT, mFftIn, mFftOut,FFTW_ESTIMATE);

    memcpy(mFftIn, OriginalDataIn.data(), SAMPLES_CNT*sizeof(double));

    fftw_execute(mFftPlan);

    // 计算幅度谱   幅值处理方式：amp幅值Amplitude,ampDB为幅值加上分贝,mag为幅度谱，只是对fft结果取模，
    QVector<double> MagnitudeSpectrum(SAMPLES_CNT / 2 + 1);
    QVector<double> AmplitudeSpectrum(SAMPLES_CNT / 2 + 1);
    for (int i = 0; i < SAMPLES_CNT / 2 + 1; ++i)
    {
        MagnitudeSpectrum[i] = sqrt(mFftOut[i][0] * mFftOut[i][0] + mFftOut[i][1] * mFftOut[i][1]);
        if(i == 0)
        {
            AmplitudeSpectrum[i] = MagnitudeSpectrum[i] / SAMPLES_CNT;
        }
        else if(i == SAMPLES_CNT/2)
        {
            AmplitudeSpectrum[i] = MagnitudeSpectrum[i] / SAMPLES_CNT;
        }
        else
        {
            AmplitudeSpectrum[i] = MagnitudeSpectrum[i] / (SAMPLES_CNT/2);
        }
    }
    TransformedOut->append(AmplitudeSpectrum);  //将AmplitudeSpectrum赋值给TransformedOut
}

/*对采集的12个通道波形进行滤波处理*/
void Widget::FilterProcessing(QVector<double> FilterOriginalDataIn, QVector<double>* FilteredDataOut)
{
    WinType = Filter::RECTANGLE;
    Scala = 64;

    bool ok;
    double SampleFrequency = 1000000000 / (data_packet->strDecimateSpan.toDouble(&ok)); //uint->Hz  获取用户设置的周期并转换成频率
    Freq = SampleFrequency;
    Filter filter(FilterType, WinType, Scala, Freq, LowerFreq, UpperFreq);
    // 输出波形
    QVector<float> f4FilterOriginalDataIn(SAMPLES_CNT);

    for(uint32_t u4Temp = 0; u4Temp < SAMPLES_CNT; u4Temp++)
    {
       f4FilterOriginalDataIn[u4Temp] = (float)FilterOriginalDataIn[u4Temp];
    }
    filter.Convolution(f4FilterOriginalDataIn.data(), SAMPLES_CNT);
    if(filter.OutData == nullptr)
        return;
    int Len = 1;
    while(Len < SAMPLES_CNT)
        Len *= 2;
    Len /= 2;
    valarray<complex<long double>> pp(static_cast<size_t>(Len));
    valarray<complex<long double>> f(static_cast<size_t>(Len));

    QVector<double> xOut(SAMPLES_CNT), yOut(SAMPLES_CNT);

    double MaxOut = 0, MinOut = 0;
    for (auto i = 0; i < SAMPLES_CNT; i++) {
        xOut[i] = i;
        yOut[i] = filter.OutData[i];
        if(i < Len)
            pp[static_cast<size_t>(i)] = complex<long double>(static_cast<long double>(yOut[i]), static_cast<long double>(0.0));
        if(yOut[i] > MaxOut)
            MaxOut = yOut[i];
        if(yOut[i] < MinOut)
            MinOut = yOut[i];
    }
    FilteredDataOut->append(yOut);
}

/*对采集的12个通道波形进行保存*/
void Widget::vSaveWaveformData(QString FileName)
{
    // 生成时间戳
    QString TimeStamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    //设置文件名和存储路径，默认存在app应用下自动存储，且文件名为当前时间
    QString DirpathName = QString("%1/%2.hex").arg(qApp->applicationDirPath()).arg(FileName+TimeStamp);
    QString DirpathNameCsv = QString("%1/%2.csv").arg(qApp->applicationDirPath()).arg(FileName+TimeStamp);

    if(DirpathName!=NULL)
    {
        //保存為HEX格式的数据
        QFile file(DirpathName);
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(NULL,QStringLiteral("提示"),
                                  QStringLiteral("无法创建Hex文件"));
            return ;
        }
        else
        {
            // 写入字节数组到文件
            file.write(UdpDataWriteGramBuf);  //二进制形式
            file.close();
        }

        //保存為Csv格式的数据
        QFile fileCsv(DirpathNameCsv);
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!fileCsv.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(NULL,QStringLiteral("提示"),
                                  QStringLiteral("无法创建Csv文件"));
            return ;
        }
        else
        {
            // 创建一个文本流，用于向文件中写入数据
            QTextStream out(&fileCsv);
            // 确定两个向量中的最大长度
            int maxSize = 4096;
            // 写入CSV数据
            for (int i = 0; i < maxSize; ++i) {
                for (int ch = 0; ch < 12; ++ch) {
                    if (i < vf8VoltageChs[ch].size()) {
                        out << (vf8VoltageChs[ch])[i];
                    }
                    // 如果不是最后一列，添加逗号分隔符
                    if (ch < 11) {
                        out << ",";
                    }
                }
                out << "\n";
            }
            fileCsv.close();
        }
    }
}

/*对当前的保存波形的波形HEX文件进行恢复显示*/
void Widget::on_pB_reload_file_released()
{
    QVector<QVector<double>> voltages;
    UdpDataReadGramBuf.clear();
    UdpDataReadGramBuf = vReadWaveformData();/*对采集的12个通道波形数据进行加载读取*/
    if(UdpDataReadGramBuf.size() <= CH12_DATA_END_INDEX)
    {
        QMessageBox::warning(this, u8"警告", u8"加载文件异常！", u8"关闭");
    }
    else
    {
       udp_ultrasnoic_master->UdpDataFormBufToChannels(voltages, UdpDataReadGramBuf);
       /*计算声时修正偏移点数*/
       uint32_t u4ModifyCnt = static_cast<uint32_t>(AppConfig::u4ZeroModify / (AppConfig::u4DecimateSpanCH00 * 40));
       // 处理代码
       for (int i = 0; i < UdpData::MAX_CH_CNT; ++i)
       {
           ShowQVariant[i].setValue(voltages[i]);
           QVector<double> modifiedData = ShowQVariant[i].value<QVector<double>>();
           modifiedData.insert(0, u4ModifyCnt, 1600.0);
           ShowQVariant[i].setValue(modifiedData);
           vf8VoltageChs[i] = modifiedData;
           emit RxScopeAll(i+1, ShowQVariant[i], channelNames[i], xAxisToTimeFactor, vQCustomPlot::ORIGINAL);
       }
    }
}

/*对当前的波形进行截屏图像保存*/
void Widget::on_pB_Screenshot_clicked()
{
    // 打开文件保存对话框，让用户选择文件保存路径
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), "", tr("PNG Files (*.png)"));

    // 如果用户选择了路径
    if (!filePath.isEmpty()) {
        // 创建一个QPixmap对象，用于保存截图
        QPixmap pixmap = this->grab(); // grab() 方法会截取整个窗口的内容

        // 将截图保存为指定路径的PNG文件
        if (pixmap.save(filePath)) {
            qDebug() << "Screenshot saved to:" << filePath;
        } else {
            qDebug() << "Failed to save screenshot.";
        }
    }
}

void Widget::on_pBtSaveData_clicked()
{
#if 1
    //fileName:返回用户选择的完整文件路径（包含文件名）
    fileName = QFileDialog::getSaveFileName(
                this,
                QStringLiteral("存储"),
                qApp->applicationDirPath(),
                QStringLiteral("Files (*.segy);;Files (*.sg2)")
                );


    if (!fileName.isEmpty()) //此部分用于调用vSaveWaveformData来保存另外2中格式的数据 HEX和CSV
    {
        QFileInfo fileInfo(fileName);
        // 获取不包含扩展名的文件名
        QString baseFileName = fileInfo.baseName();
        qDebug() << "不包含扩展名的文件名: " << baseFileName;
        vSaveWaveformData(baseFileName);//保存另外2中格式的数据 HEX和CSV
    }

    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix().toLower();
    if (fileName.isEmpty())
    {
        return;
    }
    if (suffix == "segy")
    {
        // 准备通道数据 (假设有12个通道)
        // 转换为float格式
        QVector<QVector<float>> channelsData(UdpData::MAX_CH_CNT);
        for (int i = 0; i < UdpData::MAX_CH_CNT; ++i) {
            channelsData[i] = convertToFloatVector(vf8VoltageChs[i]);
        }

        // 写入SEG-Y文件
        const int sampleInterval = 500; // us
        const int sampleCount = 4096;
        const int traceDistance = 20000; // mm

        seg_y writer(fileName, sampleInterval, sampleCount, traceDistance);
        if (!writer.open())
        {
            QMessageBox::critical(this, "Error", "Failed to create SEG-Y file");
            return;
        }

        for (int i = 0; i < channelsData.size(); ++i) {
            if (!writer.writeTrace(i + 1, channelsData[i])) {
                QMessageBox::warning(this, "Warning",
                                   QString("Failed to write trace %1").arg(i + 1));
            }
        }

        writer.close();
        QMessageBox::information(this, "Success", "SEG-Y file saved successfully");
    }
    else if (suffix == "sg2")
    {
#if 1
        seg_2 processor;
        processor.processFile(fileName, vf8VoltageChs[0], vf8VoltageChs[1], vf8VoltageChs[2], vf8VoltageChs[3],
                              vf8VoltageChs[4], vf8VoltageChs[5], vf8VoltageChs[6], vf8VoltageChs[7],
                              vf8VoltageChs[8], vf8VoltageChs[9], vf8VoltageChs[10], vf8VoltageChs[11]);
#endif
    }
#endif
}

/*对采集的12个通道波形数据进行加载读取*/
QByteArray  Widget::vReadWaveformData(void)
{
    QByteArray byteArray;
    QString dirpath = QFileDialog::getOpenFileName(this,QStringLiteral("导入历史波形文件"),
                                                   qApp->applicationDirPath(), QString(tr("File (*.hex)")));
    if(dirpath!=NULL)
    {
        QFile file(dirpath);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(NULL,
                                  QStringLiteral("提示"),
                                  QStringLiteral("无法打开该文件"));
            return byteArray;
        }
        else
        {
            // 读取文件的所有内容
            byteArray = file.readAll();
            file.close();
            return byteArray;
        }
    }
}

QVector<float> Widget::convertToFloatVector(const QVector<double>& doubleVec)
{
    QVector<float> floatVec;
    floatVec.reserve(doubleVec.size());

    for (auto val : doubleVec) {
        floatVec.append(static_cast<float>(val));
    }

    return floatVec;
}

void Widget::on_pushButton_3_clicked()
{

}
