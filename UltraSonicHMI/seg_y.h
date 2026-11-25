#ifndef SEG_Y_H
#define SEG_Y_H

#include <QString>
#include <QVector>
#include <fstream>
#include <vector>

class seg_y {
public:
    seg_y(const QString& filename, int sampleInterval, int sampleCount, int traceDistance);
    ~seg_y();

    bool open();
    bool writeTrace(int traceNumber, const QVector<float>& traceData);
    void close();

private:
    struct BinaryHeader {
        int32_t jobid;
        int32_t lino;
        int32_t reno;
        int16_t ntrpr;
        int16_t nart;
        int16_t hdt;
        int16_t dto;
        int16_t hns;
        int16_t nso;
        int16_t format;
        int16_t fold;
        int16_t tsort;
        int16_t vscode;
        int16_t hsfs;
        int16_t hsfe;
        int16_t hslen;
        int16_t hstyp;
        int16_t schn;
        int16_t hstas;
        int16_t hstae;
        int16_t htatyp;
        int16_t hcorr;
        int16_t bgrcv;
        int16_t rcvm;
        int16_t mfeet;
        int16_t polyt;
        int16_t vpol;
        char hunass[170];
    };

    struct TraceHeader {
        int32_t tracl;
        int32_t tracr;
        int32_t fldr;
        int32_t tracf;
        int32_t ep;
        int32_t cdp;
        int32_t cdpt;
        int16_t trid;
        int16_t nvs;
        int16_t nhs;
        int16_t duse;
        int32_t offset;
        int32_t gelev;
        int32_t selev;
        int32_t sdepth;
        int32_t gdel;
        int32_t sdel;
        int32_t swdep;
        int32_t gwdep;
        int16_t scalel;
        int16_t scalco;
        int32_t sx;
        int32_t sy;
        int32_t gx;
        int32_t gy;
        int16_t counit;
        int16_t wevel;
        int16_t swevel;
        int16_t sut;
        int16_t gut;
        int16_t sstat;
        int16_t gstat;
        int16_t tstat;
        int16_t laga;
        int16_t lagb;
        int16_t delrt;
        int16_t muts;
        int16_t mute;
        int16_t ns;
        int16_t dt;
        int16_t gain;
        int16_t igc;
        int16_t igi;
        int16_t corr;
        int16_t sfs;
        int16_t sfe;
        int16_t slen;
        int16_t styp;
        int16_t stas;
        int16_t stae;
        int16_t tatyp;
        int16_t afilf;
        int16_t afils;
        int16_t nofilf;
        int16_t nofils;
        int16_t lcf;
        int16_t hcf;
        int16_t lcs;
        int16_t hcs;
        int16_t year;
        int16_t day;
        int16_t hour;
        int16_t minute;
        int16_t sec;
        int16_t timbas;
        int16_t trwf;
        int16_t grnors;
        int16_t grnofr;
        int16_t grnlof;
        int16_t gaps;
        int16_t otrav;
        int32_t d1;
        int32_t f1;
        int32_t d2;
        int32_t f2;
        int32_t ungpow;
        int32_t unscale;
        int32_t ntr;
        int16_t mark;
        int16_t shortpad;
        char unass[28];
    };

    void writeFileHeader();
    void writeTraceHeader(int traceNumber);

    std::ofstream m_file;
    QString m_filename;
    int m_sampleInterval;
    int m_sampleCount;
    int m_traceDistance;
    BinaryHeader m_binaryHeader;
};

#endif // SEG_Y_H
