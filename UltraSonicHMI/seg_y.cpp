/****************************************************************************
**
** 文件名: seg_y.c
** 作者: Zhou LiGui
** 创建日期: 2024年4月11日
** 功能描述: 对地震文件seg_y进行封装
**
** 详细描述: 对地震文件seg_y进行封装
**
** 版权所有 (C): 2024 GZU
**
** 注意事项:
**
****************************************************************************/
#include "seg_y.h"
#include <QDebug>
#include <QMessageBox>
#include <cstring>

seg_y::seg_y(const QString& filename, int sampleInterval,
             int sampleCount, int traceDistance)
     :m_filename(filename),
      m_sampleInterval(sampleInterval),
      m_sampleCount(sampleCount),
      m_traceDistance(traceDistance)
{

    // 初始化二进制卷头
    memset(&m_binaryHeader, 0, sizeof(m_binaryHeader));
    m_binaryHeader.hdt = m_sampleInterval;
    m_binaryHeader.hns = m_sampleCount;
    m_binaryHeader.format = 5; // IEEE 浮点数格式
    m_binaryHeader.mfeet = 1;  // 使用米作为单位
}

seg_y::~seg_y() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

bool seg_y::open() {
    m_file.open(m_filename.toStdString(), std::ios::binary | std::ios::out);
    if (!m_file.is_open()) {
        qWarning() << "Failed to open SEG-Y file:" << m_filename;
        return false;
    }

    // 写入3200字节的EBCDIC卷头(这里简单填充0)
    const int ebcdicHeaderSize = 3200;
    std::vector<char> ebcdicHeader(ebcdicHeaderSize, 0);
    m_file.write(ebcdicHeader.data(), ebcdicHeaderSize);

    // 写入二进制卷头
    writeFileHeader();

    return true;
}

void seg_y::writeFileHeader() {
    m_file.write(reinterpret_cast<const char*>(&m_binaryHeader), sizeof(BinaryHeader));
}

bool seg_y::writeTrace(int traceNumber, const QVector<float>& traceData) {
    if (!m_file.is_open()) {
        qWarning() << "SEG-Y file is not open";
        return false;
    }

    if (traceData.size() != m_sampleCount) {
        qWarning() << "Trace data size does not match sample count";
        return false;
    }

    // 写入道头
    writeTraceHeader(traceNumber);

    // 写入道数据
    m_file.write(reinterpret_cast<const char*>(traceData.constData()),
                m_sampleCount * sizeof(float));

    return m_file.good();
}

void seg_y::writeTraceHeader(int traceNumber) {
    TraceHeader header;
    memset(&header, 0, sizeof(header));

    header.tracl = traceNumber;  // 道序列号(线内)
    header.tracr = traceNumber;  // 道序列号(文件内)
    header.fldr = 1;             // 原始野外记录号
    header.tracf = traceNumber;  // 原始记录内的道号
    header.ep = 1;               // 能量点号
    header.cdp = 201;            // CDP号
    header.cdpt = 1;             // CDP道号
    header.ns = m_sampleCount;   // 本道采样点数
    header.dt = m_sampleInterval; // 采样间隔(微秒)
    header.offset = m_traceDistance / 1000; // 偏移距(米)

    // 写入道头
    m_file.write(reinterpret_cast<const char*>(&header), sizeof(TraceHeader));
}

void seg_y::close() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

#if 0 //一下为老版本代码从widget.cpp中拷贝过来备份的，应该没啥用
void Widget::BHD_Rev(Struct_bheader &b2h)
{
   swap(&b2h.jobid);
   swap(&b2h.lino);
   swap(&b2h.reno);
   swap(&b2h.ntrpr);
   swap(&b2h.nart);

   swap(&b2h.hdt);
   swap(&b2h.dto);
   swap(&b2h.hns);
   swap(&b2h.nso);

   swap(&b2h.format);
   swap(&b2h.fold);
   swap(&b2h.tsort);
   swap(&b2h.vscode);

   swap(&b2h.hsfs);
   swap(&b2h.hsfe);
   swap(&b2h.hslen);
   swap(&b2h.hstyp);

   swap(&b2h.schn);
   swap(&b2h.hstas);
   swap(&b2h.hstae);
   swap(&b2h.htatyp);

   swap(&b2h.hcorr);
   swap(&b2h.bgrcv);
   swap(&b2h.rcvm);
   swap(&b2h.mfeet);
   swap(&b2h.polyt);
   swap(&b2h.vpol);

   for (int i = 0; i < 170; i++)
   {
       swap(&b2h.hunass[i]);
   }
   return;
}


