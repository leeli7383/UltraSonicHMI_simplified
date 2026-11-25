/****************************************************************************
**
** 文件名: appconfig.c
** 作者: Zhou LiGui
** 创建日期: 2025年4月11日
** 功能描述: 上位机的一些配置进行qsettings类的.ini文件进行保存和初始化调取
**
** 详细描述:
**
** 版权所有 (C): 2024 GZU
**
** 注意事项:
**
****************************************************************************/

#include "appconfig.h"
#include "qthelper.h"
#include "qsettings.h"
QString AppConfig::ConfigFile = "config.ini";
QString AppConfig::SendFileName = "send.txt";
QString AppConfig::DeviceFileName = "device.txt";

QString AppConfig::PortName = "COM1";
int AppConfig::BaudRate = 9600;
int AppConfig::DataBit = 8;
QString AppConfig::Parity = QString::fromUtf8("无");
double AppConfig::StopBit = 1;

bool AppConfig::HexSend = false;
bool AppConfig::HexReceive = false;
bool AppConfig::Debug = false;
bool AppConfig::AutoClear = false;

bool AppConfig::AutoSend = false;
int AppConfig::SendInterval = 1000;
bool AppConfig::AutoSave = false;
int AppConfig::SaveInterval = 5000;

QString AppConfig::Mode = "Udp_Client";
QString AppConfig::SlaveDeviceIP = "192.168.1.178";
int AppConfig::SlaveDevicePort = 502;
int AppConfig::SleepTime = 100;
bool AppConfig::AutoConnect = false;


bool AppConfig::Waveform1DisplayEn = true;
bool AppConfig::Waveform2DisplayEn = true;
bool AppConfig::Waveform3DisplayEn = true;
bool AppConfig::Waveform4DisplayEn = true;
bool AppConfig::Waveform5DisplayEn = true;
bool AppConfig::Waveform6DisplayEn = true;
bool AppConfig::Waveform7DisplayEn = true;
bool AppConfig::Waveform8DisplayEn = true;
bool AppConfig::Waveform9DisplayEn = true;
bool AppConfig::Waveform10DisplayEn = true;
bool AppConfig::Waveform11DisplayEn = true;
bool AppConfig::Waveform12DisplayEn = true;

bool AppConfig::GlobalZoom = true;
//抽取间隔
quint32 AppConfig::u4DecimateSpanCH00 = 50; //uint ns
quint32 AppConfig::u4DecimateSpanCH01 = 50; //uint ns
quint32 AppConfig::u4DecimateSpanCH02 = 50;
quint32 AppConfig::u4DecimateSpanCH03 = 50;
quint32 AppConfig::u4DecimateSpanCH04 = 50;
quint32 AppConfig::u4DecimateSpanCH05 = 50;
quint32 AppConfig::u4DecimateSpanCH06 = 50;
quint32 AppConfig::u4DecimateSpanCH07 = 50;
quint32 AppConfig::u4DecimateSpanCH08 = 50;
quint32 AppConfig::u4DecimateSpanCH09 = 50;
quint32 AppConfig::u4DecimateSpanCH10 = 50;
quint32 AppConfig::u4DecimateSpanCH11 = 50;
quint32 AppConfig::u4DecimateSpanCH12 = 50;

