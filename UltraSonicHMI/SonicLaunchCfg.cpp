/****************************************************************************
**
** 文件名: SonicLaunchCfg.c
** 作者: Zhou LiGui
** 创建日期: 2025年4月11日
** 功能描述: 对声波的发射和回波采集参数进行设置。
**
** 详细描述:
**
** 版权所有 (C): 2024 GZU
**
** 注意事项:
**
****************************************************************************/
#include "SonicLaunchCfg.h"
#include <ui/ui_SonicLaunchCfg.h>
#include <QVector>
#include "QDebug"
#include "widget.h"
#include <QPushButton>
#include <QObject>
#include "appconfig.h"
#include <unordered_map>



SonicLaunchCfg::SonicLaunchCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SonicLaunchCfg)
{
    ui->setupUi(this);
    this->initLaunchConfig();//初始化发射配置文件

    /*绑定12个增益的槽函数*/
    lineEditsGainConfig << ui->lineEdit_2 << ui->lE_HardwareAmpCH2 << ui->lE_HardwareAmpCH3 << ui->lE_HardwareAmpCH4 \
                        << ui->lE_HardwareAmpCH5 << ui->lE_HardwareAmpCH6 << ui->lE_HardwareAmpCH7 << ui->lE_HardwareAmpCH8
                        << ui->lE_HardwareAmpCH9 << ui->lE_HardwareAmpCH10 << ui->lE_HardwareAmpCH11 << ui->lE_HardwareAmpCH12;
    // 连接增益配置的 QLineEdit 的 textChanged 信号到 slots_gain 槽函数
    for (int i = 0; i < lineEditsGainConfig.count(); ++i)
    {
        QLineEdit *LineEdit = (QLineEdit *)lineEditsGainConfig.at(i);
        QObject::connect(LineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(slots_gain()));
    }

    // 创建按钮组并添加按钮
    /*创建_触发模式_按钮组并添加按钮*/
    createButtonGroup(ButtonGroupId::TRIGGER_MODE, groupButtonTriggerMode, {ui->radioButton, ui->radioButton_2, ui->radioButton_3, ui->radioButton_4, ui->radioButton_5}, {0, 1, 2, 3, 4});
    /*创建_采样点数_按钮组并添加按钮*/
    createButtonGroup(ButtonGroupId::SAMPLING_RATE, groupButtonSampleCounter, {ui->radioButton_6, ui->radioButton_7, ui->radioButton_8, ui->radioButton_9}, {0, 1, 2, 3});
    /*创建_发射电压_按钮组并添加按钮*/
    createButtonGroup(ButtonGroupId::VOLTAGE, groupButtonTriggerVoltage, {ui->radioButton_10, ui->radioButton_12, ui->radioButton_11, ui->radioButton_13, ui->rB_CustomVoltage}, {0, 1, 2, 3, 4});
    /*创建_高通滤波_按钮组并添加按钮*/
    createButtonGroup(ButtonGroupId::HIGH_PASS_FILTER, groupButtonHighPass, {ui->rB_NoHighPassFilter, ui->radioButton_14, ui->radioButton_15, ui->radioButton_16}, {0, 1, 2, 3});
    /*创建_低通滤波_按钮组并添加按钮*/
    createButtonGroup(ButtonGroupId::LOW_PASS_FILTER, groupButtonLowPass, {ui->rB_NoLowPassFilter, ui->radioButton_17, ui->radioButton_18, ui->radioButton_19, ui->radioButton_20, ui->radioButton_21}, {0, 1, 2, 3, 4, 5});

    // 连接信号与槽
    connectSignalsToSlots();
}


/*析构函数*/
SonicLaunchCfg::~SonicLaunchCfg()
{
    delete ui;
}

// 创建按钮组并添加按钮
void SonicLaunchCfg::createButtonGroup(ButtonGroupId groupId, QButtonGroup *&buttonGroup, const QList<QRadioButton *> &radioButtons, const QList<int> &ids) {
    buttonGroup = new QButtonGroup(this);
    for (int i = 0; i < radioButtons.count(); ++i)
    {
        buttonGroup->addButton(radioButtons.at(i), ids.at(i));
    }
}

