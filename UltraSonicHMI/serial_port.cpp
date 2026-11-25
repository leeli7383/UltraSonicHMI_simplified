#include "serial_port.h"

Serial_Port::Serial_Port(QObject *parent) : QObject(parent)
{
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &Serial_Port::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if (error == QSerialPort::ResourceError) {
            QMessageBox::critical(nullptr, tr("Error"), m_serialPort->errorString());
            closePort();
        }
    });
}

Serial_Port::~Serial_Port()
{
    closePort();
    delete m_serialPort;
}

QStringList Serial_Port::getAvailablePorts()
{
    QStringList ports;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ports << info.portName();
    }
    return ports;
}

bool Serial_Port::openPort(const PortSettings &config)
{
    if (m_serialPort->isOpen()) {
        closePort();
    }

    m_serialPort->setPortName(config.portName);
    m_serialPort->setBaudRate(config.baudRate);
    m_serialPort->setDataBits(config.dataBits);
    m_serialPort->setParity(config.parity);
    m_serialPort->setStopBits(config.stopBits);
    m_serialPort->setFlowControl(config.flowControl);

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(nullptr, tr("Error"),
                             tr("Failed to open port %1: %2")
                             .arg(config.portName)
                             .arg(m_serialPort->errorString()));
        return false;
    }

    m_currentConfig = config;
    emit portStateChanged(true);
    return true;
}

void Serial_Port::closePort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        emit portStateChanged(false);
    }
}

bool Serial_Port::isOpen() const
{
    return m_serialPort->isOpen();
}

void Serial_Port::clear()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->clear();
    }
}

void Serial_Port::sendData(const QByteArray &data)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->write(data);
    } else {
        qWarning() << "Serial port is not open, cannot send data";
    }
}

void Serial_Port::setDataReceivedCallback(std::function<void (const QByteArray &)> callback)
{
    m_dataCallback = callback;
}

Serial_Port::PortSettings Serial_Port::getCurrentConfig() const
{
    return m_currentConfig;
}

void Serial_Port::handleReadyRead()
{
    QByteArray data = m_serialPort->readAll();

    // 处理剩余数据
    while (m_serialPort->waitForReadyRead(10)) {
        data += m_serialPort->readAll();
    }

    // 发射信号
    emit dataReceived(data);

    // 如果有回调函数，调用回调
    if (m_dataCallback) {
        m_dataCallback(data);
    }
}

Serial_Port::PortSettings  Serial_Port::getSettingsFromUI(QComboBox* portCb,
                                                       QComboBox* baudCb,
                                                       QComboBox* dataCb,
                                                       QComboBox* stopCb,
                                                       QComboBox* parityCb)
{
    PortSettings  settings;

    settings.portName = portCb->currentText();
    settings.baudRate = getBaudRateFromText(baudCb->currentText());
    settings.dataBits = getDataBitsFromText(dataCb->currentText());
    settings.stopBits = getStopBitsFromText(stopCb->currentText());
    settings.parity = getParityFromText(parityCb->currentText());

    return settings;
}

bool Serial_Port::openWithUI(QComboBox* portCb,
                           QComboBox* baudCb,
                           QComboBox* dataCb,
                           QComboBox* stopCb,
                           QComboBox* parityCb)
{
    PortSettings  settings = getSettingsFromUI(portCb, baudCb, dataCb, stopCb, parityCb);
    return openPort(settings);
}

// 辅助方法实现
QSerialPort::BaudRate Serial_Port::getBaudRateFromText(const QString &baudText)
{
    static const QMap<QString, QSerialPort::BaudRate> baudMap = {
        {"1200", QSerialPort::Baud1200},
        {"2400", QSerialPort::Baud2400},
        {"4800", QSerialPort::Baud4800},
        {"9600", QSerialPort::Baud9600},
        {"19200", QSerialPort::Baud19200},
        {"38400", QSerialPort::Baud38400},
        {"57600", QSerialPort::Baud57600},
        {"115200", QSerialPort::Baud115200}
    };
    return baudMap.value(baudText, QSerialPort::UnknownBaud);
}

QSerialPort::DataBits Serial_Port::getDataBitsFromText(const QString &dataText)
{
    static const QMap<QString, QSerialPort::DataBits> dataMap = {
        {"5", QSerialPort::Data5},
        {"6", QSerialPort::Data6},
        {"7", QSerialPort::Data7},
        {"8", QSerialPort::Data8}
    };
    return dataMap.value(dataText, QSerialPort::UnknownDataBits);
}

QSerialPort::StopBits Serial_Port::getStopBitsFromText(const QString &stopText)
{
    static const QMap<QString, QSerialPort::StopBits> stopMap = {
        {"1", QSerialPort::OneStop},
        {"1.5", QSerialPort::OneAndHalfStop},
        {"2", QSerialPort::TwoStop}
    };
    return stopMap.value(stopText, QSerialPort::UnknownStopBits);
}

QSerialPort::Parity Serial_Port::getParityFromText(const QString &parityText)
{
    static const QMap<QString, QSerialPort::Parity> parityMap = {
        {"None", QSerialPort::NoParity},
        {"Even", QSerialPort::EvenParity},
        {"Odd", QSerialPort::OddParity},
        {"Space", QSerialPort::SpaceParity},
        {"Mark", QSerialPort::MarkParity}
    };
    return parityMap.value(parityText, QSerialPort::UnknownParity);
}
