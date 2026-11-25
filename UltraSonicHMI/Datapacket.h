#ifndef DATA_PACKET_H
#define DATA_PACKET_H

class DataPacket
{
public:
    QByteArray QbDataPacket;


    QString  strTriggerMode;
    QString  strDecimateSpan;
    QString  strSampleCount;
    QString  strVoltageValue;
    QString  strTriggerChannelSelected;
    QString  strPulsePeriod;
    QString  strHardwareAmplifyFactor;
    QString  strPulseWidth;
    QString  strSamplingOffset;

    void initDataPacket();
    void vOneChannelSelectedDataPacket(uint16_t u2ChannelSelectedCHx,uint32_t u4DecimateSpanCHx,uint32_t u4SampleCountCHx,\
                                       uint16_t u2PulseWidthChx,uint16_t u2VoltageValueChx,uint16_t u2HardwareAmplifyFactorCHx);
};

#endif // DATA_PACKET_H
