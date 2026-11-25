#ifndef SONIC_LAUNCH_CFG_H
#define SONIC_LAUNCH_CFG_H

#include <QButtonGroup>
#include <QWidget>
#include <QUdpSocket>
#include <QStyleOptionTitleBar>
#include <QProxyStyle>
#include <QLineEdit>
#include <QButtonGroup>
#include <QComboBox>
#include <QRadioButton>

typedef enum{
  STOP_TRI= 0x00,
  ONCE_TRI = 0x01,
  CONTINUE_TRI,
  ONCE_EXT_TRI,
  CONTINUE_EXT_TRI,
  SIGNAL_THRESHOLD_TRI,
  BSACN_SEQUENCE_TRI, //B扫成像序列发射模式
  NONE_TRI
}SERIAL_HMI_AUTO_PARSER_STATUS_E;


// 枚举表示不同的按钮组 ID
enum class ButtonGroupId {
    TRIGGER_MODE = 0,
    SAMPLING_RATE,
    VOLTAGE,
    HIGH_PASS_FILTER,
    LOW_PASS_FILTER
};

namespace Ui {
class SonicLaunchCfg;
}

class SonicLaunchCfg : public QWidget
{
    Q_OBJECT

public:
    explicit SonicLaunchCfg(QWidget *parent = nullptr);
    ~SonicLaunchCfg();

private slots:
    void slots_trigger();//触发
    void slots_HPF();//高通滤波
    void slots_LPF();//低通滤波
    void slots_sampling();//采样点数
    void slots_Voltage();//电压
    void slots_SingleSetting();//启用单路配置
    void slots_channel();//通道选择
    void slots_interval();//采样间隔
    void slots_Pulse();//发射脉宽
    void slots_gain();//增益
    void slots_period();//连续触发周期值
    void slots_ZeroTimingModify();//零声时修正
    void slots_SetSamplingOffset();//采集点偏移时修正

    void CheckVoltageRbt(uint16_t  u2VoltageValueX);
    void CheckSampleCountRbt(uint16_t  u2SampleCountX);

    void on_pBtSaveTriSetting_clicked();

    // 创建按钮组并添加按钮
    void createButtonGroup(ButtonGroupId groupId, QButtonGroup *&buttonGroup, const QList<QRadioButton *> &radioButtons, const QList<int> &ids);
    // 连接信号与槽
    void connectSignalsToSlots();

signals:
    void  SigRefreshTriggerParameter();
private:
    Ui::SonicLaunchCfg *ui;

    QButtonGroup *groupButtonTriggerMode;
    QButtonGroup *groupButtonSampleCounter;
    QButtonGroup *groupButtonTriggerVoltage;
    QButtonGroup *groupButtonLowPass;
    QButtonGroup *groupButtonHighPass;
    QString  strTriggerMode;
    QString  strDecimateSpan;
    QString  strSampleCount;
    QString  strVoltageValue;
    QList<QLineEdit *> lineEditsGainConfig;
protected slots:
    void initLaunchConfig();          //初始化配置文件
    void saveConfig();          //保存配置文件

};

#endif // SONIC_LAUNCH_CFG_H