// 连接信号与槽
void SonicLaunchCfg::connectSignalsToSlots()
{
    // 连接触发模式按钮的 clicked 信号到 slots_trigger 槽函数
    connect(groupButtonTriggerMode, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &SonicLaunchCfg::slots_trigger);

    // 连接高通滤波按钮的 clicked 信号到 slots_HPF 槽函数
    connect(groupButtonHighPass, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &SonicLaunchCfg::slots_HPF);

    // 连接低通滤波按钮的 clicked 信号到 slots_LPF 槽函数
    connect(groupButtonLowPass, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &SonicLaunchCfg::slots_LPF);

    // 连接采样率按钮的 clicked 信号到 slots_sampling 槽函数
    connect(groupButtonSampleCounter, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &SonicLaunchCfg::slots_sampling);
    connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, &SonicLaunchCfg::slots_sampling);

    // 连接电压按钮的 clicked 信号到 slots_Voltage 槽函数
    connect(groupButtonTriggerVoltage, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &SonicLaunchCfg::slots_Voltage);
    connect(ui->LE_CustomVoltage, &QLineEdit::textChanged, this, &SonicLaunchCfg::slots_Voltage);

    // 连接通道选择的 currentTextChanged 信号到 slots_channel 槽函数
    connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, &SonicLaunchCfg::slots_channel);

    // 连接其他 QLineEdit 的 textChanged 信号到相应的槽函数
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &SonicLaunchCfg::slots_interval);
    connect(ui->lineEdit_3, &QLineEdit::textChanged, this, &SonicLaunchCfg::slots_Pulse);
    connect(ui->lineEdit_4, &QLineEdit::textChanged, this, &SonicLaunchCfg::slots_period);
    connect(ui->lineEdit_ZeroModify, &QLineEdit::textChanged, this, &SonicLaunchCfg::slots_ZeroTimingModify);
    connect(ui->lineEdit_SamplingOffset, &QLineEdit::textChanged, this, &SonicLaunchCfg::slots_SetSamplingOffset);

    // 连接单通道选择按钮的 clicked 信号到 slots_SingleSetting 槽函数
    connect(ui->rBtnSingleSetting, &QRadioButton::clicked, this, &SonicLaunchCfg::slots_SingleSetting);
}

/*触发模式槽函数*/
void SonicLaunchCfg::slots_trigger()
{
    AppConfig::u2TriggerMode = groupButtonTriggerMode->checkedId() + 1; /*触发模式的值与Id值差一个1*/
}

/*高通滤波槽函数*/
void SonicLaunchCfg::slots_HPF()
{
    switch(groupButtonHighPass->checkedId())
    {
    case 0:
        AppConfig::u4HighPassFilteringCH00 = 0;
        break;
    case 1:
        AppConfig::u4HighPassFilteringCH00 = 10;
        break;
    case 2:
        AppConfig::u4HighPassFilteringCH00 = 500;
        break;
    case 3:
        AppConfig::u4HighPassFilteringCH00 = 2000;
        break;
    default:
        break;
    }
}

/*低通滤波槽函数*/
void SonicLaunchCfg::slots_LPF()
{
    switch(groupButtonLowPass->checkedId())
    {
    case 0:
        AppConfig::u4LowPassFilteringCH00 = 0;
        break;
    case 1:
        AppConfig::u4LowPassFilteringCH00 = 3000;
        break;
    case 2:
        AppConfig::u4LowPassFilteringCH00 = 30000;
        break;
    case 3:
        AppConfig::u4LowPassFilteringCH00 = 60000;
        break;
    case 4:
        AppConfig::u4LowPassFilteringCH00 = 200000;
        break;
    case 5:
        AppConfig::u4LowPassFilteringCH00 = 500000;
        break;
    default:
        break;
    }
}

