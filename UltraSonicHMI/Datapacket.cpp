/****************************************************************************
**
** 文件名: Datapacket.c
** 作者: Zhou LiGui
** 创建日期: 2024年4月11日
** 功能描述: 对超声波主机和PC上位机之前的通信规约进行UDP数据打包
**
** 详细描述: 对超声波主机和PC上位机之前的通信规约进行UDP数据打包
**
** 版权所有 (C): 2024 GZU
**
** 注意事项:
**
****************************************************************************/


#include "appconfig.h"
#include "qthelper.h"
#include "Datapacket.h"
#include "SonicLaunchCfg.h"


void DataPacket::initDataPacket()
{
    uint32_t u4Temp = 0x00;

    // 设置byteArray的大小以容纳所有数据
    QbDataPacket.append(256,char(0));
    QbDataPacket[0] = 0xAA;
    QbDataPacket[1] = 0x00;
    QbDataPacket[2] = 0x00;
    QbDataPacket[3] = 0xBB;
    QbDataPacket[4] = 0x00;
    QbDataPacket[5] = 0x00;
    QbDataPacket[6] = 0x00;
    QbDataPacket[7] = 0xFC;
    QbDataPacket[8] = 0x01;
    QbDataPacket[9] = 0x10;
    QbDataPacket[10] = 0x10;
    QbDataPacket[11] = 0x01;
    QbDataPacket[12] = 0x00;
    QbDataPacket[13] = 0xF2;
    QbDataPacket[14] = 0x00;

    switch(AppConfig::u2TriggerMode)
    {
    case STOP_TRI:
        QbDataPacket[15] = 0x00;
        this->strTriggerMode = "无触发";
        break;
    case ONCE_TRI:
        QbDataPacket[15] = 0x01;
        this->strTriggerMode = "单次触发";
        break;
    case CONTINUE_TRI:
        QbDataPacket[15] = 0x01; //"连续触发"由上位机控制这里修改置为0x01 不是0x02
        this->strTriggerMode = "连续触发";
        break;
    case ONCE_EXT_TRI:
        QbDataPacket[15] = 0x03;
        this->strTriggerMode = "单次外部触发";
        break;
    case CONTINUE_EXT_TRI:
        QbDataPacket[15] = 0x04;
        this->strTriggerMode = "连续外部触发";
        break;
    case SIGNAL_THRESHOLD_TRI:
        QbDataPacket[15] = 0x05;
        this->strTriggerMode = "信号阈值触发";
        break;
    case BSACN_SEQUENCE_TRI:
        QbDataPacket[15] = 0x06;
        this->strTriggerMode = "B扫成像序列触发";
        break;
    default:
        QbDataPacket[15] = 0x00;
        this->strTriggerMode = "触发错误";
        break;
    }
    int startIdx = 16;
    int i = 0;
    QByteArray hexData;

    if(AppConfig::blSingleChannelSettingEn == true) //每个通道单独配置。
    {
        this->vOneChannelSelectedDataPacket(1, AppConfig::u4DecimateSpanCH01, AppConfig::u4SampleCountCH01,\
                                            AppConfig::u2PulseWidth01, AppConfig::u2VoltageValue01, AppConfig::u2HardwareAmplifyFactorCH01);

        this->vOneChannelSelectedDataPacket(2, AppConfig::u4DecimateSpanCH02, AppConfig::u4SampleCountCH02,\
                                            AppConfig::u2PulseWidth02, AppConfig::u2VoltageValue02, AppConfig::u2HardwareAmplifyFactorCH02);

        this->vOneChannelSelectedDataPacket(3, AppConfig::u4DecimateSpanCH03, AppConfig::u4SampleCountCH03,\
                                            AppConfig::u2PulseWidth03, AppConfig::u2VoltageValue03, AppConfig::u2HardwareAmplifyFactorCH03);

        this->vOneChannelSelectedDataPacket(4, AppConfig::u4DecimateSpanCH04, AppConfig::u4SampleCountCH04,\
                                            AppConfig::u2PulseWidth04, AppConfig::u2VoltageValue04, AppConfig::u2HardwareAmplifyFactorCH04);

        this->vOneChannelSelectedDataPacket(5, AppConfig::u4DecimateSpanCH05, AppConfig::u4SampleCountCH05,\
                                            AppConfig::u2PulseWidth05, AppConfig::u2VoltageValue05, AppConfig::u2HardwareAmplifyFactorCH05);

        this->vOneChannelSelectedDataPacket(6, AppConfig::u4DecimateSpanCH06, AppConfig::u4SampleCountCH06,\
                                            AppConfig::u2PulseWidth06, AppConfig::u2VoltageValue06, AppConfig::u2HardwareAmplifyFactorCH06);

        this->vOneChannelSelectedDataPacket(7, AppConfig::u4DecimateSpanCH07, AppConfig::u4SampleCountCH07,\
                                            AppConfig::u2PulseWidth07, AppConfig::u2VoltageValue07, AppConfig::u2HardwareAmplifyFactorCH07);

        this->vOneChannelSelectedDataPacket(8, AppConfig::u4DecimateSpanCH08, AppConfig::u4SampleCountCH08,\
                                            AppConfig::u2PulseWidth08, AppConfig::u2VoltageValue08, AppConfig::u2HardwareAmplifyFactorCH08);

        this->vOneChannelSelectedDataPacket(9, AppConfig::u4DecimateSpanCH09, AppConfig::u4SampleCountCH09,\
                                            AppConfig::u2PulseWidth09, AppConfig::u2VoltageValue09, AppConfig::u2HardwareAmplifyFactorCH09);

        this->vOneChannelSelectedDataPacket(10, AppConfig::u4DecimateSpanCH10, AppConfig::u4SampleCountCH10,\
                                             AppConfig::u2PulseWidth10, AppConfig::u2VoltageValue10, AppConfig::u2HardwareAmplifyFactorCH10);

        this->vOneChannelSelectedDataPacket(11, AppConfig::u4DecimateSpanCH11, AppConfig::u4SampleCountCH11,\
                                            AppConfig::u2PulseWidth11, AppConfig::u2VoltageValue11, AppConfig::u2HardwareAmplifyFactorCH11);

        this->vOneChannelSelectedDataPacket(12, AppConfig::u4DecimateSpanCH12, AppConfig::u4SampleCountCH12,\
                                            AppConfig::u2PulseWidth12, AppConfig::u2VoltageValue12, AppConfig::u2HardwareAmplifyFactorCH12);

        switch(AppConfig::u2TriggerChannelSelected)
        {
        case 1:
            this->vOneChannelSelectedDataPacket(1, AppConfig::u4DecimateSpanCH01, AppConfig::u4SampleCountCH01,\
                                          AppConfig::u2PulseWidth01, AppConfig::u2VoltageValue01, AppConfig::u2HardwareAmplifyFactorCH01);
            break;
        case 2:
            this->vOneChannelSelectedDataPacket(2, AppConfig::u4DecimateSpanCH02, AppConfig::u4SampleCountCH02,\
                                          AppConfig::u2PulseWidth02, AppConfig::u2VoltageValue02, AppConfig::u2HardwareAmplifyFactorCH02);
            break;
        case 3:
            this->vOneChannelSelectedDataPacket(3, AppConfig::u4DecimateSpanCH03, AppConfig::u4SampleCountCH03,\
                                          AppConfig::u2PulseWidth03, AppConfig::u2VoltageValue03, AppConfig::u2HardwareAmplifyFactorCH03);
            break;
        case 4:
            this->vOneChannelSelectedDataPacket(4, AppConfig::u4DecimateSpanCH04, AppConfig::u4SampleCountCH04,\
                                          AppConfig::u2PulseWidth04, AppConfig::u2VoltageValue04, AppConfig::u2HardwareAmplifyFactorCH04);
            break;
        case 5:
            this->vOneChannelSelectedDataPacket(5, AppConfig::u4DecimateSpanCH05, AppConfig::u4SampleCountCH05,\
                                          AppConfig::u2PulseWidth05, AppConfig::u2VoltageValue05, AppConfig::u2HardwareAmplifyFactorCH05);
            break;
        case 6:
            this->vOneChannelSelectedDataPacket(6, AppConfig::u4DecimateSpanCH06, AppConfig::u4SampleCountCH06,\
                                          AppConfig::u2PulseWidth06, AppConfig::u2VoltageValue06, AppConfig::u2HardwareAmplifyFactorCH06);
            break;
        case 7:
            this->vOneChannelSelectedDataPacket(7, AppConfig::u4DecimateSpanCH07, AppConfig::u4SampleCountCH07,\
                                          AppConfig::u2PulseWidth07, AppConfig::u2VoltageValue07, AppConfig::u2HardwareAmplifyFactorCH07);
            break;
        case 8:
            this->vOneChannelSelectedDataPacket(8, AppConfig::u4DecimateSpanCH08, AppConfig::u4SampleCountCH08,\
                                          AppConfig::u2PulseWidth08, AppConfig::u2VoltageValue08, AppConfig::u2HardwareAmplifyFactorCH08);
            break;
        case 9:
            this->vOneChannelSelectedDataPacket(9, AppConfig::u4DecimateSpanCH09, AppConfig::u4SampleCountCH09,\
                                          AppConfig::u2PulseWidth09, AppConfig::u2VoltageValue09, AppConfig::u2HardwareAmplifyFactorCH09);
            break;
        case 10:
            this->vOneChannelSelectedDataPacket(10, AppConfig::u4DecimateSpanCH10, AppConfig::u4SampleCountCH10,\
                                          AppConfig::u2PulseWidth10, AppConfig::u2VoltageValue10, AppConfig::u2HardwareAmplifyFactorCH10);
            break;
        case 11:
            this->vOneChannelSelectedDataPacket(11, AppConfig::u4DecimateSpanCH11, AppConfig::u4SampleCountCH11,\
                                          AppConfig::u2PulseWidth11, AppConfig::u2VoltageValue11, AppConfig::u2HardwareAmplifyFactorCH11);
            break;
        case 12:
            this->vOneChannelSelectedDataPacket(12, AppConfig::u4DecimateSpanCH12, AppConfig::u4SampleCountCH12,\
                                          AppConfig::u2PulseWidth12, AppConfig::u2VoltageValue12, AppConfig::u2HardwareAmplifyFactorCH12);
            break;
        default:
            break;
        }
    }
    else //所有配置使用公共通道0的配置 1~12为各自的通道配置
    {

        this->vOneChannelSelectedDataPacket(1, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH01);
        this->vOneChannelSelectedDataPacket(2, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH02);
        this->vOneChannelSelectedDataPacket(3, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH03);
        this->vOneChannelSelectedDataPacket(4, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH04);
        this->vOneChannelSelectedDataPacket(5, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH05);
        this->vOneChannelSelectedDataPacket(6, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH06);
        this->vOneChannelSelectedDataPacket(7, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH07);
        this->vOneChannelSelectedDataPacket(8, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH08);
        this->vOneChannelSelectedDataPacket(9, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH09);
        this->vOneChannelSelectedDataPacket(10, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH10);
        this->vOneChannelSelectedDataPacket(11, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH11);
        this->vOneChannelSelectedDataPacket(12, AppConfig::u4DecimateSpanCH00, AppConfig::u4SampleCountCH00,\
                                            AppConfig::u2PulseWidth00, AppConfig::u2VoltageValue00, AppConfig::u2HardwareAmplifyFactorCH12);

    }
    startIdx = 184;
    u4Temp = AppConfig::u4PulsePeriod;
    hexData.clear();
    hexData.append((u4Temp >> 24) & 0xFF); // 最高字节
    hexData.append((u4Temp >> 16) & 0xFF);
    hexData.append((u4Temp >> 8) & 0xFF);
    hexData.append(u4Temp & 0xFF); // 最低字节
    for (i = 0; i < 4; ++i)
    {
        QbDataPacket[startIdx + i] = hexData.at(i);
    }
    this->strPulsePeriod = QString::number(AppConfig::u4PulsePeriod);

    //发射通道选择
    QbDataPacket[188] = 0x00;
    QbDataPacket[189] = (uint8_t)(AppConfig::u2TriggerChannelSelected);
    this->strTriggerChannelSelected = QString::number((AppConfig::u2TriggerChannelSelected));

    //ADC采集延时
    startIdx = 190;
    u4Temp = AppConfig::u4SamplingOffset;
    hexData.clear();
    hexData.append((u4Temp >> 24) & 0xFF); // 最高字节
    hexData.append((u4Temp >> 16) & 0xFF);
    hexData.append((u4Temp >> 8) & 0xFF);
    hexData.append(u4Temp & 0xFF); // 最低字节
    for (i = 0; i < 4; ++i)
    {
        QbDataPacket[startIdx + i] = hexData.at(i);
    }
    this->strSamplingOffset = QString::number(AppConfig::u4SamplingOffset);

}

