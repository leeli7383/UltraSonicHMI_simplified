/****************************************************************************
**
** 文件名: seg_2.c
** 作者: Zhou LiGui
** 创建日期: 2024年4月11日
** 功能描述: 对地震文件seg_2进行封装
**
** 详细描述: 对地震文件seg_2进行封装
**
** 版权所有 (C): 2024 GZU
**
** 注意事项:此部分内容参考贵阳电建院提供的代码，该部分打算重写由于时间原因搁置了
**
****************************************************************************/

#include "seg_2.h"
#include <QDebug>

#define CHANNEL_CNT 12 //通道的个数

seg_2::seg_2(QObject *parent) : QObject(parent)
{
    initParameters();
}

seg_2::~seg_2()
{
    // 可根据需要添加清理代码
}

void seg_2::initParameters()
{
    /********************变量参数初始化 **********************/
    ChannelCnt = CHANNEL_CNT;//获得总的数据行数
    SampleCnt = 4096;
    stringlength_file = 0;
    for (int i = 0; i < 17; ++i) {
        singlestringlength_file[i] = 0;
    }

    for (int i = 0; i < 11; ++i) {
        singlestringlength_data[i] = 0;
    }

    dataformat = 5;  // 64位浮点数
    datanum = 4096;
    fenge = 0; //在所有字符 串的最后是一个值为0的整数占2个字节这是字 符串结束的标志。
}