/*采样点数设置槽函数*/
void SonicLaunchCfg::slots_sampling()
{
    // 定义采样点数映射表：checkedId -> 采样数
    const uint32_t sampleCountValues[] = {512, 2048, 1024, 4096};
    int checkedId = groupButtonSampleCounter->checkedId();

    // 检查checkedId范围有效性
    if (checkedId < 0 || checkedId >= static_cast<int>(sizeof(sampleCountValues)/sizeof(sampleCountValues[0]))) {
        qWarning() << "Invalid checkedId for sampling points:" << checkedId;
        return;
    }
    uint32_t sampleCount = sampleCountValues[checkedId];

    // 通道配置映射表：通道名 -> 对应配置变量
    static const QMap<QString, uint32_t*> channelConfigMap = {
        {"CH1",  &AppConfig::u4SampleCountCH01},
        {"CH2",  &AppConfig::u4SampleCountCH02},
        {"CH3",  &AppConfig::u4SampleCountCH03},
        {"CH4",  &AppConfig::u4SampleCountCH04},
        {"CH5",  &AppConfig::u4SampleCountCH05},
        {"CH6",  &AppConfig::u4SampleCountCH06},
        {"CH7",  &AppConfig::u4SampleCountCH07},
        {"CH8",  &AppConfig::u4SampleCountCH08},
        {"CH9",  &AppConfig::u4SampleCountCH09},
        {"CH10", &AppConfig::u4SampleCountCH10},
        {"CH11", &AppConfig::u4SampleCountCH11},
        {"CH12", &AppConfig::u4SampleCountCH12}
    };

    if (AppConfig::blSingleChannelSettingEn)
    {
        // 单通道模式：获取当前通道并更新对应配置
        QString currentChannel = ui->comboBox_2->currentText();
        if (channelConfigMap.contains(currentChannel)) {
            *channelConfigMap.value(currentChannel) = sampleCount;
        }
        else
        {
            qWarning() << "Invalid channel selected:" << currentChannel;
        }
    }
    else
    {
        // 全局模式：更新统一配置
        AppConfig::u4SampleCountCH00 = sampleCount;
    }
}


/*发射电压槽函数*/
void SonicLaunchCfg::slots_Voltage()
{
    // 定义电压值
    uint16_t voltageValue = 0;
    switch (groupButtonTriggerVoltage->checkedId())
    {
        case 0: voltageValue = 300; break;  // 300V
        case 1: voltageValue = 600; break;  // 600V
        case 2: voltageValue = 800; break;  // 800V
        case 3: voltageValue = 1000; break; // 1000V
        case 4: // 自定义电压
        {
            QString text = ui->LE_CustomVoltage->text();
            bool ok;
            voltageValue = (uint16_t)text.toUShort(&ok);
            if (!ok || voltageValue > 1000)
            {
                return; // 输入无效或超出范围，直接返回
            }
            break;
        }
        default: return; // 无效的选项，直接返回
    }

    // 如果是单通道设置
    if (AppConfig::blSingleChannelSettingEn)
    {
        // 获取当前选择的通道
        QString channel = ui->comboBox_2->currentText();
        if (channel.startsWith("CH"))
        {
            // 提取通道号
            int channelNumber = channel.mid(2).toInt();
            if (channelNumber >= 1 && channelNumber <= 12)
            {
                // 使用指针数组映射通道号到对应的电压变量
                static uint16_t* voltageMap[] = {
                    &AppConfig::u2VoltageValue01,
                    &AppConfig::u2VoltageValue02,
                    &AppConfig::u2VoltageValue03,
                    &AppConfig::u2VoltageValue04,
                    &AppConfig::u2VoltageValue05,
                    &AppConfig::u2VoltageValue06,
                    &AppConfig::u2VoltageValue07,
                    &AppConfig::u2VoltageValue08,
                    &AppConfig::u2VoltageValue09,
                    &AppConfig::u2VoltageValue10,
                    &AppConfig::u2VoltageValue11,
                    &AppConfig::u2VoltageValue12
                };

                // 设置对应通道的电压值
                *voltageMap[channelNumber - 1] = voltageValue;
            }
        }
    }
    else
    {
        // 非单通道设置，设置全局电压值
        AppConfig::u2VoltageValue00 = voltageValue;
    }
}

