#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QComboBox>

class Serial_Port : public QObject
{
    Q_OBJECT
public:
    explicit Serial_Port(QObject *parent = nullptr);
    ~Serial_Port();

    // 串口配置
    struct PortSettings {
        QString portName;      // 串口名称
        qint32 baudRate;       // 波特率
        QSerialPort::DataBits dataBits;    // 数据位
        QSerialPort::Parity parity;        // 校验位
        QSerialPort::StopBits stopBits;    // 停止位
        QSerialPort::FlowControl flowControl; // 流控制
    };
    // 从UI控件获取串口设置
   PortSettings getSettingsFromUI(QComboBox* portCb,
                                QComboBox* baudCb,
                                QComboBox* dataCb,
                                QComboBox* stopCb,
                                QComboBox* parityCb);
   // 打开串口(带UI参数)
   bool openWithUI(QComboBox* portCb,
                  QComboBox* baudCb,
                  QComboBox* dataCb,
                  QComboBox* stopCb,
                  QComboBox* parityCb);

    // 获取可用串口列表
    QStringList getAvailablePorts();

    // 配置并打开串口
    bool openPort(const PortSettings &config);

    // 关闭串口
    void closePort();

    // 检查串口是否打开
    bool isOpen() const;

    // 清空接收和发送缓冲区
    void clear();

    // 发送数据
    void sendData(const QByteArray &data);

    // 定义回调函数类型
    using DataReceivedCallback = std::function<void(const QByteArray&)>;


    // 设置数据接收回调函数
    void setDataReceivedCallback(std::function<void(const QByteArray &)> callback);

    // 获取当前串口配置
    PortSettings getCurrentConfig() const;

signals:
    // 数据接收信号
    void dataReceived(const QByteArray &data);
    // 串口状态变化信号
    void portStateChanged(bool isOpen);

private slots:
    // 处理接收到的数据
    void handleReadyRead();

private:
    QSerialPort *m_serialPort;
    PortSettings m_currentConfig;
    std::function<void(const QByteArray &)> m_dataCallback;
    // 辅助方法
    QSerialPort::BaudRate getBaudRateFromText(const QString &baudText);
    QSerialPort::DataBits getDataBitsFromText(const QString &dataText);
    QSerialPort::StopBits getStopBitsFromText(const QString &stopText);
    QSerialPort::Parity getParityFromText(const QString &parityText);
};

#endif // SERIAL_PORT_H
