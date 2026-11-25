#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "head.h"

class AppConfig
{
public:
    static QString ConfigFile;          //配置文件路径
    static QString SendFileName;        //发送配置文件名
    static QString DeviceFileName;      //模拟设备数据文件名

    static QString PortName;            //串口号
    static int BaudRate;                //波特率
    static int DataBit;                 //数据位
    static QString Parity;              //校验位
    static double StopBit;              //停止位

    static bool HexSend;                //16进制发送
    static bool HexReceive;             //16进制接收
    static bool Debug;                  //模拟设备
    static bool AutoClear;              //自动清空

    static bool AutoSend;               //自动发送
    static int SendInterval;            //自动发送间隔
    static bool AutoSave;               //自动保存
    static int SaveInterval;            //自动保存间隔

    static QString Mode;                //转换模式
    static QString SlaveDeviceIP;       //从机设备IP
    static int SlaveDevicePort;         //服务器端口
    static int ListenPort;              //监听端口
    static int SleepTime;               //延时时间
    static bool AutoConnect;            //自动重连



    //波形显示配置参数
    static bool Waveform1DisplayEn;     //波形1显示开关
    static bool Waveform2DisplayEn;     //波形2显示开关
    static bool Waveform3DisplayEn;
    static bool Waveform4DisplayEn;
    static bool Waveform5DisplayEn;     //波形5显示开关
    static bool Waveform6DisplayEn;     //波形6显示开关
    static bool Waveform7DisplayEn;
    static bool Waveform8DisplayEn;
    static bool Waveform9DisplayEn;     //波形9显示开关
    static bool Waveform10DisplayEn;    //波形10显示开关
    static bool Waveform11DisplayEn;
    static bool Waveform12DisplayEn;

    static bool GlobalZoom;             //波形整体缩放使能

    //脉冲触发配置参数
    static uint16_t u2TriggerMode;     //触发模式


    static uint32_t u4DecimateSpanCH00;  //所有通道的配置 抽取间隔 单位ns
    static uint32_t u4DecimateSpanCH01;  //抽取间隔 单位ns
    static uint32_t u4DecimateSpanCH02;  //
    static uint32_t u4DecimateSpanCH03;  //
    static uint32_t u4DecimateSpanCH04;  //
    static uint32_t u4DecimateSpanCH05;  //
    static uint32_t u4DecimateSpanCH06;  //
    static uint32_t u4DecimateSpanCH07;  //
    static uint32_t u4DecimateSpanCH08;  //
    static uint32_t u4DecimateSpanCH09;  //
    static uint32_t u4DecimateSpanCH10;  //
    static uint32_t u4DecimateSpanCH11;  //
    static uint32_t u4DecimateSpanCH12;  //

    static uint32_t u4SampleCountCH00; 	 //所有通道的配置 采样点数
    static uint32_t u4SampleCountCH01; 	 //采样点数
    static uint32_t u4SampleCountCH02; 	 //
    static uint32_t u4SampleCountCH03; 	 //
    static uint32_t u4SampleCountCH04; 	 //
    static uint32_t u4SampleCountCH05; 	 //
    static uint32_t u4SampleCountCH06; 	 //
    static uint32_t u4SampleCountCH07; 	 //
    static uint32_t u4SampleCountCH08; 	 //
    static uint32_t u4SampleCountCH09; 	 //
    static uint32_t u4SampleCountCH10; 	 //
    static uint32_t u4SampleCountCH11; 	 //
    static uint32_t u4SampleCountCH12; 	 //

    static uint16_t u2PulseWidth00;      //所有通道的配置 脉冲宽度
    static uint16_t u2PulseWidth01;      //脉冲宽度
    static uint16_t u2PulseWidth02;      //
    static uint16_t u2PulseWidth03;      //
    static uint16_t u2PulseWidth04;      //
    static uint16_t u2PulseWidth05;      //
    static uint16_t u2PulseWidth06;      //
    static uint16_t u2PulseWidth07;      //
    static uint16_t u2PulseWidth08;      //
    static uint16_t u2PulseWidth09;      //
    static uint16_t u2PulseWidth10;      //
    static uint16_t u2PulseWidth11;      //
    static uint16_t u2PulseWidth12;      //