/*发射通道选择槽函数*/
void SonicLaunchCfg::slots_channel()
{
    AppConfig::u2TriggerChannelSelected = ui->comboBox_2->currentIndex() + 1;
    this->initLaunchConfig();//切换通道时,重新初始化配置文件，更新到新的channel控件值
}


/*采样点数抽取间隔槽函数*/
void SonicLaunchCfg::slots_interval()
{
    // 获取 lineEdit 中的文本
    QString text = ui->lineEdit->text();
    bool ok;
    qint64 value = text.toLongLong(&ok); // 将文本转换为整数
    if (ok)
    {
        QMap<QString, uint32_t*> channelMap =
        {
            {"CH1", &AppConfig::u4DecimateSpanCH01},
            {"CH2", &AppConfig::u4DecimateSpanCH02},
            {"CH3", &AppConfig::u4DecimateSpanCH03},
            {"CH4", &AppConfig::u4DecimateSpanCH04},
            {"CH5", &AppConfig::u4DecimateSpanCH05},
            {"CH6", &AppConfig::u4DecimateSpanCH06},
            {"CH7", &AppConfig::u4DecimateSpanCH07},
            {"CH8", &AppConfig::u4DecimateSpanCH08},
            {"CH9", &AppConfig::u4DecimateSpanCH09},
            {"CH10", &AppConfig::u4DecimateSpanCH10},
            {"CH11", &AppConfig::u4DecimateSpanCH11},
            {"CH12", &AppConfig::u4DecimateSpanCH12}
        };

        if (AppConfig::blSingleChannelSettingEn)
        {
            QString currentChannel = ui->comboBox_2->currentText();
            if (channelMap.contains(currentChannel))
            {
                *channelMap[currentChannel] = static_cast<uint32_t>(value);
            }
        }
        else
        {
            AppConfig::u4DecimateSpanCH00 = static_cast<uint32_t>(value);
        }
    }

    ui->label_6->setText(QString::number(value * 40));
}



/*脉冲宽度槽函数*/
void SonicLaunchCfg::slots_Pulse()
{
    // 获取 lineEdit 中的文本
    QString text = ui->lineEdit_3->text();

    // 将文本转换为整数
    bool ok;
    //float f4Temp = text.toFloat(&ok);
    qint64 PulseWidthValue = text.toLongLong(&ok); // 将文本转换为整数

    if(ok)
    {
        if(AppConfig::blSingleChannelSettingEn == true)
        {
            QString currentText = ui->comboBox_2->currentText();
            /*使用std::unordered_map：创建了一个std::unordered_map，把comboBox_2的选项文本和AppConfig中对应的脉冲宽度变量的指针关联起来。*/
            static const std::unordered_map<QString, uint16_t*> channelMap =
            {
                {"CH1", &AppConfig::u2PulseWidth01},
                {"CH2", &AppConfig::u2PulseWidth02},
                {"CH3", &AppConfig::u2PulseWidth03},
                {"CH4", &AppConfig::u2PulseWidth04},
                {"CH5", &AppConfig::u2PulseWidth05},
                {"CH6", &AppConfig::u2PulseWidth06},
                {"CH7", &AppConfig::u2PulseWidth07},
                {"CH8", &AppConfig::u2PulseWidth08},
                {"CH9", &AppConfig::u2PulseWidth09},
                {"CH10", &AppConfig::u2PulseWidth10},
                {"CH11", &AppConfig::u2PulseWidth11},
                {"CH12", &AppConfig::u2PulseWidth12}
            };

            auto it = channelMap.find(currentText); //借助find方法查找当前选项对应的指针，若找到就对指针所指向的变量赋值。
            if (it != channelMap.end())
            {
                /*it->second：it 是一个指向 std::unordered_map 中元素的迭代器，每个元素是一个 std::pair<const QString, uint16_t*> 类型的对象。
                 * it->first 表示键（也就是 currentText），it->second 表示值，这里的值是一个指向 uint16_t 类型变量的指针。*/
                *(it->second) = (uint16_t)(PulseWidthValue * 100); // 脉宽的单位在Qt中为us IP核的时钟为100M  一个驱动时钟计数为10ns  *100意思是需要多个10ns
            }
        }
        else
        {
            AppConfig::u2PulseWidth00 = (uint16_t)(PulseWidthValue * 100);
        }
    }
}