void seg_2::writeFileDescriptionBlock(std::ofstream& outfile)
{

    content_file[0]="GONGCHENGMIN";//工程名字
    content_file[0]+= " ";
    content_file[0]+= "GD";
    content_file[0]+= fenge;
    content_file[1]="ZHIDONGSHOUDONG";
    content_file[1]+=" ";
    content_file[1]+= QString::number(0);
    content_file[1]+= fenge;
    content_file[2]="SHENDUZENGJIAN";
    content_file[2]+=" ";
    content_file[2]+= QString::number(1);
    content_file[2]+= fenge;

    content_file[3]+=" ";
    content_file[3]="SHUANGKONGJIANJV"; //双孔间距
    content_file[3]+= QString::number(15);
    content_file[3]+= fenge;
    content_file[4]="FASHEKONGHAO";//发射孔号
    content_file[4]+=" ";
    content_file[4]+= "ZKS1";
    content_file[4]+= fenge;
    content_file[5]="FASHEKONGSHEN";//发射孔深
    content_file[5]+=" ";
    content_file[5]+= QString::number(900);
    content_file[5]+= fenge;
    content_file[6]="FASHEWEIZHI";//发射位置
    content_file[6]+=" ";
    content_file[6]+= QString::number(850);
    content_file[6]+= fenge;
    content_file[7]="JIESHOUKONGHAO";//接收孔号
    content_file[7]+=" ";
    content_file[7]+= "ZK2";
    content_file[7]+= fenge;
    content_file[8]="JIESHOUKONGSHEN";//接收孔深
    content_file[8]+=" ";
    content_file[8]+= QString::number(500);
    content_file[8]+= fenge;
    content_file[9]="JIESHOUQIDIAN";//接收起点
    content_file[9]+=" ";
    content_file[9]+= QString::number(485);
    content_file[9]+= fenge;
    content_file[10]="JIESHOUZHONGDIAN";//接收终点
    content_file[10]+=" ";
    content_file[10]+= QString::number(0);
    content_file[10]+= fenge;

    content_file[11]="CHUFAFANGSHI";//触发方式
    content_file[11]+=" ";
    content_file[11]+= QString::number(0);
    content_file[11]+= fenge;
    content_file[12]="CHUFADANGWEI";//触发档位
    content_file[12]+=" ";
    content_file[12]+= QString::number(1);
    content_file[12]+= fenge;
    content_file[13]="SHIFOULVBO";//是否滤波
    content_file[13]+=" ";
    content_file[13]+= QString::number(1);
    content_file[13]+= fenge;

    content_file[14]="LVBOJIPING";//滤波jiping?
    content_file[14]+=" ";
    content_file[14]+= QString::number(600);
    content_file[14]+= fenge;
    content_file[15]="LVBODITONG";//滤波低通
    content_file[15]+=" ";
    content_file[15]+= QString::number(600);
    content_file[15]+= fenge;
    content_file[16]="LVBOGAOTONG";//滤波高通
    content_file[16]+=" ";
    content_file[16]+= QString::number(0);
    content_file[16]+= fenge;

    for(int j=0; j<17; j++)
    {
        stringlength_file=stringlength_file+content_file[j].length();
        singlestringlength_file[j]=content_file[j].length()+2;  // 文件描述块中单个字符串长度
    }
    stringlength_file = 17*2+stringlength_file+2;  // 文件描述块字符串总长度


    bool ok;
    datanum=4096;  //  数据个数


    /********************文件描述块  ****************************/
    int block_id = 0x3a55;
    outfile.write((char *)&block_id, 2);   //  0-1，描述块标识符，低字节在前，高字节在后，2字节描述地震文件存储大小端方式

    int revision_num = 1;//版本号
    outfile.write((char *)&revision_num, 2);   //  2-3

    int iPointLarge = 4 * CHANNEL_CNT;//道指针子块的大小（M） 1个道指针大小为4个字节
    outfile.write((char *)&iPointLarge, 2);   //  4-5

    int trances = CHANNEL_CNT;
    outfile.write((char *)&trances, 2);   //  6-7  通道数

    int Ascii = 1;
    outfile.write((char *)&Ascii, 1);   //  8  字符串结束符大小

    int Ascii1 = 0;
    outfile.write((char *)&Ascii1, 1);   //  9 第1字符串结束符

    int Ascii2 = 0;//字符串终端大小
    outfile.write((char *)&Ascii2, 1);   //  10 第2字符串结束符

    int Lineend = 1;
    outfile.write((char *)&Lineend, 1);   //  11  行结束符大小 为01

    int Lineend1 = 10;
    outfile.write((char *)&Lineend1, 1);   //  12  第1个行结束符

    int Lineend2 = 0;
    outfile.write((char *)&Lineend2, 1);   //  13 第2个行结束符

    char Demo1[18] = {0};
    outfile.write((char *)&Demo1, 18);   //  14-31 保留部分

    int tranceP[1000] = {0};
    int base_file = 32 + 4 * CHANNEL_CNT + stringlength_file; //  32+(4*(N-1))

    int data_size[CHANNEL_CNT] = {0};


    switch (dataformat)
    {
    case 1:
        for(int i = 0; i < CHANNEL_CNT; i++)
        {
            data_size[i] = CalculateTraceDescriptorBlockSize() + datanum * 2;
        }
        break;
    case 2:
        for(int i = 0; i < CHANNEL_CNT; i++)
        {
            data_size[i] = CalculateTraceDescriptorBlockSize() + datanum * 4;
        }
        break;
    case 4:
        for(int i = 0; i < CHANNEL_CNT; i++)
        {
            data_size[i] = CalculateTraceDescriptorBlockSize() + datanum * 4;
        }
        break;
    case 5:
        for(int i = 0; i < CHANNEL_CNT; i++)
        {
            data_size[i] = CalculateTraceDescriptorBlockSize() + datanum * 8;
        }
        break;
    default:
        qDebug() << "error  data  format "<< endl;
        break;
    }

    for(int i = 0; i < CHANNEL_CNT; i++)
    {
        if(i == 0)
        {
            tranceP[i] = base_file;
        }
        else
        {
            tranceP[i] = tranceP[i-1] + data_size[i-1];    //  32+(4*(N-1))  指针
        }
    }

    for(int i = 0; i < trances; i++)
    {
        outfile.write((char *)&tranceP[i], 4);//   写入  32+(4*(N-1))  指针
    }

    for(int i = 0; i < 17; i++)
    {
        outfile.write((char *)&singlestringlength_file[i], 2);//   写入  文件字符串
        outfile << content_file[i].toStdString();
    }

    int end = 0;
    outfile.write((char *)&end, 2);//   写入  文件字符串    结束
    /********************文件描述块  over****************************/
}


