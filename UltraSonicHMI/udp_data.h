#ifndef UDPDATA_H
#define UDPDATA_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>

// 常量定义
constexpr int DATA_READY_OUT = 300;  // 数据准备超时时间(毫秒)
class UdpData : public QObject
{
    Q_OBJECT

public:
    explicit UdpData(QObject *parent = nullptr);
    ~UdpData();

    // 初始化UDP连接
    bool initUdpConnection(const QString &targetIp, quint16 targetPort, quint16 localPort = 0);

    // 获取本机IP地址列表
    QStringList getLocalIpAddresses() const;

    // 发送数据
    bool sendData(const QByteArray &data);

    // 获取当前连接状态
    bool isConnected() const;

    // 获取目标IP和端口
    QString getTargetIp() const;
    quint16 getTargetPort() const;

    // 获取本地绑定端口
    quint16 getLocalPort() const;
    QByteArray  UdpDataRevGramBuf;
    QTimer      UdpDataReadyWaitTimer;

    enum Channel {
        CH1, CH2, CH3, CH4, CH5, CH6, CH7,
        CH8, CH9, CH10, CH11, CH12, MAX_CH_CNT
    };

    //QVector<double> vf8VoltageCh[MAX_CH_CNT];  // 创建包含13个QVector的数组
    QVector<QVector<double>> vf8VoltageCh;
    QByteArray DataBuffer;
signals:
    // 数据接收信号
    void dataReceived(const QByteArray &data);
    // 连接状态变化信号
    void connectionStatusChanged(bool connected);
    // 错误信号
    void errorOccurred(const QString &errorMsg);
    // 修改信号声明
    void dataReceivedDone(const QVector<QVector<double>> &voltages, QByteArray  DataBuf);

private slots:
    // 处理接收到的数据
    void processPendingDatagrams();
public slots:
    void UdpDataReadyWaitTimerOut();
    void UdpDataFormBufToChannels(QVector<QVector<double>> &vf8VoltageCh, QByteArray  DataBuf);

private:
    QUdpSocket *m_udpSocket;
    QString m_targetIp;
    quint16 m_targetPort;
    quint16 m_localPort;
    bool m_isConnected;

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
};

#endif // UDPDATA_H