//采样点数
quint32 AppConfig::u4SampleCountCH00 = 4096;
quint32 AppConfig::u4SampleCountCH01 = 4096;
quint32 AppConfig::u4SampleCountCH02 = 4096;
quint32 AppConfig::u4SampleCountCH03 = 4096;
quint32 AppConfig::u4SampleCountCH04 = 4096;
quint32 AppConfig::u4SampleCountCH05 = 4096;
quint32 AppConfig::u4SampleCountCH06 = 4096;
quint32 AppConfig::u4SampleCountCH07 = 4096;
quint32 AppConfig::u4SampleCountCH08 = 4096;
quint32 AppConfig::u4SampleCountCH09 = 4096;
quint32 AppConfig::u4SampleCountCH10 = 4096;
quint32 AppConfig::u4SampleCountCH11 = 4096;
quint32 AppConfig::u4SampleCountCH12 = 4096;
//脉冲宽度
quint16 AppConfig::u2PulseWidth00 = 1; //uint us
quint16 AppConfig::u2PulseWidth01 = 1; //uint us
quint16 AppConfig::u2PulseWidth02 = 1;
quint16 AppConfig::u2PulseWidth03 = 1;
quint16 AppConfig::u2PulseWidth04 = 1;
quint16 AppConfig::u2PulseWidth05 = 1;
quint16 AppConfig::u2PulseWidth06 = 1;
quint16 AppConfig::u2PulseWidth07 = 1;
quint16 AppConfig::u2PulseWidth08 = 1;
quint16 AppConfig::u2PulseWidth09 = 1;
quint16 AppConfig::u2PulseWidth10 = 1;
quint16 AppConfig::u2PulseWidth11 = 1;
quint16 AppConfig::u2PulseWidth12 = 1;
//发射电压
quint16 AppConfig::u2VoltageValue00 = 300;
quint16 AppConfig::u2VoltageValue01 = 300;
quint16 AppConfig::u2VoltageValue02 = 300;
quint16 AppConfig::u2VoltageValue03 = 300;
quint16 AppConfig::u2VoltageValue04 = 300;
quint16 AppConfig::u2VoltageValue05 = 300;
quint16 AppConfig::u2VoltageValue06 = 300;
quint16 AppConfig::u2VoltageValue07 = 300;
quint16 AppConfig::u2VoltageValue08 = 300;
quint16 AppConfig::u2VoltageValue09 = 300;
quint16 AppConfig::u2VoltageValue10 = 300;
quint16 AppConfig::u2VoltageValue11 = 300;
quint16 AppConfig::u2VoltageValue12 = 300;
//硬件放大增益
quint16 AppConfig::u2HardwareAmplifyFactorCH00 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH01 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH02 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH03 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH04 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH05 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH06 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH07 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH08 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH09 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH10 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH11 = 1;
quint16 AppConfig::u2HardwareAmplifyFactorCH12 = 1;
//低通滤波
quint32 AppConfig::u4LowPassFilteringCH00 = 0;
quint32 AppConfig::u4LowPassFilteringCH01 = 0;
quint32 AppConfig::u4LowPassFilteringCH02 = 0;
quint32 AppConfig::u4LowPassFilteringCH03 = 0;
quint32 AppConfig::u4LowPassFilteringCH04 = 0;
quint32 AppConfig::u4LowPassFilteringCH05 = 0;
quint32 AppConfig::u4LowPassFilteringCH06 = 0;
quint32 AppConfig::u4LowPassFilteringCH07 = 0;
quint32 AppConfig::u4LowPassFilteringCH08 = 0;
quint32 AppConfig::u4LowPassFilteringCH09 = 0;
quint32 AppConfig::u4LowPassFilteringCH10 = 0;
quint32 AppConfig::u4LowPassFilteringCH11 = 0;
quint32 AppConfig::u4LowPassFilteringCH12 = 0;
//高通滤波
quint32 AppConfig::u4HighPassFilteringCH00 = 0;
quint32 AppConfig::u4HighPassFilteringCH01 = 0;
quint32 AppConfig::u4HighPassFilteringCH02 = 0;
quint32 AppConfig::u4HighPassFilteringCH03 = 0;
quint32 AppConfig::u4HighPassFilteringCH04 = 0;
quint32 AppConfig::u4HighPassFilteringCH05 = 0;
quint32 AppConfig::u4HighPassFilteringCH06 = 0;
quint32 AppConfig::u4HighPassFilteringCH07 = 0;
quint32 AppConfig::u4HighPassFilteringCH08 = 0;
quint32 AppConfig::u4HighPassFilteringCH09 = 0;
quint32 AppConfig::u4HighPassFilteringCH10 = 0;
quint32 AppConfig::u4HighPassFilteringCH11 = 0;
quint32 AppConfig::u4HighPassFilteringCH12 = 0;


//脉冲周期
quint32 AppConfig::u4PulsePeriod = 1000;  //单位为ms，默认1s

//零声时修正
quint32 AppConfig::u4ZeroModify = 0;     //单位为ns，默认0

//采样点起始时间偏移值
quint32 AppConfig::u4SamplingOffset = 0;  //单位为0s，默认0

//触发通道选择
quint16 AppConfig::u2TriggerChannelSelected = 0x01;
//触发模式选择
quint16 AppConfig::u2TriggerMode = 0x01;

QString AppConfig::DestIP = "192.168.1.178";     //目标板卡设备IP
uint32_t AppConfig::u4DestPort = 502;            //服务器端口

QString AppConfig::DestUartToWifiIP = "192.168.1.179";
uint32_t AppConfig::u4DestUartToWifi = 8899;

int AppConfig::ListenPort = 502;

bool AppConfig::blSingleChannelSettingEn = false;