/*各通道硬件增益槽函数*/
void SonicLaunchCfg::slots_gain()
{
    uint16_t* factors[] =
    {
        &AppConfig::u2HardwareAmplifyFactorCH01,
        &AppConfig::u2HardwareAmplifyFactorCH02,
        &AppConfig::u2HardwareAmplifyFactorCH03,
        &AppConfig::u2HardwareAmplifyFactorCH04,
        &AppConfig::u2HardwareAmplifyFactorCH05,
        &AppConfig::u2HardwareAmplifyFactorCH06,
        &AppConfig::u2HardwareAmplifyFactorCH07,
        &AppConfig::u2HardwareAmplifyFactorCH08,
        &AppConfig::u2HardwareAmplifyFactorCH09,
        &AppConfig::u2HardwareAmplifyFactorCH10,
        &AppConfig::u2HardwareAmplifyFactorCH11,
        &AppConfig::u2HardwareAmplifyFactorCH12
    };

    for (int i = 0; i < lineEditsGainConfig.size(); ++i)
    {
        // 获取 lineEdit 中的文本
        QString text = lineEditsGainConfig[i]->text();
        bool ok;
        // 将文本转换为整数
        int value = text.toInt(&ok);
        if (ok)
        {
            *factors[i] = static_cast<uint16_t>(value);
        }
    }
}

/*发射周期的槽函数*/
void SonicLaunchCfg::slots_period()
{
    // 获取 lineEdit 中的文本
    QString text = ui->lineEdit_4->text();
    // 将文本转换为整数
    bool ok;
    qint64 value = text.toLongLong(&ok);

    if (ok)
    {
        AppConfig::u4PulsePeriod = (uint32_t)value;
    }
}

/*零声时修正的槽函数*/
void SonicLaunchCfg::slots_ZeroTimingModify()
{
    // 获取 lineEdit 中的文本
    QString text = ui->lineEdit_ZeroModify->text();
    // 将文本转换为整数
    bool ok;
    qint64 value = text.toLongLong(&ok);

    if (ok)
    {
        AppConfig::u4ZeroModify = (uint32_t)value;
    }
}

/*采集起始点偏移的槽函数*/
void SonicLaunchCfg::slots_SetSamplingOffset()
{
    // 获取 lineEdit 中的文本
    QString text = ui->lineEdit_SamplingOffset->text();
    // 将文本转换为整数
    bool ok;
    qint64 value = text.toLongLong(&ok);

    if (ok)
    {
        AppConfig::u4SamplingOffset = (uint32_t)value;//采样点起始时间偏移值
    }
}


/*是否支持所有通道统一配置使能 =true 每个通道有各自的配置参数， false时 所有通道的位置一样，且为当前激活的通道配置参数，硬件增益除外*/
void SonicLaunchCfg::slots_SingleSetting()
{
    if (ui->rBtnSingleSetting->isChecked())
    {
       AppConfig::blSingleChannelSettingEn = true;
    }
    else
    {
       AppConfig::blSingleChannelSettingEn = false;
    }
}