void seg_2::writeChannelAndDataBlock(std::ofstream& outfile,
                                     const QVector<double>& Vf8VolageCh1,
                                     const QVector<double>& Vf8VolageCh2,
                                     const QVector<double>& Vf8VolageCh3,
                                     const QVector<double>& Vf8VolageCh4,
                                     const QVector<double>& Vf8VolageCh5,
                                     const QVector<double>& Vf8VolageCh6,
                                     const QVector<double>& Vf8VolageCh7,
                                     const QVector<double>& Vf8VolageCh8,
                                     const QVector<double>& Vf8VolageCh9,
                                     const QVector<double>& Vf8VolageCh10,
                                     const QVector<double>& Vf8VolageCh11,
                                     const QVector<double>& Vf8VolageCh12)
{
    /********************通道描述块与数据块  ****************************/

    //对12个道描述块与数据块的内容进行打包填充
    for(int ChannelIndex = 1; ChannelIndex <= CHANNEL_CNT; ChannelIndex++)
    {
        TraceDescriptorBlockStringSize = 0;
        //对当前某个道描述块内容进行打包填充
        content_data[0] = "SAMPLE_INTERVAL";
        content_data[0] += " ";
        double sample_interval = 4;
        content_data[0] += QString::number(sample_interval, 'g', 7);
        content_data[0] += fenge;
        content_data[1] = "SHUJVCHANGDU"; //数据长度
        content_data[1] += " ";
        content_data[1] += QString::number(datanum).rightJustified(10, '0');
        content_data[1] += fenge;
        content_data[2] = "CHUFATONGDAO";//触发通道
        content_data[2] += " ";
        content_data[2] += QString::number(ChannelIndex).rightJustified(10, '0');
        content_data[2] += fenge;
        content_data[3] = "CHUFADIANPING";//触发电平
        content_data[3] += " ";
        content_data[3] += QString::number(150).rightJustified(10, '0');
        content_data[3] += fenge;
        content_data[4] = "YANCHIDIANSHU";//延迟点数？
        content_data[4] += " ";
        content_data[4] += QString::number(ChannelIndex).rightJustified(10, '0');
        content_data[4] += fenge;
        content_data[5] = "CAIYANGTONGDAO";//采样通道
        content_data[5] += " ";
        content_data[5] += QString::number(ChannelIndex).rightJustified(10, '0');
        content_data[5] += fenge;
        content_data[6] = "DAOJIANJV";//道间距
        content_data[6] += " ";
        content_data[6] += QString::number(3300).rightJustified(10, '0');
        content_data[6] += fenge;
        content_data[7] = "TONGDAOHAO";
        content_data[7] += " ";
        content_data[7] += QString::number(ChannelIndex).rightJustified(10, '0');
        content_data[7] += fenge;
        content_data[8] = "FASHEWEIZHI";//发射位置
        content_data[8] += " ";
        content_data[8] += QString::number(850 + ChannelIndex).rightJustified(10, '0');
        content_data[8] += fenge;
        content_data[9] = "RECEIVER_LOCATION";//接收位置
        content_data[9] += " ";
        content_data[9] += QString::number(0+ChannelIndex).rightJustified(10, '0');
        content_data[9] += fenge;
        content_data[10] = "CHUZHI";
        content_data[10] += " ";
        content_data[10] += QString::number(1).rightJustified(10, '0');
        content_data[10] += fenge;

        //对当前某个道描述块的字节数进行统计
        for(int k = 0; k < 11; k++)
        {
            singlestringlength_data[k] = content_data[k].length() + 2;// 数据块单个字符串长度
        }
        //对当前某个道描述块的字节数进行统计
        for(int k = 0; k < 11; k++)
        {
            TraceDescriptorBlockStringSize += content_data[k].length() + 2 ;// 数据块单个字符串长度
        }

        /********************道描述块 ****************************/
        int block_id_trance = 0x4422;
        outfile.write((char *)&block_id_trance, 2);//0-1 道描述块标识符
        int block_miaoshu_large = 32 + TraceDescriptorBlockStringSize + 2;
        outfile.write((char *)&block_miaoshu_large, 2);//2-3 //描述块大小

        int shujvkuaisize;
        switch (dataformat)
        {
        case 1:
            shujvkuaisize = datanum * 2;// 16位整型
            break;
        case 2:
            shujvkuaisize = datanum * 4;// 32位整型
            break;
        case 4:
            shujvkuaisize = datanum * 4;// 32位浮点
            break;
        case 5:
            shujvkuaisize = datanum * 8;// 64位浮点
            break;
        default:
            qDebug() << "error  data  format "<< endl;
            break;
        }
        outfile.write((char *)&shujvkuaisize, 4);//4-7  ******* 相应数据块大小
        outfile.write((char *)&datanum, 4);//8-11  数据块中采样点数
        outfile.write((char *)&dataformat, 1);//12  数据格式代码  05h  64位浮点数

        char baoliu[19] = {0}; //13~31保留
        outfile.write((char *)&baoliu, 19);//13-31 保留

        for(int i = 0; i < 11; i++)
        {
            outfile.write((char *)&singlestringlength_data[i], 2);//   写入  文件字符串
            outfile << content_data[i].toStdString();
        }
        // 修改这里
        int end = 0;
        outfile.write((char *)&end, 2);//   写入  文件字符串    结束
        /********************道描述块   over****************************/

        /********************道数据块 start  ****************************/
        const QVector<double>* prDoubleData;// 使用 QVector<double> 存储数据
        switch (ChannelIndex)
        {
        case 1:
            prDoubleData = &Vf8VolageCh1;
            break;
        case 2:
            prDoubleData = &Vf8VolageCh2;
            break;
        case 3:
            prDoubleData = &Vf8VolageCh3;
            break;
        case 4:
            prDoubleData = &Vf8VolageCh4;
            break;
        case 5:
            prDoubleData = &Vf8VolageCh5;
            break;
        case 6:
            prDoubleData = &Vf8VolageCh6;
            break;
        case 7:
            prDoubleData = &Vf8VolageCh7;
            break;
        case 8:
            prDoubleData = &Vf8VolageCh8;
            break;
        case 9:
            prDoubleData = &Vf8VolageCh9;
            break;
        case 10:
            prDoubleData = &Vf8VolageCh10;
            break;
        case 11:
            prDoubleData = &Vf8VolageCh11;
            break;
        case 12:
            prDoubleData = &Vf8VolageCh12;
            break;
        default:
            break;
        }

        switch (dataformat)
        {
        case 1:
            outfile.write((char *)prDoubleData->data(), datanum * 2);
            break;
        case 2:
            outfile.write((char *)prDoubleData->data(), datanum * 4);
            break;
        case 4:
            outfile.write((char *)prDoubleData->data(), datanum * 4);
            break;
        case 5:
            outfile.write((char *)prDoubleData->data(), datanum * 8);
            break;
        default:
            qDebug() << "error  data  format "<< endl;
            break;
        }
    }
}