void AppConfig::readConfig()
{
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);

    set.beginGroup("ComConfig");
    AppConfig::PortName = set.value("PortName", AppConfig::PortName).toString();
    AppConfig::BaudRate = set.value("BaudRate", AppConfig::BaudRate).toInt();
    AppConfig::DataBit = set.value("DataBit", AppConfig::DataBit).toInt();
    AppConfig::Parity = set.value("Parity", AppConfig::Parity).toString();
    AppConfig::StopBit = set.value("StopBit", AppConfig::StopBit).toInt();

    AppConfig::HexSend = set.value("HexSend", AppConfig::HexSend).toBool();
    AppConfig::HexReceive = set.value("HexReceive", AppConfig::HexReceive).toBool();
    AppConfig::Debug = set.value("Debug", AppConfig::Debug).toBool();
    AppConfig::AutoClear = set.value("AutoClear", AppConfig::AutoClear).toBool();

    AppConfig::AutoSend = set.value("AutoSend", AppConfig::AutoSend).toBool();
    AppConfig::SendInterval = set.value("SendInterval", AppConfig::SendInterval).toInt();
    AppConfig::AutoSave = set.value("AutoSave", AppConfig::AutoSave).toBool();
    AppConfig::SaveInterval = set.value("SaveInterval", AppConfig::SaveInterval).toInt();
    set.endGroup();


    //Udp网络配置组
    set.beginGroup("NetUdpConfig");
    AppConfig::Mode = set.value("Mode", AppConfig::Mode).toString();
    AppConfig::SlaveDeviceIP = set.value("SlaveDeviceIP", AppConfig::SlaveDeviceIP).toString();
    AppConfig::SlaveDevicePort = set.value("SlaveDevicePort", AppConfig::SlaveDevicePort).toInt();
    AppConfig::SleepTime = set.value("SleepTime", AppConfig::SleepTime).toInt();
    AppConfig::AutoConnect = set.value("AutoConnect", AppConfig::AutoConnect).toBool();
    //udp
    AppConfig::ListenPort = set.value("ListenPort", AppConfig::ListenPort).toInt();
    AppConfig::DestIP = set.value("DestIP", AppConfig::DestIP).toString();
    //触发模式选择
    AppConfig::u4DestPort = set.value("u4DestPort", AppConfig::u4DestPort).toUInt();
    set.endGroup();


    //波形显示配置组
    set.beginGroup("WigetWaveformConfig");
    AppConfig::Waveform1DisplayEn  = set.value("Waveform1DisplayEn", AppConfig::Waveform1DisplayEn ).toBool();
    AppConfig::Waveform2DisplayEn  = set.value("Waveform2DisplayEn", AppConfig::Waveform2DisplayEn ).toBool();
    AppConfig::Waveform3DisplayEn  = set.value("Waveform3DisplayEn", AppConfig::Waveform3DisplayEn ).toBool();
    AppConfig::Waveform4DisplayEn  = set.value("Waveform4DisplayEn", AppConfig::Waveform4DisplayEn ).toBool();
    AppConfig::Waveform5DisplayEn  = set.value("Waveform5DisplayEn", AppConfig::Waveform5DisplayEn ).toBool();
    AppConfig::Waveform6DisplayEn  = set.value("Waveform6DisplayEn", AppConfig::Waveform6DisplayEn ).toBool();
    AppConfig::Waveform7DisplayEn  = set.value("Waveform7DisplayEn", AppConfig::Waveform7DisplayEn ).toBool();
    AppConfig::Waveform8DisplayEn  = set.value("Waveform8DisplayEn", AppConfig::Waveform8DisplayEn ).toBool();
    AppConfig::Waveform9DisplayEn  = set.value("Waveform9DisplayEn", AppConfig::Waveform9DisplayEn ).toBool();
    AppConfig::Waveform10DisplayEn = set.value("Waveform10DisplayEn", AppConfig::Waveform10DisplayEn).toBool();
    AppConfig::Waveform11DisplayEn = set.value("Waveform11DisplayEn", AppConfig::Waveform11DisplayEn).toBool();
    AppConfig::Waveform12DisplayEn = set.value("Waveform12DisplayEn", AppConfig::Waveform12DisplayEn).toBool();
    AppConfig::GlobalZoom = set.value("GlobalZoom", AppConfig::GlobalZoom).toBool();
    set.endGroup();

    //脉冲发射配置组
    set.beginGroup("PulseTriggerConfig");
    //抽取间隔
    AppConfig::u4DecimateSpanCH00 = set.value("u4DecimateSpanCH00", AppConfig::u4DecimateSpanCH00).toUInt();
    AppConfig::u4DecimateSpanCH01 = set.value("u4DecimateSpanCH01", AppConfig::u4DecimateSpanCH01).toUInt();
    AppConfig::u4DecimateSpanCH02 = set.value("u4DecimateSpanCH02", AppConfig::u4DecimateSpanCH02).toUInt();
    AppConfig::u4DecimateSpanCH03 = set.value("u4DecimateSpanCH03", AppConfig::u4DecimateSpanCH03).toUInt();
    AppConfig::u4DecimateSpanCH04 = set.value("u4DecimateSpanCH04", AppConfig::u4DecimateSpanCH04).toUInt();
    AppConfig::u4DecimateSpanCH05 = set.value("u4DecimateSpanCH05", AppConfig::u4DecimateSpanCH05).toUInt();
    AppConfig::u4DecimateSpanCH06 = set.value("u4DecimateSpanCH06", AppConfig::u4DecimateSpanCH06).toUInt();
    AppConfig::u4DecimateSpanCH07 = set.value("u4DecimateSpanCH07", AppConfig::u4DecimateSpanCH07).toUInt();
    AppConfig::u4DecimateSpanCH08 = set.value("u4DecimateSpanCH08", AppConfig::u4DecimateSpanCH08).toUInt();
    AppConfig::u4DecimateSpanCH09 = set.value("u4DecimateSpanCH09", AppConfig::u4DecimateSpanCH09).toUInt();
    AppConfig::u4DecimateSpanCH10 = set.value("u4DecimateSpanCH10", AppConfig::u4DecimateSpanCH10).toUInt();
    AppConfig::u4DecimateSpanCH11 = set.value("u4DecimateSpanCH11", AppConfig::u4DecimateSpanCH11).toUInt();
    AppConfig::u4DecimateSpanCH12 = set.value("u4DecimateSpanCH12", AppConfig::u4DecimateSpanCH12).toUInt();
    //采样点数
    AppConfig::u4SampleCountCH00 = set.value("u4SampleCountCH00", AppConfig::u4SampleCountCH00).toUInt();
    AppConfig::u4SampleCountCH01 = set.value("u4SampleCountCH01", AppConfig::u4SampleCountCH01).toUInt();
    AppConfig::u4SampleCountCH02 = set.value("u4SampleCountCH02", AppConfig::u4SampleCountCH02).toUInt();
    AppConfig::u4SampleCountCH03 = set.value("u4SampleCountCH03", AppConfig::u4SampleCountCH03).toUInt();
    AppConfig::u4SampleCountCH04 = set.value("u4SampleCountCH04", AppConfig::u4SampleCountCH04).toUInt();
    AppConfig::u4SampleCountCH05 = set.value("u4SampleCountCH05", AppConfig::u4SampleCountCH05).toUInt();
    AppConfig::u4SampleCountCH06 = set.value("u4SampleCountCH06", AppConfig::u4SampleCountCH06).toUInt();
    AppConfig::u4SampleCountCH07 = set.value("u4SampleCountCH07", AppConfig::u4SampleCountCH07).toUInt();
    AppConfig::u4SampleCountCH08 = set.value("u4SampleCountCH08", AppConfig::u4SampleCountCH08).toUInt();
    AppConfig::u4SampleCountCH09 = set.value("u4SampleCountCH09", AppConfig::u4SampleCountCH09).toUInt();
    AppConfig::u4SampleCountCH10 = set.value("u4SampleCountCH10", AppConfig::u4SampleCountCH10).toUInt();
    AppConfig::u4SampleCountCH11 = set.value("u4SampleCountCH11", AppConfig::u4SampleCountCH11).toUInt();
    AppConfig::u4SampleCountCH12 = set.value("u4SampleCountCH12", AppConfig::u4SampleCountCH12).toUInt();
    //脉冲宽度
    AppConfig::u2PulseWidth00 = set.value("u2PulseWidth00", AppConfig::u2PulseWidth00).toUInt();
    AppConfig::u2PulseWidth01 = set.value("u2PulseWidth01", AppConfig::u2PulseWidth01).toUInt();
    AppConfig::u2PulseWidth02 = set.value("u2PulseWidth02", AppConfig::u2PulseWidth02).toUInt();
    AppConfig::u2PulseWidth03 = set.value("u2PulseWidth03", AppConfig::u2PulseWidth03).toUInt();
    AppConfig::u2PulseWidth04 = set.value("u2PulseWidth04", AppConfig::u2PulseWidth04).toUInt();
    AppConfig::u2PulseWidth05 = set.value("u2PulseWidth05", AppConfig::u2PulseWidth05).toUInt();
    AppConfig::u2PulseWidth06 = set.value("u2PulseWidth06", AppConfig::u2PulseWidth06).toUInt();
    AppConfig::u2PulseWidth07 = set.value("u2PulseWidth07", AppConfig::u2PulseWidth07).toUInt();
    AppConfig::u2PulseWidth08 = set.value("u2PulseWidth08", AppConfig::u2PulseWidth08).toUInt();
    AppConfig::u2PulseWidth09 = set.value("u2PulseWidth09", AppConfig::u2PulseWidth09).toUInt();
    AppConfig::u2PulseWidth10 = set.value("u2PulseWidth10", AppConfig::u2PulseWidth10).toUInt();
    AppConfig::u2PulseWidth11 = set.value("u2PulseWidth11", AppConfig::u2PulseWidth11).toUInt();
    AppConfig::u2PulseWidth12 = set.value("u2PulseWidth12", AppConfig::u2PulseWidth12).toUInt();
    //发射电压
    AppConfig::u2VoltageValue00 = set.value("u2VoltageValue00", AppConfig::u2VoltageValue00).toUInt();
    AppConfig::u2VoltageValue01 = set.value("u2VoltageValue01", AppConfig::u2VoltageValue01).toUInt();
    AppConfig::u2VoltageValue02 = set.value("u2VoltageValue02", AppConfig::u2VoltageValue02).toUInt();
    AppConfig::u2VoltageValue03 = set.value("u2VoltageValue03", AppConfig::u2VoltageValue03).toUInt();
    AppConfig::u2VoltageValue04 = set.value("u2VoltageValue04", AppConfig::u2VoltageValue04).toUInt();
    AppConfig::u2VoltageValue05 = set.value("u2VoltageValue05", AppConfig::u2VoltageValue05).toUInt();
    AppConfig::u2VoltageValue06 = set.value("u2VoltageValue06", AppConfig::u2VoltageValue06).toUInt();
    AppConfig::u2VoltageValue07 = set.value("u2VoltageValue07", AppConfig::u2VoltageValue07).toUInt();
    AppConfig::u2VoltageValue08 = set.value("u2VoltageValue08", AppConfig::u2VoltageValue08).toUInt();
    AppConfig::u2VoltageValue09 = set.value("u2VoltageValue09", AppConfig::u2VoltageValue09).toUInt();
    AppConfig::u2VoltageValue10 = set.value("u2VoltageValue10", AppConfig::u2VoltageValue10).toUInt();
    AppConfig::u2VoltageValue11 = set.value("u2VoltageValue11", AppConfig::u2VoltageValue11).toUInt();
    AppConfig::u2VoltageValue12 = set.value("u2VoltageValue12", AppConfig::u2VoltageValue12).toUInt();
    //硬件放大增益
    AppConfig::u2HardwareAmplifyFactorCH00 = set.value("u2HardwareAmplifyFactorCH00", AppConfig::u2HardwareAmplifyFactorCH00).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH01 = set.value("u2HardwareAmplifyFactorCH01", AppConfig::u2HardwareAmplifyFactorCH01).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH02 = set.value("u2HardwareAmplifyFactorCH02", AppConfig::u2HardwareAmplifyFactorCH02).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH03 = set.value("u2HardwareAmplifyFactorCH03", AppConfig::u2HardwareAmplifyFactorCH03).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH04 = set.value("u2HardwareAmplifyFactorCH04", AppConfig::u2HardwareAmplifyFactorCH04).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH05 = set.value("u2HardwareAmplifyFactorCH05", AppConfig::u2HardwareAmplifyFactorCH05).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH06 = set.value("u2HardwareAmplifyFactorCH06", AppConfig::u2HardwareAmplifyFactorCH06).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH07 = set.value("u2HardwareAmplifyFactorCH07", AppConfig::u2HardwareAmplifyFactorCH07).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH08 = set.value("u2HardwareAmplifyFactorCH08", AppConfig::u2HardwareAmplifyFactorCH08).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH09 = set.value("u2HardwareAmplifyFactorCH09", AppConfig::u2HardwareAmplifyFactorCH09).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH10 = set.value("u2HardwareAmplifyFactorCH10", AppConfig::u2HardwareAmplifyFactorCH10).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH11 = set.value("u2HardwareAmplifyFactorCH11", AppConfig::u2HardwareAmplifyFactorCH11).toUInt();
    AppConfig::u2HardwareAmplifyFactorCH12 = set.value("u2HardwareAmplifyFactorCH12", AppConfig::u2HardwareAmplifyFactorCH12).toUInt();
    //低通滤波
    AppConfig::u4LowPassFilteringCH00 = set.value("u4LowPassFilteringCH00", AppConfig::u4LowPassFilteringCH00).toUInt();
    AppConfig::u4LowPassFilteringCH01 = set.value("u4LowPassFilteringCH01", AppConfig::u4LowPassFilteringCH01).toUInt();
    AppConfig::u4LowPassFilteringCH02 = set.value("u4LowPassFilteringCH02", AppConfig::u4LowPassFilteringCH02).toUInt();
    AppConfig::u4LowPassFilteringCH03 = set.value("u4LowPassFilteringCH03", AppConfig::u4LowPassFilteringCH03).toUInt();
    AppConfig::u4LowPassFilteringCH04 = set.value("u4LowPassFilteringCH04", AppConfig::u4LowPassFilteringCH04).toUInt();
    AppConfig::u4LowPassFilteringCH05 = set.value("u4LowPassFilteringCH05", AppConfig::u4LowPassFilteringCH05).toUInt();
    AppConfig::u4LowPassFilteringCH06 = set.value("u4LowPassFilteringCH06", AppConfig::u4LowPassFilteringCH06).toUInt();
    AppConfig::u4LowPassFilteringCH07 = set.value("u4LowPassFilteringCH07", AppConfig::u4LowPassFilteringCH07).toUInt();
    AppConfig::u4LowPassFilteringCH08 = set.value("u4LowPassFilteringCH08", AppConfig::u4LowPassFilteringCH08).toUInt();
    AppConfig::u4LowPassFilteringCH09 = set.value("u4LowPassFilteringCH09", AppConfig::u4LowPassFilteringCH09).toUInt();
    AppConfig::u4LowPassFilteringCH10 = set.value("u4LowPassFilteringCH10", AppConfig::u4LowPassFilteringCH10).toUInt();
    AppConfig::u4LowPassFilteringCH11 = set.value("u4LowPassFilteringCH11", AppConfig::u4LowPassFilteringCH11).toUInt();
    AppConfig::u4LowPassFilteringCH12 = set.value("u4LowPassFilteringCH12", AppConfig::u4LowPassFilteringCH12).toUInt();
    //高通滤波
    AppConfig::u4HighPassFilteringCH00 = set.value("u4HighPassFilteringCH00", AppConfig::u4HighPassFilteringCH00).toUInt();
    AppConfig::u4HighPassFilteringCH01 = set.value("u4HighPassFilteringCH01", AppConfig::u4HighPassFilteringCH01).toUInt();
    AppConfig::u4HighPassFilteringCH02 = set.value("u4HighPassFilteringCH02", AppConfig::u4HighPassFilteringCH02).toUInt();
    AppConfig::u4HighPassFilteringCH03 = set.value("u4HighPassFilteringCH03", AppConfig::u4HighPassFilteringCH03).toUInt();
    AppConfig::u4HighPassFilteringCH04 = set.value("u4HighPassFilteringCH04", AppConfig::u4HighPassFilteringCH04).toUInt();
    AppConfig::u4HighPassFilteringCH05 = set.value("u4HighPassFilteringCH05", AppConfig::u4HighPassFilteringCH05).toUInt();
    AppConfig::u4HighPassFilteringCH06 = set.value("u4HighPassFilteringCH06", AppConfig::u4HighPassFilteringCH06).toUInt();
    AppConfig::u4HighPassFilteringCH07 = set.value("u4HighPassFilteringCH07", AppConfig::u4HighPassFilteringCH07).toUInt();
    AppConfig::u4HighPassFilteringCH08 = set.value("u4HighPassFilteringCH08", AppConfig::u4HighPassFilteringCH08).toUInt();
    AppConfig::u4HighPassFilteringCH09 = set.value("u4HighPassFilteringCH09", AppConfig::u4HighPassFilteringCH09).toUInt();
    AppConfig::u4HighPassFilteringCH10 = set.value("u4HighPassFilteringCH10", AppConfig::u4HighPassFilteringCH10).toUInt();
    AppConfig::u4HighPassFilteringCH11 = set.value("u4HighPassFilteringCH11", AppConfig::u4HighPassFilteringCH11).toUInt();
    AppConfig::u4HighPassFilteringCH12 = set.value("u4HighPassFilteringCH12", AppConfig::u4HighPassFilteringCH12).toUInt();
    //脉冲周期
    AppConfig::u4PulsePeriod = set.value("u4PulsePeriod", AppConfig::u4PulsePeriod).toUInt();
    //触发通道选择
    AppConfig::u2TriggerChannelSelected = set.value("u2TriggerChannelSelected", AppConfig::u2TriggerChannelSelected).toUInt();
    //触发模式选择
    AppConfig::u2TriggerMode = set.value("u2TriggerMode", AppConfig::u2TriggerMode).toUInt();

    //零声时修正
    AppConfig::u4ZeroModify = set.value("u4ZeroModify", AppConfig::u4ZeroModify).toUInt();
    //采样点起始时间偏移值
    AppConfig::u4SamplingOffset = set.value("u4SamplingOffset", AppConfig::u4SamplingOffset).toUInt();

    set.endGroup();

    //单路配置是否启用
    set.beginGroup("SystemSetting");
    AppConfig::blSingleChannelSettingEn = set.value("blSingleChannelSettingEn", AppConfig::blSingleChannelSettingEn).toBool();
    set.endGroup();

    //配置文件不存在或者不全则重新生成
    if (!QtHelper::checkIniFile(AppConfig::ConfigFile)) {
        writeConfig();
        return;
    }
}

