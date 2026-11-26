#include "udp_data.h"
#include <QtEndian>

UdpData::UdpData(QObject *parent) : QObject(parent),
    m_udpSocket(nullptr),
    m_targetPort(0),
    m_localPort(0),
    m_isConnected(false)
{

}

UdpData::~UdpData()
{
    if (m_udpSocket) {
        m_udpSocket->close();
        delete m_udpSocket;
    }
}

/*初始化UDP连接*/
bool UdpData::initUdpConnection(const QString &targetIp, quint16 targetPort, quint16 localPort)
{
    // 检查IP地址有效性
    QHostAddress targetAddress;
    if (!targetAddress.setAddress(targetIp)) {
        emit errorOccurred(tr("Invalid target IP address: %1").arg(targetIp));
        return false;
    }

    // 如果已经存在socket，先关闭
    if (m_udpSocket) {
        m_udpSocket->close();
        delete m_udpSocket;
        m_udpSocket = nullptr;
    }

    // 创建新的UDP socket
    m_udpSocket = new QUdpSocket(this);

    // 绑定本地端口
    if (localPort != 0) {
        if (!m_udpSocket->bind(QHostAddress::Any, localPort)) {
            emit errorOccurred(tr("Failed to bind to local port %1: %2")
                              .arg(localPort).arg(m_udpSocket->errorString()));
            m_isConnected = false;
            emit connectionStatusChanged(false);
            return false;
        }
        m_localPort = localPort;
    } else
    {
#if 0
        // 不指定端口，让系统自动分配
        if (!m_udpSocket->bind(QHostAddress::Any, 0)) {
            emit errorOccurred(tr("Failed to bind to any local port: %1")
                              .arg(m_udpSocket->errorString()));
            m_isConnected = false;
            emit connectionStatusChanged(false);
            return false;
        }
        m_localPort = m_udpSocket->localPort();
#endif
    }

    // 设置目标IP和端口
    m_targetIp = targetIp;
    m_targetPort = targetPort;

    // 连接信号槽
    //connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpData::processPendingDatagrams);

    //绑定UDP超时接收结束的定时器，防止UDP数据包一次发送不完整，
    connect(&UdpDataReadyWaitTimer, SIGNAL(timeout()), this, SLOT(UdpDataReadyWaitTimerOut()));

    // 连接信号槽
    connect(m_udpSocket,&QUdpSocket::readyRead,this,[=]()
    {
        QHostAddress srcAddress;
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(),&srcAddress);
        UdpDataRevGramBuf.append(datagram);
        //_u8UdpRevCnt +=datagram.size();
        UdpDataReadyWaitTimer.start(DATA_READY_OUT); //每次进入就重新开始计时，超时等待DATA_READY_OUT毫秒，即视为本次传输接收完成
    });

    connect(m_udpSocket, QOverload<QUdpSocket::SocketError>::of(&QUdpSocket::error),
            this, [this](QUdpSocket::SocketError error) {
        Q_UNUSED(error)
        emit errorOccurred(m_udpSocket->errorString());
        m_isConnected = false;
        emit connectionStatusChanged(false);
    });

    m_isConnected = true;
    emit connectionStatusChanged(true);

    qDebug() << "UDP connection initialized successfully";
    qDebug() << "Local IPs:" << getLocalIpAddresses();
    qDebug() << "Local port:" << m_localPort;
    qDebug() << "Target IP:" << m_targetIp;
    qDebug() << "Target port:" << m_targetPort;

    return true;
}

/*获取IP地址*/
QStringList UdpData::getLocalIpAddresses() const
{
    QStringList ipList;
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry &entry, interface.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                !entry.ip().isLoopback()) {
                ipList << entry.ip().toString();
            }
        }
    }
    return ipList;
}


bool UdpData::sendData(const QByteArray &data)
{
    if (!m_isConnected || !m_udpSocket) {
        emit errorOccurred(tr("UDP connection is not initialized"));
        return false;
    }

    qint64 bytesSent = m_udpSocket->writeDatagram(data, QHostAddress(m_targetIp), m_targetPort);
    if (bytesSent == -1) {
        emit errorOccurred(tr("Failed to send data: %1").arg(m_udpSocket->errorString()));
        return false;
    }

    return true;
}

bool UdpData::isConnected() const
{
    return m_isConnected;
}

QString UdpData::getTargetIp() const
{
    return m_targetIp;
}

quint16 UdpData::getTargetPort() const
{
    return m_targetPort;
}

quint16 UdpData::getLocalPort() const
{
    return m_localPort;
}

void UdpData::processPendingDatagrams()
{
    if (!m_udpSocket) return;

    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        qint64 bytesRead = m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if (bytesRead == -1) {
            emit errorOccurred(tr("Failed to read datagram: %1").arg(m_udpSocket->errorString()));
            continue;
        }

        // 可以在这里添加数据解析逻辑

        // 发出数据接收信号
        emit dataReceived(datagram);
    }
}

/*判断结束接收数据后进行数据解析读*/
void UdpData::UdpDataReadyWaitTimerOut()
{
    UdpDataReadyWaitTimer.stop();

    /*将接收的数据根据制定的协议映射到每个通道中*/
    UdpDataFormBufToChannels(vf8VoltageCh, UdpDataRevGramBuf);
    // 发出数据接收信号
    emit dataReceivedDone(vf8VoltageCh, UdpDataRevGramBuf);
    UdpDataRevGramBuf.clear();
}

/*将接收的数据根据制定的协议映射到每个通道中*/
void UdpData::UdpDataFormBufToChannels(QVector<QVector<double>> &channels, const QByteArray &frame)
{
    channels.clear();
    channels.resize(MAX_CH_CNT);
    if (frame.size() < WaveformFrameBytes) {
        return;
    }

    constexpr int bytesPerChannel = SamplesPerChannel * sizeof(qint16);
    for (int ch = 0; ch < MAX_CH_CNT; ++ch) {
        const char *channelData = frame.constData() + WaveformHeaderBytes
            + ch * (bytesPerChannel + ChannelGapBytes);
        auto &channel = channels[ch];
        channel.resize(SamplesPerChannel);
        for (int i = 0; i < SamplesPerChannel; ++i) {
            const qint16 value = qFromLittleEndian<qint16>(channelData + i * sizeof(qint16));
            channel[i] = (value + 32768) * 0.04882887;
        }
    }
}