int  seg_2::CalculateTraceDescriptorBlockSize(void)
{

        TraceDescriptorBlockStringSize = 0;
        //对当前某个道描述块内容进行打包填充
        content_data[0] = "SAMPLE_INTERVAL";
        content_data[0] += " ";
        double sample_interval = 4.0;
        content_data[0] += QString::number(sample_interval, 'g', 7);
        content_data[0] += fenge;
        content_data[1] = "SHUJVCHANGDU";
        content_data[1] += " ";
        content_data[1] += QString::number(4096).rightJustified(10, '0');
        content_data[1] += fenge;
        content_data[2] = "CHUFATONGDAO";
        content_data[2] += " ";
        content_data[2] += QString::number(1).rightJustified(10, '0');
        content_data[2] += fenge;
        content_data[3] = "CHUFADIANPING";
        content_data[3] += " ";
        content_data[3] += QString::number(150).rightJustified(10, '0');
        content_data[3] += fenge;
        content_data[4] = "YANCHIDIANSHU";
        content_data[4] += " ";
        content_data[4] += QString::number(0).rightJustified(10, '0');
        content_data[4] += fenge;
        content_data[5] = "CAIYANGTONGDAO";
        content_data[5] += " ";
        content_data[5] += QString::number(4).rightJustified(10, '0');
        content_data[5] += fenge;
        content_data[6] = "DAOJIANJV";
        content_data[6] += " ";
        content_data[6] += QString::number(1).rightJustified(10, '0');
        content_data[6] += fenge;
        content_data[7] = "TONGDAOHAO";
        content_data[7] += " ";
        content_data[7] += QString::number(1).rightJustified(10, '0');
        content_data[7] += fenge;
        content_data[8] = "FASHEWEIZHI";
        content_data[8] += " ";
        content_data[8] += QString::number(850).rightJustified(10, '0');
        content_data[8] += fenge;
        content_data[9] = "RECEIVER_LOCATION";
        content_data[9] += " ";
        content_data[9] += QString::number(85).rightJustified(10, '0');
        content_data[9] += fenge;
        content_data[10] = "CHUZHI";
        content_data[10] += " ";
        content_data[10] += QString::number(10).rightJustified(10, '0');
        content_data[10] += fenge;

        //对当前某个道描述块的字节数进行统计
        for(int k = 0; k < 11; k++)
        {
            TraceDescriptorBlockStringSize += content_data[k].length() + 2;// 数据块单个字符串长度
        }
        int TraceDescriptorBlockSizeTemp = 32 + TraceDescriptorBlockStringSize + 2;
        return TraceDescriptorBlockSizeTemp;
}