void AppConfig::writeConfig()
{    
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);

    set.beginGroup("ComConfig");
    set.setValue("PortName", AppConfig::PortName);
    set.setValue("BaudRate", AppConfig::BaudRate);
    set.setValue("DataBit", AppConfig::DataBit);
    set.setValue("Parity", AppConfig::Parity);
    set.setValue("StopBit", AppConfig::StopBit);

    set.setValue("HexSend", AppConfig::HexSend);
    set.setValue("HexReceive", AppConfig::HexReceive);
    set.setValue("Debug", AppConfig::Debug);
    set.setValue("AutoClear", AppConfig::AutoClear);

    set.setValue("AutoSend", AppConfig::AutoSend);
    set.setValue("SendInterval", AppConfig::SendInterval);
    set.setValue("AutoSave", AppConfig::AutoSave);
    set.setValue("SaveInterval", AppConfig::SaveInterval);
    set.endGroup();

    set.beginGroup("NetUdpConfig");
    set.setValue("Mode", AppConfig::Mode);
    set.setValue("ServerIP", AppConfig::SlaveDeviceIP);
    set.setValue("ServerPort", AppConfig::SlaveDevicePort);
    set.setValue("SleepTime", AppConfig::SleepTime);
    set.setValue("AutoConnect", AppConfig::AutoConnect);
    //udp
    set.setValue("ListenPort", AppConfig::ListenPort);
    set.setValue("DestIP", AppConfig::DestIP);
    set.setValue("u4DestPort", AppConfig::u4DestPort);
    set.endGroup();

    set.beginGroup("WigetWaveformConfig");
    set.setValue("Waveform1DisplayEn", AppConfig::Waveform1DisplayEn);
    set.setValue("Waveform2DisplayEn", AppConfig::Waveform2DisplayEn);
    set.setValue("Waveform3DisplayEn", AppConfig::Waveform3DisplayEn);
    set.setValue("Waveform4DisplayEn", AppConfig::Waveform4DisplayEn);
    set.setValue("Waveform5DisplayEn", AppConfig::Waveform5DisplayEn);
    set.setValue("Waveform6DisplayEn", AppConfig::Waveform6DisplayEn);
    set.setValue("Waveform7DisplayEn", AppConfig::Waveform7DisplayEn);
    set.setValue("Waveform8DisplayEn", AppConfig::Waveform8DisplayEn);
    set.setValue("Waveform9DisplayEn", AppConfig::Waveform9DisplayEn);
    set.setValue("Waveform10DisplayEn", AppConfig::Waveform10DisplayEn);
    set.setValue("Waveform11DisplayEn", AppConfig::Waveform11DisplayEn);
    set.setValue("Waveform12DisplayEn", AppConfig::Waveform12DisplayEn);
    set.setValue("GlobalZoom", AppConfig::GlobalZoom);
    set.endGroup();

    //脉冲发射配置组
    set.beginGroup("PulseTriggerConfig");
    //抽取间隔
    set.setValue("u4DecimateSpanCH00", AppConfig::u4DecimateSpanCH00);
    set.setValue("u4DecimateSpanCH01", AppConfig::u4DecimateSpanCH01);
    set.setValue("u4DecimateSpanCH02", AppConfig::u4DecimateSpanCH02);
    set.setValue("u4DecimateSpanCH03", AppConfig::u4DecimateSpanCH03);
    set.setValue("u4DecimateSpanCH04", AppConfig::u4DecimateSpanCH04);
    set.setValue("u4DecimateSpanCH05", AppConfig::u4DecimateSpanCH05);
    set.setValue("u4DecimateSpanCH06", AppConfig::u4DecimateSpanCH06);
    set.setValue("u4DecimateSpanCH07", AppConfig::u4DecimateSpanCH07);
    set.setValue("u4DecimateSpanCH08", AppConfig::u4DecimateSpanCH08);
    set.setValue("u4DecimateSpanCH09", AppConfig::u4DecimateSpanCH09);
    set.setValue("u4DecimateSpanCH10", AppConfig::u4DecimateSpanCH10);
    set.setValue("u4DecimateSpanCH11", AppConfig::u4DecimateSpanCH11);
    set.setValue("u4DecimateSpanCH12", AppConfig::u4DecimateSpanCH12);
    //采样点数
    set.setValue("u4SampleCountCH00", AppConfig::u4SampleCountCH00);
    set.setValue("u4SampleCountCH01", AppConfig::u4SampleCountCH01);
    set.setValue("u4SampleCountCH02", AppConfig::u4SampleCountCH02);
    set.setValue("u4SampleCountCH03", AppConfig::u4SampleCountCH03);
    set.setValue("u4SampleCountCH04", AppConfig::u4SampleCountCH04);
    set.setValue("u4SampleCountCH05", AppConfig::u4SampleCountCH05);
    set.setValue("u4SampleCountCH06", AppConfig::u4SampleCountCH06);
    set.setValue("u4SampleCountCH07", AppConfig::u4SampleCountCH07);
    set.setValue("u4SampleCountCH08", AppConfig::u4SampleCountCH08);
    set.setValue("u4SampleCountCH09", AppConfig::u4SampleCountCH09);
    set.setValue("u4SampleCountCH10", AppConfig::u4SampleCountCH10);
    set.setValue("u4SampleCountCH11", AppConfig::u4SampleCountCH11);
    set.setValue("u4SampleCountCH12", AppConfig::u4SampleCountCH12);
    //脉冲宽度
    set.setValue("u2PulseWidth00", AppConfig::u2PulseWidth00);
    set.setValue("u2PulseWidth01", AppConfig::u2PulseWidth01);
    set.setValue("u2PulseWidth02", AppConfig::u2PulseWidth02);
    set.setValue("u2PulseWidth03", AppConfig::u2PulseWidth03);
    set.setValue("u2PulseWidth04", AppConfig::u2PulseWidth04);
    set.setValue("u2PulseWidth05", AppConfig::u2PulseWidth05);
    set.setValue("u2PulseWidth06", AppConfig::u2PulseWidth06);
    set.setValue("u2PulseWidth07", AppConfig::u2PulseWidth07);
    set.setValue("u2PulseWidth08", AppConfig::u2PulseWidth08);
    set.setValue("u2PulseWidth09", AppConfig::u2PulseWidth09);
    set.setValue("u2PulseWidth10", AppConfig::u2PulseWidth10);
    set.setValue("u2PulseWidth11", AppConfig::u2PulseWidth11);
    set.setValue("u2PulseWidth12", AppConfig::u2PulseWidth12);
    //发射电压
    set.setValue("u2VoltageValue00", AppConfig::u2VoltageValue00);
    set.setValue("u2VoltageValue01", AppConfig::u2VoltageValue01);
    set.setValue("u2VoltageValue02", AppConfig::u2VoltageValue02);
    set.setValue("u2VoltageValue03", AppConfig::u2VoltageValue03);
    set.setValue("u2VoltageValue04", AppConfig::u2VoltageValue04);
    set.setValue("u2VoltageValue05", AppConfig::u2VoltageValue05);
    set.setValue("u2VoltageValue06", AppConfig::u2VoltageValue06);
    set.setValue("u2VoltageValue07", AppConfig::u2VoltageValue07);
    set.setValue("u2VoltageValue08", AppConfig::u2VoltageValue08);
    set.setValue("u2VoltageValue09", AppConfig::u2VoltageValue09);
    set.setValue("u2VoltageValue10", AppConfig::u2VoltageValue10);
    set.setValue("u2VoltageValue11", AppConfig::u2VoltageValue11);
    set.setValue("u2VoltageValue12", AppConfig::u2VoltageValue12);
    //硬件放大增益
    set.setValue("u2HardwareAmplifyFactorCH00", AppConfig::u2HardwareAmplifyFactorCH00);
    set.setValue("u2HardwareAmplifyFactorCH01", AppConfig::u2HardwareAmplifyFactorCH01);
    set.setValue("u2HardwareAmplifyFactorCH02", AppConfig::u2HardwareAmplifyFactorCH02);
    set.setValue("u2HardwareAmplifyFactorCH03", AppConfig::u2HardwareAmplifyFactorCH03);
    set.setValue("u2HardwareAmplifyFactorCH04", AppConfig::u2HardwareAmplifyFactorCH04);
    set.setValue("u2HardwareAmplifyFactorCH05", AppConfig::u2HardwareAmplifyFactorCH05);
    set.setValue("u2HardwareAmplifyFactorCH06", AppConfig::u2HardwareAmplifyFactorCH06);
    set.setValue("u2HardwareAmplifyFactorCH07", AppConfig::u2HardwareAmplifyFactorCH07);
    set.setValue("u2HardwareAmplifyFactorCH08", AppConfig::u2HardwareAmplifyFactorCH08);
    set.setValue("u2HardwareAmplifyFactorCH09", AppConfig::u2HardwareAmplifyFactorCH09);
    set.setValue("u2HardwareAmplifyFactorCH10", AppConfig::u2HardwareAmplifyFactorCH10);
    set.setValue("u2HardwareAmplifyFactorCH11", AppConfig::u2HardwareAmplifyFactorCH11);
    set.setValue("u2HardwareAmplifyFactorCH12", AppConfig::u2HardwareAmplifyFactorCH12);
    //低通滤波
    set.setValue("u4LowPassFilteringCH00", AppConfig::u4LowPassFilteringCH00);
    set.setValue("u4LowPassFilteringCH01", AppConfig::u4LowPassFilteringCH01);
    set.setValue("u4LowPassFilteringCH02", AppConfig::u4LowPassFilteringCH02);
    set.setValue("u4LowPassFilteringCH03", AppConfig::u4LowPassFilteringCH03);
    set.setValue("u4LowPassFilteringCH04", AppConfig::u4LowPassFilteringCH04);
    set.setValue("u4LowPassFilteringCH05", AppConfig::u4LowPassFilteringCH05);
    set.setValue("u4LowPassFilteringCH06", AppConfig::u4LowPassFilteringCH06);
    set.setValue("u4LowPassFilteringCH07", AppConfig::u4LowPassFilteringCH07);
    set.setValue("u4LowPassFilteringCH08", AppConfig::u4LowPassFilteringCH08);
    set.setValue("u4LowPassFilteringCH09", AppConfig::u4LowPassFilteringCH09);
    set.setValue("u4LowPassFilteringCH10", AppConfig::u4LowPassFilteringCH10);
    set.setValue("u4LowPassFilteringCH11", AppConfig::u4LowPassFilteringCH11);
    set.setValue("u4LowPassFilteringCH12", AppConfig::u4LowPassFilteringCH12);
    //高通滤波
    set.setValue("u4HighPassFilteringCH00", AppConfig::u4HighPassFilteringCH00);
    set.setValue("u4HighPassFilteringCH01", AppConfig::u4HighPassFilteringCH01);
    set.setValue("u4HighPassFilteringCH02", AppConfig::u4HighPassFilteringCH02);
    set.setValue("u4HighPassFilteringCH03", AppConfig::u4HighPassFilteringCH03);
    set.setValue("u4HighPassFilteringCH04", AppConfig::u4HighPassFilteringCH04);
    set.setValue("u4HighPassFilteringCH05", AppConfig::u4HighPassFilteringCH05);
    set.setValue("u4HighPassFilteringCH06", AppConfig::u4HighPassFilteringCH06);
    set.setValue("u4HighPassFilteringCH07", AppConfig::u4HighPassFilteringCH07);
    set.setValue("u4HighPassFilteringCH08", AppConfig::u4HighPassFilteringCH08);
    set.setValue("u4HighPassFilteringCH09", AppConfig::u4HighPassFilteringCH09);
    set.setValue("u4HighPassFilteringCH10", AppConfig::u4HighPassFilteringCH10);
    set.setValue("u4HighPassFilteringCH11", AppConfig::u4HighPassFilteringCH11);
    set.setValue("u4HighPassFilteringCH12", AppConfig::u4HighPassFilteringCH12);

    //脉冲周期
    set.setValue("u4PulsePeriod", AppConfig::u4PulsePeriod);

    //触发通道选择
    set.setValue("u2TriggerChannelSelected", AppConfig::u2TriggerChannelSelected);

    //触发模式选择
    set.setValue("u2TriggerMode", AppConfig::u2TriggerMode);

    //零声时修正
    set.setValue("u4ZeroModify", AppConfig::u4ZeroModify);
    //采样点起始时间偏移值
    set.setValue("u4SamplingOffset", AppConfig::u4SamplingOffset);

    set.endGroup();

    //单路配置启用
    set.beginGroup("SystemSetting");
    set.setValue("blSingleChannelSettingEn", AppConfig::blSingleChannelSettingEn);
    set.endGroup();
}