    static uint16_t u2VoltageValue00;    //所有通道的配置 发射电压
    static uint16_t u2VoltageValue01;    //发射电压
    static uint16_t u2VoltageValue02;    //
    static uint16_t u2VoltageValue03;    //
    static uint16_t u2VoltageValue04;    //
    static uint16_t u2VoltageValue05;    //
    static uint16_t u2VoltageValue06;    //
    static uint16_t u2VoltageValue07;    //
    static uint16_t u2VoltageValue08;    //
    static uint16_t u2VoltageValue09;    //
    static uint16_t u2VoltageValue10;    //
    static uint16_t u2VoltageValue11;    //
    static uint16_t u2VoltageValue12;    //

    static uint16_t u2HardwareAmplifyFactorCH00; //所有通道的配置硬件运放放大增益
    static uint16_t u2HardwareAmplifyFactorCH01; //硬件运放放大增益
    static uint16_t u2HardwareAmplifyFactorCH02;
    static uint16_t u2HardwareAmplifyFactorCH03;
    static uint16_t u2HardwareAmplifyFactorCH04;
    static uint16_t u2HardwareAmplifyFactorCH05;
    static uint16_t u2HardwareAmplifyFactorCH06;
    static uint16_t u2HardwareAmplifyFactorCH07;
    static uint16_t u2HardwareAmplifyFactorCH08;
    static uint16_t u2HardwareAmplifyFactorCH09;
    static uint16_t u2HardwareAmplifyFactorCH10;
    static uint16_t u2HardwareAmplifyFactorCH11;
    static uint16_t u2HardwareAmplifyFactorCH12;

    static uint32_t u4LowPassFilteringCH00; //所有通道的配置 低通滤波
    static uint32_t u4LowPassFilteringCH01; //低通滤波
    static uint32_t u4LowPassFilteringCH02;
    static uint32_t u4LowPassFilteringCH03;
    static uint32_t u4LowPassFilteringCH04;
    static uint32_t u4LowPassFilteringCH05;
    static uint32_t u4LowPassFilteringCH06;
    static uint32_t u4LowPassFilteringCH07;
    static uint32_t u4LowPassFilteringCH08;
    static uint32_t u4LowPassFilteringCH09;
    static uint32_t u4LowPassFilteringCH10;
    static uint32_t u4LowPassFilteringCH11;
    static uint32_t u4LowPassFilteringCH12;

    static uint32_t u4HighPassFilteringCH00; //所有通道的配置 高通滤波
    static uint32_t u4HighPassFilteringCH01; //高通滤波
    static uint32_t u4HighPassFilteringCH02;
    static uint32_t u4HighPassFilteringCH03;
    static uint32_t u4HighPassFilteringCH04;
    static uint32_t u4HighPassFilteringCH05;
    static uint32_t u4HighPassFilteringCH06;
    static uint32_t u4HighPassFilteringCH07;
    static uint32_t u4HighPassFilteringCH08;
    static uint32_t u4HighPassFilteringCH09;
    static uint32_t u4HighPassFilteringCH10;
    static uint32_t u4HighPassFilteringCH11;
    static uint32_t u4HighPassFilteringCH12;

    static uint32_t u4PulsePeriod;      //脉冲周期
    static uint16_t u2TriggerChannelSelected;     //触发通道选择

    static QString  DestIP;             //目标超声主板板卡设备IP
    static uint32_t u4DestPort;         //目标超声主板板卡端口

    static QString  DestUartToWifiIP;   //RS485、RS232转网口设备的IP
    static uint32_t u4DestUartToWifi;   //RS485、RS232转网口设备的端口

    static bool blSingleChannelSettingEn; //是否启用单个通道配置

    static uint32_t u4ZeroModify;          //零声时修正
    static uint32_t u4SamplingOffset;      //采样点起始时间偏移值

    //读写配置参数
    static void readConfig();           //读取配置参数
    static void writeConfig();          //写入配置参数
};

#endif // APPCONFIG_H
