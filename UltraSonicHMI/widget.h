#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QUdpSocket>
#include<QSerialPort>
#include<qcustomplot.h>
#include"SonicLaunchCfg.h"
#include <fftw3.h>
#include <QtWidgets/QGraphicsView>


#include "serial_port.h"


#include <QPainter>
#include <QMouseEvent>
#include "QMenu"
#include "QMenuBar"
#include "QAction"
#include "QMessageBox"
#include "QFileDialog"
#include "QDebug"

#include "Datapacket.h"
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QThread>
#include <QVariant>
#include <qcustomplot.h>
#include <vtracer.h>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <QGestureEvent>
#include <QPanGesture>
#include <QPinchGesture>
#include <QSwipeGesture>
#include <QtWidgets/QGesture>
#include <QtWidgets/QGestureEvent>

#include "filter.h"
#include "vqcustomplot.h"


#include "udp_data.h"



extern QByteArray datagram;
extern QByteArray _rSegyArray;

extern int Shujvchangdu;   //数据长度
extern int Caiyangjiange;   //采样间隔
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    //void dealMsg();
    void P_plot();
    DataPacket *data_packet;
    SonicLaunchCfg *ConfigWindow;
    QGraphicsScene *scene;
    QGraphicsProxyWidget* w1;
    QGraphicsView *view ;
    Serial_Port *serial_port;
    UdpData *udp_ultrasnoic_master;//用于超声波信号主机
    UdpData *udp2_data;//用于modbus设备的串口转wifi或网口设备主机
    //QByteArray array;
    QByteArray message;
    QString getIP();
    QByteArray  UdpDataWriteGramBuf; //给写波形存储文件提供缓冲数据
    QByteArray  UdpDataReadGramBuf;  //给读波形存储文件提供缓冲数据
    QByteArray DataUartToWifiBuf;
    QByteArray _rSegyArray;

    QVector<double> vf8VoltageChs[UdpData::MAX_CH_CNT];  // 创建包含 UdpData::MAX_CH_CNT 个QVector的数组
    QStringList channelNames = {"通道1", "通道2", "通道3","通道3", "通道4", "通道5", "通道6", "通道7", "通道8","通道9", "通道10", "通道11", "通道12"};

    QString StrBsacnRunInfo;

    int i4CurrentTriggerChannelSelected = 1;


    // 在类定义中添加
    QVector<QVariant> m_showVariants;
    QVector<QVector<double>> m_voltageChs;
    QVector<void (Widget::*)(QVariant, const QString&, double, int)> m_signals;
    QStringList m_channelNames;

public:
signals:
    void RxScopeAll(int channel, const QVariant &rxData, QString GraphName, double xAxisFactor, vQCustomPlot::PLOT_TYPE_E ePlotType);
    void rangeChanged(const QCPRange &range);
    void SigCloseEntireWindow();
    void BscanDataReadyDone();
    //void CustomPlotRangeChanged(const QCPRange &rangeX,const QCPRange &rangeY);