void seg_2::processFile(const QString& fileName,
                        const QVector<double>& Vf8VolageCh1,
                        const QVector<double>& Vf8VolageCh2,
                        const QVector<double>& Vf8VolageCh3,
                        const QVector<double>& Vf8VolageCh4,
                        const QVector<double>& Vf8VolageCh5,
                        const QVector<double>& Vf8VolageCh6,
                        const QVector<double>& Vf8VolageCh7,
                        const QVector<double>& Vf8VolageCh8,
                        const QVector<double>& Vf8VolageCh9,
                        const QVector<double>& Vf8VolageCh10,
                        const QVector<double>& Vf8VolageCh11,
                        const QVector<double>& Vf8VolageCh12)
{
    std::string str_toDir_sg2 = fileName.toStdString();
    std::ofstream outfile(str_toDir_sg2, std::ios::binary | std::ios::out);

    if(!outfile)
    {
        qDebug() << " open error!" << endl;
    }
    else
    {
        qDebug() << " open sg2 " << endl;

        writeFileDescriptionBlock(outfile);
        writeChannelAndDataBlock(outfile, Vf8VolageCh1, Vf8VolageCh2, Vf8VolageCh3,
                                 Vf8VolageCh4, Vf8VolageCh5, Vf8VolageCh6,
                                 Vf8VolageCh7, Vf8VolageCh8, Vf8VolageCh9,
                                 Vf8VolageCh10, Vf8VolageCh11, Vf8VolageCh12);

        outfile.close();
    }
}