void Widget::THD_Rev(Struct_Su240 &tr1)
{
    swap(&tr1.tracl);    //1-4
    swap(&tr1.tracr);    //5-8
    swap(&tr1.fldr);     /* Original field record number */  //9-12
    swap(&tr1.tracf);    //rownum; /* Trace number within original field record */  //13-16
    swap(&tr1.ep);       //rownum;    //17-20
    swap(&tr1.cdp);      //1; /* Ensemble number (i.e. CDP, CMP, CRP,...) */  //21-24
    swap(&tr1.cdpt); //1;   //25-28
    swap(&tr1.trid); //LineNumOfTheFile;  //29-30
    swap(&tr1.nvs); //1;  //31-32
    swap(&tr1.nhs); //1;    //33-34
    swap(&tr1.duse); //1;   //35-36
    swap(&tr1.offset); //20;  //37-40
    swap(&tr1.gelev); //1;      //41-44
    swap(&tr1.selev); //1; /* Surface elevation at source. */  //45-48
    swap(&tr1.sdepth); //1; /* Source depth below surface (a positive number). */  //49-52
    swap(&tr1.gdel); //1; /* Datum elevation at receiver group. */  //53-56
    swap(&tr1.sdel); //1; /* Datum elevation at source. */  //57-60
    swap(&tr1.swdep); //1; /* Water depth at source. */   // 61-64
    swap(&tr1.gwdep); //1; /* Water depth at receiver group. */  //65-68
    swap(&tr1.scalel); //1;   // 69-70
    swap(&tr1.scalco); //1;  //71-72
    swap(&tr1.sx); //1; /* Source coordinate - X */    // 73-76
    swap(&tr1.sy); //rownum; /* Source coordinate - Y */  //77-80
    swap(&tr1.gx); //1; /* Group coordinate - X */         //81-84
    swap(&tr1.gy); //rownum; /* Group coordinate - Y */    // 85-88
    swap(&tr1.counit); //1;                              //   89-90
    swap(&tr1.wevel); //1; /* Weathering velocity. */      //  91-92
    swap(&tr1.swevel); //1; /* Subweathering velocity. */    // 93-94
    swap(&tr1.sut); //1; /* Uphole time at source in milliseconds. */   // 95-96
    swap(&tr1.gut); //1; /* Uphole time at receiver group in milliseconds. */   // 97-98
    swap(&tr1.sstat); //1; /* Source static correction in milliseconds. */    //  99-100
    swap(&tr1.gstat); //1; /* Group static correction in milliseconds.*/    //  101-102
    swap(&tr1.tstat); //1;         //  103-104
    swap(&tr1.laga); //1;        //  105-106
    swap(&tr1.lagb); //1;      //  107-108
    swap(&tr1.delrt); //1;   //  109-110
    swap(&tr1.muts); //1; /* mute time--start */   //  111-112
    swap(&tr1.mute); //1; /* mute time--end */    //   113-114
    swap(&tr1.ns); //2048; /* number of samples in this trace */  //  115-116
    swap(&tr1.dt); //10; /* sample interval; in micro-seconds */   //  117-118
    swap(&tr1.gain); //1;                       //  119-120
    swap(&tr1.igc); //1; /* instrument gain constant */   //  121-122
    swap(&tr1.igi); //1; /* instrument early or initial gain */   //  123-124
    swap(&tr1.corr); //1;                             //  125-126
    swap(&tr1.sfs); //1; /* sweep frequency at start */  //  127-128
    swap(&tr1.sfe); //1; /* sweep frequency at end */    //  129-130
    swap(&tr1.slen); //1; /* sweep length in ms */    //   131-132
    swap(&tr1.styp); //1;                           //  133-134
    swap(&tr1.stas); //1; /* sweep trace length at start in ms */   //  135-136
    swap(&tr1.stae); //1; /* sweep trace length at end in ms */     //  137-138
    swap(&tr1.tatyp); //1; /* taper type: 1); //linear, 2); //cos^2, 3); //other */  //  139-140
    swap(&tr1.afilf); //1; /* alias filter frequency if used */      //  141-142
    swap(&tr1.afils); //1; /* alias filter slope */            //  143-144
    swap(&tr1.nofilf); //1; /* notch filter frequency if used */   //  145-146
    swap(&tr1.nofils); //1; /* notch filter slope */   //  147-148
    swap(&tr1.lcf); //1; /* low cut frequency if used */  // 149-150
    swap(&tr1.hcf); //1; /* high cut frequncy if used */  //  151-152
    swap(&tr1.lcs); //1; /* low cut slope */           //  153-154
    swap(&tr1.hcs); //1; /* high cut slope */         //   155-156
    swap(&tr1.year); //1; /* year data recorded */   //  157-158
    swap(&tr1.day); //1; /* day of year */          //  159-160
    swap(&tr1.hour); //1; /* hour of day (24 hour clock) */   // 161-162
    swap(&tr1.minute); //1; /* minute of hour */      //  163-164
    swap(&tr1.sec); //1; /* second of minute */    //  165-166
    swap(&tr1.timbas); //1;   //  167-168
    swap(&tr1.trwf); //1;        //  169-170
    swap(&tr1.grnors); //1;  //  171-172
    swap(&tr1.grnofr); //1;  //  173-174
    swap(&tr1.grnlof); //1;  //  175-176
    swap(&tr1.gaps); //1; /* gap size (total number of groups dropped) */   //177-178
    swap(&tr1.otrav); //1;    //
    swap(&tr1.d1); //1; /* sample spacing for non-seismic data */   //
    swap(&tr1.f1); //1; /* first sample location for non-seismic data */  //
    swap(&tr1.d2); //1; /* sample spacing between traces */  //
    swap(&tr1.f2); //1; /* first trace location */  //
    swap(&tr1.ungpow); //1; /* negative of power used for dynamic range compression */   //
    swap(&tr1.unscale); //1; /* reciprocal of scaling factor to normalize range */  //
    swap(&tr1.ntr); //1; /* number of traces */  //
    swap(&tr1.mark); //1; /* mark selected traces */  //
    swap(&tr1.shortpad); //1; /* alignment padding */  //
    //swap(&unass[14]); //1;
    for(int i=0;i<14;i++){
       swap(&tr1.unass[i]); //33;
    }
    return;
}


