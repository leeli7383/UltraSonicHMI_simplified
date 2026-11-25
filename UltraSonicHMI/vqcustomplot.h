#ifndef MYQCUSTOMPLOT_H
#define MYQCUSTOMPLOT_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QThread>
#include <QVariant>
#include <qcustomplot.h>
#include <vtracer.h>
//#include "bsp_protocol.h"
//#include "serial_port/serial_port.h"
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <QGestureEvent>
#include <QPanGesture>
#include <QPinchGesture>
#include <QSwipeGesture>
#include <QtWidgets/QGesture>
#include <QtWidgets/QGestureEvent>
//#define DATA_READY_OUT  (300)




/*
 *****************************
 ************波形绘制************
 ******************************
 */

typedef struct
{
    Qt::Key X_Shortcut;
    Qt::Key Y_Shortcut;
    Qt::Key All_Shortcut;

}PlotShortcut;

class vQCustomPlot : public QCustomPlot
{
    Q_OBJECT
public:
//    explicit vQCustomPlot(QWidget *parent = nullptr)
//            //: QCustomPlot(parent)
//        {
//            setAttribute(Qt::WA_AcceptTouchEvents);
//        }
    explicit vQCustomPlot(QWidget *parent = nullptr);
    ~vQCustomPlot();
    QVector<double> qv_x, qv_y;
    PlotShortcut    myPlotShortcut;
    vTracer        *mxTracer = nullptr; //坐标跟随鼠标.使用时创建
    QString        *vName = nullptr;
    bool xAxisAuto = true;
    bool showEnable=true;
    uint8_t keyStatus;
    qint32 GraphShowTimerSet = 65;//波形刷新
    int lenSeasky = 0;
    QTimer  GraphShowTimer;
    bool TimerEnable;
    void TimerStart(void);
    void TimerStop(void);
    void vSetNameAddr(QString * addr);
    void setQCustomPlotTimer(qint32 tim)
    {
        GraphShowTimerSet = tim;
    }
    QTimer timerRangeUpdate;
    QTimer MultiTouchFilterTimer;
    bool MultiTouchFilterEnd = false;

    enum PLOT_TYPE_E
    {
        ORIGINAL,   //原始採集的電壓信號波形
        FFT,        //转换出的FFT频谱波形
        FILTERED,   //已滤波的电压波形
    };

public slots:
    void contextMenuRequest(QPoint pos);
    void moveLegend(void);
    void selectionChanged(void);
    void addRandomGraph(void);
    void removeSelectedGraph(void);
    void removeAllGraphs(void);
    void hideSelectedGraph(void);
    void hideAllGraph(void);
    void showAllGraph(void);
    bool isAllGraphHide(void);
    bool ishaveGraphHide(void);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void colorSet(void);
    void rescaleYAxis(void);
    void GraphShowSlot(void);
    void UltraSonicScope(const QVariant &rxData, QString GraphName, double xAxisFactor, PLOT_TYPE_E ePlotType);
    void showCsvData(double *data,int size);
    void addSeaskyGraph(void);
    void addSingleDefaultGraph(void);

    void vReadCsvData(void);
    void vSaveCsvData(void);

   // void fileOpenActionSlot();
    /*
    void BHD_Rev(Struct_bheader &b2h);
    void THD_Rev(Struct_Su240 &tr1);
    void Seis_Rev(float *s, int ns);
    void Seis_Rev(short *s, int ns);
    void Seis_Rev(int   *s, int ns);
    void swap(short *tni2);
    void swap(unsigned short *tni2) ;
    void swap(int *tni4);
    void swap(unsigned int *tni4);
    void swap(long *tni4);
    void swap(unsigned long *tni4);
    void swap(float *tnf4);
    void swap(double *tndd8);
    void slotActionInsert1();
*/
    void ZoomRangeUpdateTimerOut();
    void MultiTouchFilterTimerOut();
public:
signals:
    void CustomPlotRangeChanged(const QCPRange &rangeX,const QCPRange &rangeY);

protected:
    //滚轮
    void wheelEvent(QWheelEvent *ev);
    //鼠标点击事件
    void mousePressEvent(QMouseEvent *ev) ;
    void mouseWheelChanged();
//    //鼠标抬起事件
//    void mouseReleaseEvent(QMouseEvent *ev) ;
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *ev) ;
//    //鼠标进入区域
//    void enterEvent(QEvent *ev);
//    //鼠标离开区域
//    void leaveEvent(QEvent *ev);
//    //键盘事件
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);
    bool event(QEvent *ev);
    //bool event(QGestureEvent* ev);
signals:
    void ledShowEnable();
private:
    static constexpr double MAX_X_RANGE = 4200; // 设置最大X轴范围
    static constexpr double MAX_Y_RANGE = 3300;  // 设置最大Y轴范围
    QPointF currentTouchPointPos; //zhou li gui modify for (Multi) Touch and Gestures by  https://www.qcustomplot.com/index.php/support/forum/638
    //int Zhengyuandianhao;  //震源点号
    //int Jieshoudianhao;  //接收点号
   // int Shuipingjvli;  //震源点号
    //void OpenTheFile(QString);
   // QStringList fileNames_csv;  //打印所有选择的文件的路径
    //QString fileName;  // sg2路径
    //Struct_bheader juantou;
   // Struct_Su240 daotou;
private slots:
   //void setyAxisRange(QCPRange);
   void SetAxisRange( QCPRange &rangeX, QCPRange &rangeY);
protected:

};

#endif // MYQCUSTOMPLOT_H