void DataPacket::vOneChannelSelectedDataPacket(uint16_t u2ChannelSelectedCHx,uint32_t u4DecimateSpanCHx,uint32_t u4SampleCountCHx,\
                                   uint16_t u2PulseWidthChx,uint16_t u2VoltageValueChx,uint16_t u2HardwareAmplifyFactorCHx)
{
    uint32_t u4Temp = 0x00;
    uint16_t u2Temp = 0x00;
    int i = 0;
    QByteArray hexData;

    //采样抽取间隔
    int startIdx = 16 + ((u2ChannelSelectedCHx-1) * 4);
    u4Temp = u4DecimateSpanCHx;
    hexData.clear();
    hexData.append((u4Temp >> 24) & 0xFF); // 最高字节
    hexData.append((u4Temp >> 16) & 0xFF);
    hexData.append((u4Temp >> 8) & 0xFF);
    hexData.append(u4Temp & 0xFF); // 最低字节
    for (i = 0; i < 4; ++i)
    {
        QbDataPacket[startIdx + i] = hexData.at(i);
    }

    this->strDecimateSpan = QString::number(u4DecimateSpanCHx*40);

    //采样点数
    startIdx = 64+ ((u2ChannelSelectedCHx-1) * 4);
    switch (u4SampleCountCHx)
    {
    case 512:
        QbDataPacket[startIdx+0] = 0x00;
        QbDataPacket[startIdx+1] = 0x00;
        QbDataPacket[startIdx+2] = 0x02;
        QbDataPacket[startIdx+3] = 0x00;
        this->strSampleCount = "512";
        break;
    case 1024:
        QbDataPacket[startIdx+0] = 0x00;
        QbDataPacket[startIdx+1] = 0x00;
        QbDataPacket[startIdx+2] = 0x04;
        QbDataPacket[startIdx+3] = 0x00;
        this->strSampleCount = "1024";
        break;
    case 2048:
        QbDataPacket[startIdx+0] = 0x00;
        QbDataPacket[startIdx+1] = 0x00;
        QbDataPacket[startIdx+2] = 0x08;
        QbDataPacket[startIdx+3] = 0x00;
        this->strSampleCount = "2048";
        break;
    case 4096:
        QbDataPacket[startIdx+0] = 0x00;
        QbDataPacket[startIdx+1] = 0x00;
        QbDataPacket[startIdx+2] = 0x10;
        QbDataPacket[startIdx+3] = 0x00;
        this->strSampleCount = "4096";
        break;
    default:
        QbDataPacket[startIdx+0] = 0x00;
        QbDataPacket[startIdx+1] = 0x00;
        QbDataPacket[startIdx+2] = 0x00;
        QbDataPacket[startIdx+3] = 0x00;
        this->strSampleCount = "xxxx";
        break;
    }

    //脉冲宽度
    startIdx = 112+ ((u2ChannelSelectedCHx-1) * 2);
    u2Temp = u2PulseWidthChx;
    hexData.clear();
    hexData.append((u2Temp >> 8) & 0xFF); // 高字节
    hexData.append(u2Temp & 0xFF);        // 低字节
    for (i = 0; i < hexData.size(); ++i)
    {
        QbDataPacket[startIdx + i] = hexData.at(i);
    }
    this->strPulseWidth = QString::number(u2PulseWidthChx);

    //发射电压
    startIdx = 136+ ((u2ChannelSelectedCHx-1) * 2);
    switch (u2VoltageValueChx)
    {
    case 300:
        QbDataPacket[startIdx+0] = 0x01;
        QbDataPacket[startIdx+1] = 0x2C;
        this->strVoltageValue = "300V";
        break;
    case 600:
        QbDataPacket[startIdx+0] = 0x02;
        QbDataPacket[startIdx+1] = 0x58;
        this->strVoltageValue = "600V";
        break;
    case 800:
        QbDataPacket[startIdx+0] = 0x03;
        QbDataPacket[startIdx+1] = 0x20;
        this->strVoltageValue = "800V";
        break;
    case 1000:
        QbDataPacket[startIdx+0] = 0x03;
        QbDataPacket[startIdx+1] = 0xE8;
        this->strVoltageValue = "1000V";
        break;
    default:
        QbDataPacket[startIdx+0] = static_cast<uint8_t>(((u2VoltageValueChx >> 8) & 0xFF)); // 高字节;
        QbDataPacket[startIdx+1] = static_cast<uint8_t>(u2VoltageValueChx & 0xFF);
        this->strVoltageValue = QString::number(u2VoltageValueChx) + "V";
        break;
    }
    //硬件增益
    startIdx = 160 + ((u2ChannelSelectedCHx-1) * 2);
    u2Temp = u2HardwareAmplifyFactorCHx;
    hexData.clear();
    hexData.append((u2Temp >> 8) & 0xFF); // 高字节
    hexData.append(u2Temp & 0xFF);        // 低字节
    for (i = 0; i < hexData.size(); ++i)
    {
        QbDataPacket[startIdx + i] = hexData.at(i);
    }
    this->strHardwareAmplifyFactor = QString::number(u2HardwareAmplifyFactorCHx);

}