void Widget:: Seis_Rev(float *s, int ns)
{
    for (int i = 0; i < ns; i++)
    {
        swap(&s[i]);
    }
}

void Widget:: Seis_Rev(short *s, int ns)
{
    for (int i = 0; i < ns; i++)
    {
        swap(&s[i]);
    }
}

void Widget:: Seis_Rev(int   *s, int ns)
{
    for (int i = 0; i < ns; i++)
    {
        swap(&s[i]);
    }
}



//字节交换(swapbytes)函数重载
void Widget:: swap(short *tni2) //swap_short_2
{
    *tni2=(((*tni2>>8)&0xff) | ((*tni2&0xff)<<8));
}
void Widget:: swap(unsigned short *tni2) //swap_u_short_2
{
    *tni2=(((*tni2>>8)&0xff) | ((*tni2&0xff)<<8));
}
void Widget:: swap(int *tni4) //swap_int_4
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}
void Widget:: swap(unsigned int *tni4) //swap_u_int_4
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}
void Widget:: swap(long *tni4) //swap_long_4
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}
void Widget:: swap(unsigned long *tni4) //swap_u_long_4
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}
void Widget:: swap(float *tnf4) //swap_float_4
{
    int *tni4=(int *)tnf4;
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}
void Widget:: swap(double *tndd8) //swap_double_8
{
    char *tnd8=(char *)tndd8;
    char tnc;
    tnc= *tnd8;
    *tnd8= *(tnd8+7);
    *(tnd8+7)=tnc;
    tnc= *(tnd8+1);
    *(tnd8+1)= *(tnd8+6);
    *(tnd8+6)=tnc;
    tnc= *(tnd8+2);
    *(tnd8+2)= *(tnd8+5);
    *(tnd8+5)=tnc;
    tnc= *(tnd8+3);
    *(tnd8+3)= *(tnd8+4);
    *(tnd8+4)=tnc;
}
#endif