private slots:
    //void slotActionInsert();
    // void analysisThecsvFile(QByteArray array);
    //void readPendingDatagrams();

    //void on_reset_clicked();
    void recv_slot();
    /******串口**************************/

    //void serialPortreadyread_slot();
    void on_open_clicked();
    void on_clear_clicked();
    void on_send_clicked();
    void on_close_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pBtTriggerSetting_clicked();

    void UartToWifiDataReadyWaitTimerOut();
    void ContinueTriggerTimerOut();

    void onCustomPlot1RangeChanged();
    void connectCustomPlot2();
    void on_pBtTriggerEn_clicked();

    void on_pBtCloseWindow_released();

    void on_pBtTriggerStop_clicked();

    void on_pB_FFT_Transform_clicked();
    void FFT_Transform(QVector<double> OriginalDataIn, QVector<double>* TransformedOut);
    void FilterProcessing(QVector<double> FilterOriginalDataIn, QVector<double>* FilteredDataOut);
    void vSaveWaveformData(QString FileName);
    QByteArray vReadWaveformData(void);
    void on_pB_reload_file_released();

    void on_pB_Screenshot_clicked();


    QVector<float> convertToFloatVector(const QVector<double>& doubleVec);

    void handleUdpDataReceived(const QVector<QVector<double>>& voltages, const QByteArray &DataBuffer);

    void on_pBtSaveData_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;

    //QSerialPort *serialPort;


    QCPAxisRect *axisRects1;
    QCPAxisRect *axisRects2;
    QCPAxisRect *axisRects3;
    QCPAxisRect *axisRects4;
    QCPAxisRect *axisRects5;
    QCPAxisRect *axisRects6;
    QCPAxisRect *axisRects7;
    QCPAxisRect *axisRects8;
    QCPAxisRect *axisRects9;
    QCPAxisRect *axisRects10;
    QCPAxisRect *axisRects11;
    QCPAxisRect *axisRects12;

    /*FFT变换相关参数*/
    fftw_plan mFftPlan;
    double *mFftIn;
    fftw_complex *mFftOut;
    QVector<double> mFftIndices;

    // 假设这些结果变量已改为数组形式
    QVector<double> FFT_TransformedResults[UdpData::MAX_CH_CNT];
    /* FFT变换相关参数-END*/


    /* 数字滤波相关参数*/
    /* fType:过滤器类型
     * wType:窗口类型
     * scala:阶数
     * freq:频率
     * lfreq:下限频率
     * hfreq:上限频率
     */
    int FilterType;
    int WinType;
    int Scala;
    double Freq;
    double LowerFreq;
    double UpperFreq;
    QVector<double> FilteredResults[UdpData::MAX_CH_CNT];
    /* 数字滤波相关参数-END*/

    QByteArray vec;
    QByteArray ver;

    int Zhengyuandianhao;  //震源点号
    int Jieshoudianhao;  //接收点号
    int Shuipingjvli;  //震源点号
    //void OpenTheFile(QString);
    // QStringList fileNames_csv;  //打印所有选择的文件的路径
    QString fileName;  // sg2路径

    QTimer      *UartToWifiDataReadyWaitTimer;
    QTimer      *ContinueTriggerTimer;

    enum DATA_INDEX
    {
        CH1_DATA_START_INDEX = 258,
         CH1_DATA_END_INDEX = 8449,
         CH2_DATA_START_INDEX = 8452,
         CH2_DATA_END_INDEX = 16643,
         CH3_DATA_START_INDEX = 16646,
         CH3_DATA_END_INDEX = 24837,
         CH4_DATA_START_INDEX = 24840,
         CH4_DATA_END_INDEX = 33031,
         CH5_DATA_START_INDEX = 33034,
         CH5_DATA_END_INDEX = 41225,
         CH6_DATA_START_INDEX = 41228,
         CH6_DATA_END_INDEX = 49419,
         CH7_DATA_START_INDEX = 49422,
         CH7_DATA_END_INDEX = 57613,
         CH8_DATA_START_INDEX = 57616,
         CH8_DATA_END_INDEX = 65807,
         CH9_DATA_START_INDEX = 65810,
         CH9_DATA_END_INDEX = 74001,
         CH10_DATA_START_INDEX = 74004,
         CH10_DATA_END_INDEX = 82195,
         CH11_DATA_START_INDEX = 82198,
         CH11_DATA_END_INDEX = 90389,
         CH12_DATA_START_INDEX = 90392,
         CH12_DATA_END_INDEX = 98583,
    };
    QVector<uint8_t> vUdpRxdData;

    QVariant ShowQVariant[UdpData::MAX_CH_CNT];  // 创建包含12个QVariant的数组

    double   xAxisToTimeFactor;//由采集的点数序号转换成时间值，

    QVector<QCheckBox*> m_checkBoxes;
    QVector<vQCustomPlot*> m_customPlots;

    Serial_Port::PortSettings SerialSettings;

    void handlePlotVisibility(int index, int state);
    void setAllPlotsVisible(bool visible);
    void updateLayout();

protected:
    //bool event(QEvent* ev);
    //bool eventFilter(QObject *target, QEvent *event);//事件过滤器
    //void wheelEvent(QWheelEvent *ev);
protected slots:
    void vRefreshTriggerParameter();
    void initConfig();          //初始化配置文件
    void saveConfig();          //保存配置文件
    void initSerialPort();      //初始化串口
    void initUdp();             //初始化Udp
};
#endif // WIDGET_H