/*初始化发射参数配置*/
void SonicLaunchCfg::initLaunchConfig()
{
    //触发通道选择
    ui->comboBox_2->setCurrentIndex(AppConfig::u2TriggerChannelSelected - 1); //通道序号与Index差一个1
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    //12通道共有的触发模式选择
    switch(AppConfig::u2TriggerMode)
    {
    case STOP_TRI:
        {
            //none operation
        }
        break;
    case ONCE_TRI:
        {
            ui->radioButton->setChecked(true);
            this->strTriggerMode = "单次触发";
        }
        break;
    case CONTINUE_TRI:
        {
            ui->radioButton_2->setChecked(true);
        }
        break;
    case ONCE_EXT_TRI:
        {
            ui->radioButton_3->setChecked(true);
            this->strTriggerMode = "单次外部触发";
        }
        break;
    case CONTINUE_EXT_TRI:
        {
            ui->radioButton_4->setChecked(true);
            this->strTriggerMode = "连续外部触发";
        }
        break;
    case SIGNAL_THRESHOLD_TRI:
        {
            ui->radioButton_5->setChecked(true);
            this->strTriggerMode = "信号阈值触发";
        }
        break;
//    case BSACN_SEQUENCE_TRI:
//        ui->radioButton_4->setChecked(true);
//        this->strTriggerMode = "B扫成像序列触发";
//        break;
    default:
        break;
    }

    int index = ui->comboBox_2->currentIndex();//获得索引
    if(AppConfig::blSingleChannelSettingEn == true)
    {
        //12通道各自独立的属性选择
        switch(index + 1) //获取通道索引值
        {
        case 1:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue01);//初始化电压选择按钮状态
                CheckSampleCountRbt(AppConfig::u4SampleCountCH01);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH01));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth01/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH01*40));
            }
            break;
        case 2:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue02);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH02);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH02));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth02/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH02*40));
            }
            break;
        case 3:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue03);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH03);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH03));
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH03));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth03/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH03*40));
            }
            break;
        case 4:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue04);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH04);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH04));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth04/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH04*40));
            }
            break;
        case 5:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue05);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH05);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH05));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth05/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH05*40));
            }
            break;
        case 6  :
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue06);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH06);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH06));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth06/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH06*40));
            }
            break;
        case 7:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue07);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH07);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH07));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth07/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH07*40));
            }
            break;
        case 8:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue08);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH08);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH08));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth08/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH08*40));
            }
            break;
        case 9:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue09);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH09);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH09));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth09/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH09*40));
            }
            break;
        case 10:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue10);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH10);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH10));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth10/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH10*40));
            }
            break;
        case 11:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue11);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH11);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH11));
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth11/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH11*40));
            }
            break;
        case 12:
            {
                CheckVoltageRbt(AppConfig::u2VoltageValue12);
                CheckSampleCountRbt(AppConfig::u4SampleCountCH12);
                ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH12));
                //ui->lineEdit_2->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH12)); //硬件增益
                ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth12/100));
                ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH12*40));
            }
            break;
        default:
            break;
        }
    }
    else //12路共用配置0通道
    {
        CheckVoltageRbt(AppConfig::u2VoltageValue00);
        CheckSampleCountRbt(AppConfig::u4SampleCountCH00);
        ui->lineEdit->setText(QString::number(AppConfig::u4DecimateSpanCH00));
        ui->lineEdit_2->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH00)); //硬件增益
        ui->lineEdit_3->setText(QString::number(AppConfig::u2PulseWidth00/100));
        ui->label_6->setText(QString::number(AppConfig::u4DecimateSpanCH00*40));
    }

    /*低通濾波界面选择初始化*/
    switch(AppConfig::u4LowPassFilteringCH00)
    {
    case 0:
        {
            ui->rB_NoLowPassFilter->setChecked(true);
        }
        break;
    case 3000:
        {
            ui->radioButton_17->setChecked(true);
        }
        break;
    case 30000:
        {
            ui->radioButton_18->setChecked(true);
        }
        break;
    case 60000:
        {
            ui->radioButton_19->setChecked(true);
        }
        break;
    case 200000:
        {
            ui->radioButton_20->setChecked(true);
        }
        break;
    case 500000:
        {
            ui->radioButton_21->setChecked(true);
        }
        break;
    default:
        break;
    }

    /*高通濾波界面选择初始化*/
    switch(AppConfig::u4HighPassFilteringCH00)
    {
    case 0:
        {
            ui->rB_NoHighPassFilter->setChecked(true);
        }
        break;
    case 10:
        {
            ui->radioButton_14->setChecked(true);
        }
        break;
    case 500:
        {
            ui->radioButton_15->setChecked(true);
        }
        break;
    case 2000:
        {
            ui->radioButton_16->setChecked(true);
        }
        break;
    default:
        break;
    }

    ui->lineEdit_4->setText(QString::number(AppConfig::u4PulsePeriod));//脉冲周期
    ui->rBtnSingleSetting->setChecked(AppConfig::blSingleChannelSettingEn);

    /*前端硬件运放增益数值初始化显示*/
    ui->lineEdit_2->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH01)); //硬件增益
    ui->lE_HardwareAmpCH2->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH02)); //硬件增益
    ui->lE_HardwareAmpCH3->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH03)); //硬件增益
    ui->lE_HardwareAmpCH4->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH04)); //硬件增益
    ui->lE_HardwareAmpCH5->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH05)); //硬件增益
    ui->lE_HardwareAmpCH6->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH06)); //硬件增益
    ui->lE_HardwareAmpCH7->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH07)); //硬件增益
    ui->lE_HardwareAmpCH8->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH08)); //硬件增益
    ui->lE_HardwareAmpCH9->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH09)); //硬件增益
    ui->lE_HardwareAmpCH10->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH10)); //硬件增益
    ui->lE_HardwareAmpCH11->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH11)); //硬件增益
    ui->lE_HardwareAmpCH12->setText(QString::number(AppConfig::u2HardwareAmplifyFactorCH12)); //硬件增益

    /*零声时数值初始化显示*/
    ui->lineEdit_ZeroModify->setText(QString::number(AppConfig::u4ZeroModify));

    /*采集起始点数值初始化显示*/
    ui->lineEdit_SamplingOffset->setText(QString::number(AppConfig::u4SamplingOffset));
}

