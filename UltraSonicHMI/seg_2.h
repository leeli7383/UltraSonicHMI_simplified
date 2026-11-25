#ifndef SEG_2_H
#define SEG_2_H

#include <QObject>
#include <QString>
#include <QVector>
#include <fstream>


class seg_2 : public QObject
{
    Q_OBJECT
public:
    explicit seg_2(QObject *parent = nullptr);
    ~seg_2();

    // 处理文件的主要函数
    void processFile(const QString& fileName,
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
                     const QVector<double>& Vf8VolageCh12);
//    struct TraceDescriptorBlock
//    {
//        int
//        QString basePart, expPart, suffixPart;
//        QRect baseBounds, expBounds, suffixBounds, totalBounds, rotatedTotalBounds;
//        QFont baseFont, expFont;
//    };

private:
    // 初始化变量参数的函数
    void initParameters();

    // 写入文件描述块的函数
    void writeFileDescriptionBlock(std::ofstream& outfile);

    // 写入通道描述块与数据块的函数
    void writeChannelAndDataBlock(std::ofstream& outfile,
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
                                  const QVector<double>& Vf8VolageCh12);

    int CalculateTraceDescriptorBlockSize(void);
    int LineNumOfTheFile;
    int ChannelCnt;
    int SampleCnt;

    QString content_file[17];
    int stringlength_file;
    int singlestringlength_file[17];

    QString content_data[11];
    int stringlength_data[1000];
    //int stringlength_data[1000];
    int singlestringlength_data[11]; //11个描述属性
    int TraceDescriptorBlockStringSize; //该道描述块的字符串部分的大小
    int TraceDescriptorBlockSize; //该道描述块的大小
    int dataformat;
    int datanum;
    char fenge;
};

#endif // SEG_2_H