/*保存当前更改设置的发射参数*/
void SonicLaunchCfg::saveConfig()
{
    AppConfig::writeConfig();
}

/*初始化上次选择的电压值*/
void SonicLaunchCfg::CheckVoltageRbt(uint16_t  u2VoltageValueX)
{
    if(u2VoltageValueX == 300)
    {
        ui->radioButton_10->setChecked(true);
        this->strVoltageValue = ui->radioButton_10->text();
    }
    else if(u2VoltageValueX == 600)
    {
        ui->radioButton_12->setChecked(true);
        this->strVoltageValue = ui->radioButton_12->text();
    }
    else if(u2VoltageValueX == 800)
    {
        ui->radioButton_11->setChecked(true);
        this->strVoltageValue = ui->radioButton_11->text();
    }
    else if(u2VoltageValueX == 1000)
    {
        ui->radioButton_13->setChecked(true);
        this->strVoltageValue = ui->radioButton_13->text();
    }
    else//自定义
    {
        ui->rB_CustomVoltage->setChecked(true);
        this->strVoltageValue = QString::number(u2VoltageValueX); //自定义
        ui->LE_CustomVoltage->setText(QString::number(u2VoltageValueX)); //显示自定义电压值
    }
}

/*初始化上次选择的采样点数*/
void SonicLaunchCfg::CheckSampleCountRbt(uint16_t  u2SampleCountX) //__Rbt is radioButton abbreviate name
{
    if(u2SampleCountX == 512)
    {
        ui->radioButton_6->setChecked(true);
        this->strSampleCount = ui->radioButton_6->text();
    }
    else if(u2SampleCountX == 1024)
    {
        ui->radioButton_8->setChecked(true);
        this->strSampleCount = ui->radioButton_8->text();
    }
    else if(u2SampleCountX == 2048)
    {
        ui->radioButton_7->setChecked(true);
        this->strSampleCount = ui->radioButton_7->text();
    }
    else if(u2SampleCountX == 4096)
    {
        ui->radioButton_9->setChecked(true);
        this->strSampleCount = ui->radioButton_9->text();
    }
    else
    {
        //no operation
    }
}

void SonicLaunchCfg::on_pBtSaveTriSetting_clicked()
{
    this->saveConfig(); //保存配置参数
    emit SigRefreshTriggerParameter();  //发射信号，更新主界面的参数状态栏显示
}
