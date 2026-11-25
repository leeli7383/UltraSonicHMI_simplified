#include "vqcustomplot.h"
#include <QDebug>
#include <QApplication>

/*
 *****************************
 ************波形绘制************
 ******************************
 */

const uint8_t KEY_CTRL = 0X01;
const uint8_t KEY_ALT  = 0X02;
const uint8_t KEY_ALT_CTRL=KEY_CTRL|KEY_ALT;


uint8_t u1virgin = 0X01;
vQCustomPlot::vQCustomPlot(QWidget *parent) : QCustomPlot(parent)
{
    //this->setOpenGl(true);
    this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                          QCP::iSelectLegend | QCP::iSelectPlottables);
    this->axisRect()->setupFullAxesBox();
    this->xAxis2->setVisible(true);
    this->xAxis2->setTickLabels(false);
    this->yAxis2->setVisible(true);
    this->yAxis2->setTickLabels(true);


    /*增加双指触摸相关代码*/
    this->setAttribute(Qt::WA_AcceptTouchEvents); //支持触摸缩放
#if 0
    // Grab all currently supported gestures in this widget.
    QList<Qt::GestureType> gestures; //对多种手势支持
    gestures << Qt::TapGesture
         << Qt::TapAndHoldGesture
         << Qt::PanGesture
         << Qt::PinchGesture
         << Qt::SwipeGesture;

    foreach (Qt::GestureType gesture, gestures)
        grabGesture(gesture);
#else
    grabGesture(Qt::PinchGesture); //仅对捏合缩放进行手势支持
#endif

#if 1
    // 使上下两个X轴的范围总是相等，使左右两个Y轴的范围总是相等,方框的对边
    connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this->xAxis2, SLOT(setRange(QCPRange)));
    connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)),
            this->yAxis2, SLOT(setRange(QCPRange)));
#endif
    //鼠标滚轮事件
//    connect(this, SIGNAL(&QCustomPlot::mouseWheel(QWheelEvent *event)),
//            this, SLOT(mouseWheelChanged()));
    //选中图例显示
    connect(this, SIGNAL(selectionChangedByUser()),
            this, SLOT(selectionChanged()));

    //双击修改坐标轴名称
    connect(this, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    //双击修改图例名称
    connect(this, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)),
            this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    // connect slot that shows a message in the status bar when a graph is clicked:
    // connect(this, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));

    connect(&GraphShowTimer,&QTimer::timeout,
            this,&vQCustomPlot::GraphShowSlot);

    connect(&timerRangeUpdate, SIGNAL(timeout()), this, SLOT(ZoomRangeUpdateTimerOut()));
    connect(&MultiTouchFilterTimer, SIGNAL(timeout()), this, SLOT(MultiTouchFilterTimerOut()));

    connect(this,SIGNAL(rangeChanged(QCPRange,QCPRange)),this,SLOT(SetAxisRange(QCPRange, QCPRange)));

    //显示图例名称
    this->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->legend->setFont(legendFont);
    this->legend->setSelectedFont(legendFont);
    this->legend->setSelectableParts(QCPLegend::spItems);

    //追踪光标
    mxTracer = new vTracer(this, this->graph(),TracerType::DataTracer);
    //快捷键
    myPlotShortcut.X_Shortcut = Qt::Key_X;//横向缩放
    myPlotShortcut.Y_Shortcut = Qt::Key_V;//纵向缩放
    myPlotShortcut.All_Shortcut = Qt::Key_A;
    this->keyStatus = 0;

    //设置轴的初始显示范围
    this->xAxis->setRange(0,8192);
    this->yAxis->setRangeUpper(3300);
    this->yAxis->setRange(0,3300);

    GraphShowTimer.setTimerType(Qt::PreciseTimer);
    TimerStop();
    //鼠标右键逻辑
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequest(QPoint)));

    //默认添加一条曲线，此项目为一个表框固定一条曲线
    addSingleDefaultGraph();

    //设置X轴和Y轴名称
    //this->xAxis->setLabel("时间/us");
    this->yAxis->setLabel("幅值/mV");

    this->replot(QCustomPlot::rpQueuedReplot);
}
vQCustomPlot::~vQCustomPlot()
{
    delete  mxTracer;
}
void vQCustomPlot::TimerStart(void)
{
    TimerEnable = true;
    if(!GraphShowTimer.isActive())
    {
        GraphShowTimer.start(GraphShowTimerSet);
    }
}
void vQCustomPlot::TimerStop(void)
{
    TimerEnable = false;
    if(GraphShowTimer.isActive())
    {
        GraphShowTimer.stop();
    }
}

void mouseWheelChanged()
{
    uint8_t u1Temp = 0X01;
    u1Temp = 0x02;
}

void vQCustomPlot::selectionChanged(void)
{
    // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        this->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        this->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    // synchronize selection of graphs with selection of corresponding legend items:
    for (int i=0; i<this->graphCount(); ++i)
    {
        QCPGraph *graph = this->graph(i);
        QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
    }
}

void vQCustomPlot::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
    // 双击轴标签，设置轴标签
    if (part == QCPAxis::spAxisLabel)
    {
        bool ok;
        QString newLabel = QInputDialog::getText(this,
                                                 QStringLiteral("更改坐标名称"),
                                                 QStringLiteral("新的坐标名称"), QLineEdit::Normal, axis->label(), &ok);
        if (ok)
        {
            axis->setLabel(newLabel);
            this->replot(QCustomPlot::rpQueuedReplot);
        }
    }
}

void vQCustomPlot::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
    // 通过双击图形的图例项来重命名图形
    Q_UNUSED(legend)

    if (item)
    {
        QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
        bool ok;
        QString newName =
                QInputDialog::getText(this,
                                      QStringLiteral("更改曲线名称"),
                                      QStringLiteral("新的曲线名称"),
                                      QLineEdit::Normal, plItem->plottable()->name(),
                                      &ok,Qt::WindowCloseButtonHint);
        if (ok)
        {
            plItem->plottable()->setName(newName);
            this->replot(QCustomPlot::rpQueuedReplot);
        }
    }
}
void vQCustomPlot::colorSet(void)
{
    QColor color = QColorDialog::getColor(Qt::red, this,
                                          QString::fromLocal8Bit("设置曲线颜色"),
                                          QColorDialog::ShowAlphaChannel);
    QPen graphPen;
    graphPen.setColor(color);
    graphPen.setWidthF(1.5);
    this->selectedGraphs().first()->setPen(graphPen);
}
#if 1
/*鼠标键盘事件*/
void vQCustomPlot::wheelEvent(QWheelEvent *ev)
{
    //如果选择了轴，只允许缩放该轴的方向
    //如果没有选择轴，两个方向都可以缩放
    uint8_t key_val = this->keyStatus;

    // 如果放大，检查放大后的范围是否超出限制值
#if 0
   if (xAxis->range().size() > MAX_X_RANGE)
   {
       double newMin = xAxis->range().lower + (xAxis->range().size() - MAX_X_RANGE) / 2;
       xAxis->setRange(newMin, newMin + MAX_X_RANGE);
   }
   if (yAxis->range().size() > MAX_Y_RANGE) {
       double newMin = yAxis->range().lower + (yAxis->range().size() - MAX_Y_RANGE) / 2;
       yAxis->setRange(newMin, newMin + MAX_Y_RANGE);
   }
#endif

    timerRangeUpdate.start(10); //启动当前通道启动缩放更新的定时器。定时器结束时拾取当前的缩放值
    //if(key_val != KEY_ALT_CTRL)
    {
        if ((yAxis->range().size() < MAX_Y_RANGE)&&(xAxis->range().size() < MAX_X_RANGE))
        {
            if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
                this->axisRect()->setRangeZoom(this->xAxis->orientation());

            else if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
                this->axisRect()->setRangeZoom(this->yAxis->orientation());
            else
                this->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
        }
    }
    qDebug()<<"Two-finger trigger accept";



    QCustomPlot::wheelEvent(ev);

    // 在当前波形窗口发出缩放值信号给其他通道
    QCPRange newRangeX = this->xAxis->range();
    QCPRange newRangeY = this->yAxis->range();
    // 发出自定义信号
    emit SetAxisRange(newRangeX,newRangeY);

}
#endif

void vQCustomPlot::ZoomRangeUpdateTimerOut()
{
    timerRangeUpdate.stop(); //

    // 在当前波形窗口发出缩放值信号给其他通道
    QCPRange newRangeX = this->xAxis->range();
    QCPRange newRangeY = this->yAxis->range();

    // 发出自定义信号，携带新的范围
    emit CustomPlotRangeChanged(newRangeX,newRangeY);

}


//X和Y只显示正半轴
void vQCustomPlot::SetAxisRange( QCPRange &rangeX, QCPRange &rangeY)
{
    this->yAxis->setRangeLower(0);
    this->xAxis->setRangeLower(0);

    //this->yAxis->setRangeUpper(3300*10);
    //this->xAxis->setRangeUpper(4096*10);
}

void vQCustomPlot::mousePressEvent(QMouseEvent *ev)
{
    //如果选择了一个轴，只允许拖动该轴的方向
    //如果没有选择轴，两个方向都可以拖动
    uint8_t key_val = this->keyStatus;
    if(key_val != KEY_ALT_CTRL)
    {
        if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
            this->axisRect()->setRangeDrag(this->xAxis->orientation());
        else if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
            this->axisRect()->setRangeDrag(this->yAxis->orientation());
        else
        {
            this->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
        }
    }
    QCustomPlot::mousePressEvent(ev);
}

void vQCustomPlot::rescaleYAxis(void)
{
    this->yAxis->rescale(true);
    this->replot(QCustomPlot::rpQueuedReplot);
}

/*光标追踪数据点*/
void vQCustomPlot::mouseMoveEvent(QMouseEvent *ev)
{
    /*处理触摸屏滑动无法同步缩放的问题-Start*/
    uint8_t key_val = this->keyStatus;

    timerRangeUpdate.start(10); //启动当前通道启动缩放更新的定时器。定时器结束时拾取当前的缩放值
    if(key_val != KEY_ALT_CTRL)
    {
        if ((yAxis->range().size() < MAX_Y_RANGE)&&(xAxis->range().size() < MAX_X_RANGE))
        {
            if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
                this->axisRect()->setRangeZoom(this->xAxis->orientation());

            else if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
                this->axisRect()->setRangeZoom(this->yAxis->orientation());
            else
                this->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
        }
    }
    /*处理触摸屏滑动无法同步缩放的问题-End*/

    if(mxTracer==nullptr)
    {
        return;
    }
    if(this->selectedGraphs().size() <= 0)
    {
        mxTracer->setVisible(false);
        QCustomPlot::mouseMoveEvent(ev);
        return;
    }
    mxTracer->setVisible(true);
    //获取容器
    QSharedPointer<QCPGraphDataContainer> tmpContainer;
    tmpContainer = this->selectedGraphs().first()->data();
    //获取x,y轴坐标
    double x = 0;
    double y = 0;
    {
        //使用二分法快速查找所在点数据
        x = this->xAxis->pixelToCoord(ev->pos().x());
        int low = 0, high = tmpContainer->size();
        while(high > low)
        {
            int middle = (low + high) / 2;
            if(x < tmpContainer->constBegin()->mainKey() ||
                    x > (tmpContainer->constEnd()-1)->mainKey())
                break;

            if(x == (tmpContainer->constBegin() + middle)->mainKey())
            {
                y = (tmpContainer->constBegin() + middle)->mainValue();
                break;
            }
            if(x > (tmpContainer->constBegin() + middle)->mainKey())
            {
                low = middle;
            }
            else if(x < (tmpContainer->constBegin() + middle)->mainKey())
            {
                high = middle;
            }
            if(high - low <= 1)
            {   //差值计算所在位置数据
                y = (tmpContainer->constBegin()+low)->mainValue() + ( (x - (tmpContainer->constBegin() + low)->mainKey()) *
                                                                      ((tmpContainer->constBegin()+high)->mainValue() - (tmpContainer->constBegin()+low)->mainValue()) ) /
                        ((tmpContainer->constBegin()+high)->mainKey() - (tmpContainer->constBegin()+low)->mainKey());
                break;
            }

        }
    }
    //更新Tracer
    QString text = "X:" + QString::number(x, 'g', 6) + " Y:" + QString::number(y, 'g', 6);
    mxTracer->updatePosition(x, y);
    mxTracer->setText(text);
    /*重新显示*/
    QCustomPlot::mouseMoveEvent(ev);
}

void vQCustomPlot::keyPressEvent(QKeyEvent *ev)
{
    int key = ev->key();
    if(key == myPlotShortcut.X_Shortcut)
    {
        if(this->keyStatus==KEY_ALT_CTRL)
        {
            this->axisRect()->setRangeZoom(Qt::Horizontal);//设置缩放
            this->axisRect()->setRangeDrag(Qt::Horizontal);//设置拖动
        }
    }
    else if(key == myPlotShortcut.Y_Shortcut)
    {
        if(this->keyStatus==KEY_ALT_CTRL)
        {
            this->axisRect()->setRangeZoom(Qt::Vertical);
            this->axisRect()->setRangeDrag(Qt::Vertical);
        }
    }
    else if(key == myPlotShortcut.All_Shortcut)
    {
//        qDebug()<<"a";
    }
    if(key== Qt::Key_Control)
    {
        this->keyStatus |= KEY_CTRL;
    }
    if(key== Qt::Key_Alt)
    {
        this->keyStatus |= KEY_ALT;
    }
    QCustomPlot::keyPressEvent(ev);
}
//键盘松开触发事件
void vQCustomPlot::keyReleaseEvent(QKeyEvent *ev)
{
    int key = ev->key();
    if(key== Qt::Key_Control)
    {
        this->keyStatus &= 0XFF^KEY_CTRL;
    }
    if(key== Qt::Key_Alt)
    {
        this->keyStatus &= 0XFF^KEY_ALT;
    }
    if(key == myPlotShortcut.X_Shortcut)
    {
        if(this->keyStatus==KEY_ALT_CTRL)
        {
            this->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);//设置缩放
            this->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);//设置拖动
        }
    }
    else if(key == myPlotShortcut.Y_Shortcut)
    {
        if(this->keyStatus==KEY_ALT_CTRL)
        {
            this->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
            this->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
        }
    }
    else if(key == myPlotShortcut.All_Shortcut)
    {
//        qDebug()<<"a";
    }
    QCustomPlot::keyReleaseEvent(ev);
}

#if  1
bool vQCustomPlot::event( QEvent *event )
{
    switch( event->type() )
        {
            case QEvent::Gesture:
            {
                QGestureEvent *gestureEve = static_cast<QGestureEvent*>(event);
                if( QGesture *pinch = gestureEve->gesture(Qt::PinchGesture) ) //捏合手势
                {
                    QPinchGesture *pinchEve = static_cast<QPinchGesture *>(pinch);
                    qreal scaleFactor = pinchEve->totalScaleFactor();
                    if( scaleFactor > 1.0 )
                    {
                        scaleFactor *= 50;
                        qDebug()<<"Two-finger trigger + ";
                    }
                    else
                    {
                        scaleFactor *= -50;
                        qDebug()<<"Two-finger trigger - ";
                    }
                    QWheelEvent *wheelEve = new QWheelEvent(currentTouchPointPos, scaleFactor, Qt::NoButton, Qt::NoModifier, Qt::Vertical);
                    this->wheelEvent(wheelEve);
                    return true;
                }
                else if( QGesture *swipe = gestureEve->gesture(Qt::SwipeGesture) ) //三指扫描
                {
                    qDebug()<<"Three-finger trigger";
                    // 恢复默认窗口缩放值
                    this->xAxis->setRange(0, 4200);
                    this->yAxis->setRangeUpper(3300);
                    this->yAxis->setRange(0, 3300);
                    this->replot(QCustomPlot::rpQueuedReplot);
                    return true;
                }
                break;
            }
            case QEvent::TouchBegin:
            case QEvent::TouchUpdate:
            case QEvent::TouchEnd:
            {
                QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
                QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
                if( touchPoints.count() == 1 ){
                    const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                    currentTouchPointPos = touchPoint0.pos();
                    // 不再模拟鼠标事件
                    // 如果需要取消之前创建的鼠标事件处理，则不做任何操作
                    return true;
                }
                break;
            }
            default:
                break;
        }

        return QWidget::event(event);
}
#else
bool vQCustomPlot::event(QEvent *e)
{
    static int index = 0;
    if (e->type() == QEvent::Gesture)
    {
        QGestureEvent* gEvent = static_cast<QGestureEvent *>(e);
        if (QGesture* pinch = gEvent->gesture(Qt::PinchGesture))
        {
            QPinchGesture* pEvent = static_cast<QPinchGesture *>(pinch);
            QPinchGesture::ChangeFlags changeFlags = pEvent->changeFlags();
            static qreal s_factor = 1.;
            int id = (int)pEvent->state();
            switch (id)
            {
            case Qt::GestureStarted:
            case Qt::GestureUpdated:
            {
                //setDragMode(QGraphicsView::NoDrag);
                //缩放比例的变化记录
                if (changeFlags & QPinchGesture::ScaleFactorChanged)
                {
                    s_factor = pEvent->totalScaleFactor();
                }
                //视图在变换期间应如何定位场景。
                //QGraphicsView使用此属性来决定在变换矩阵发生变化以及视图的坐标系发生变换时如何在视口中定位场景。
                //默认行为AnchorViewCenter可确保视图中心的场景点在变换期间保持不变（例如，旋转时，场景将显示为围绕视图中心旋转）。
                setTransformationAnchor(QGraphicsView::AnchorViewCenter);  //视图中心的场景点用作锚点
                if (_lastScaleFactor != 0)
                {
                    if (s_factor > _lastScaleFactor)
                    {
                        index++;
                    }
                    else if (s_factor < _lastScaleFactor)
                    {
                        index--;
                    }
                    if (index == 5) //超过5次放大，则认为有效.防抖操作
                    {
                        index = 0;
                        //zoomOnce(true);
                    }
                    else if (index == -5)
                    {
                        index = 0;
                        //zoomOnce(false);
                    }
                    _lastScaleFactor = s_factor;
                }
                else
                {
                    _lastScaleFactor = s_factor;
                }
                setTransformationAnchor(QGraphicsView::AnchorUnderMouse);  //鼠标下方的点用作锚点。
            }
                break;

            case Qt::GestureFinished:
            default:
            {
                qDebug() << "finished::";
                setDragMode(QGraphicsView::ScrollHandDrag);
                _lastScaleFactor = 0;
                s_factor = 1.;
            }
                break;
            }
        }
    }
    return QGraphicsView::event(e);
}
#endif
void vQCustomPlot::MultiTouchFilterTimerOut()
{
    MultiTouchFilterTimer.stop();
    MultiTouchFilterEnd = true;

}

void vQCustomPlot::moveLegend(void)
{
    if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
    {
        bool ok;
        int dataInt = contextAction->data().toInt(&ok);
        if (ok)
        {
            this->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
            //this->replot(QCustomPlot::rpQueuedReplot);
        }
    }
}
void vQCustomPlot::vSetNameAddr(QString * addr)
{
    this->vName=addr;
}
void vQCustomPlot::addRandomGraph(void)
{
    this->addGraph();
    if(this->vName[this->graphCount()-1]!=nullptr)
    {
        this->graph()->setName(this->vName[this->graphCount()-1]);
    }
    this->graph()->setLineStyle((QCPGraph::LineStyle::lsLine));
    this->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc,1.325));//数据点
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(1.3);
    this->graph()->setPen(graphPen);
    this->replot(QCustomPlot::rpQueuedReplot);
}
void vQCustomPlot::removeSelectedGraph(void)
{
    if (this->selectedGraphs().size() > 0)
    {
        this->removeGraph(this->selectedGraphs().first());
        this->replot(QCustomPlot::rpQueuedReplot);
    }
}

void vQCustomPlot::removeAllGraphs(void)
{
    this->clearGraphs();
    //vSetNameAddr(vTxName);
    addSingleDefaultGraph();
    //this->replot(QCustomPlot::rpQueuedReplot);
}
void vQCustomPlot::hideSelectedGraph()
{
    if (this->selectedGraphs().size() > 0)
    {
        //获取图像编号
        int index = 0;
        for(;index < this->graphCount(); index++)
        {
            if(this->graph(index)->name() == this->selectedGraphs().first()->name())
            {
                break;
            }
        }
        //可见性控制
        if(this->selectedGraphs().first()->visible())
        {
            this->selectedGraphs().first()->setVisible(false);
            this->legend->item(index)->setTextColor(Qt::gray);
        }
        else{
            this->selectedGraphs().first()->setVisible(true);
            this->legend->item(index)->setTextColor(Qt::black);
        }
        this->replot(QCustomPlot::rpQueuedReplot);
    }
}

void vQCustomPlot::hideAllGraph()
{
    int index = 0;
    for(index = 0;index < this->graphCount(); index++){
        this->graph(index)->setVisible(false);
        this->legend->item(index)->setTextColor(Qt::gray);
    }

    this->replot(QCustomPlot::rpQueuedReplot);
}

void vQCustomPlot::showAllGraph()
{
    int index = 0;
    for(;index < this->graphCount(); index++){
        this->graph(index)->setVisible(true);
        this->legend->item(index)->setTextColor(Qt::black);
    }
    this->replot(QCustomPlot::rpQueuedReplot);
}
bool vQCustomPlot::isAllGraphHide(void)
{
    int index = 0;
    for(;index < this->graphCount(); index++)
    {
        if(this->graph(index)->visible())
        {
            return false;
        }
    }
    return true;
}
bool vQCustomPlot::ishaveGraphHide(void)
{
    int index = 0;
    for(;index < this->graphCount(); index++)
    {
        if(!this->graph(index)->visible())
        {
            return true;
        }
    }
    return false;
}
void vQCustomPlot::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    /*
    if(this->legend->selectTest(pos,false) >= 0) // context menu on legend requested
    {
        menu->addAction(QStringLiteral("移动到左上角"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
        menu->addAction(QStringLiteral("移动到右上角"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
        menu->addAction(QStringLiteral("移动到右下角"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
        menu->addAction(QStringLiteral("移动到左下角"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
        menu->addSeparator();
    }
    if(this->graphCount()==0)
    {
        menu->addAction(QStringLiteral("自动添加多条曲线"), this, SLOT(addSeaskyGraph()));
    }
    else if (this->graphCount() > 0)
    {
        menu->addAction(QStringLiteral("移除所有曲线"), this, SLOT(removeAllGraphs()));
        menu->addSeparator();
        menu->addAction(QStringLiteral("AUTO"), this, SLOT(rescaleYAxis()));
        menu->addAction(QStringLiteral("实时刷新"), this, [=]
        {
            xAxisAuto = true;
        });
        //选择了曲线
        if (this->selectedGraphs().size() > 0)
        {
            menu->addSeparator();
            menu->addAction(QStringLiteral("设置曲线颜色"), this, SLOT(colorSet()));
            //所选曲线是否可见
            if(this->selectedGraphs().first()->visible())
            {
                menu->addAction(QStringLiteral("隐藏所选曲线"), this, SLOT(hideSelectedGraph()));
            }
            else
            {
                menu->addAction(QStringLiteral("显示所选曲线"), this, SLOT(hideSelectedGraph()));
            }
        }
        menu->addSeparator();
        //不是所有曲线被隐藏
        if(!isAllGraphHide())
        {
            menu->addAction(QStringLiteral("隐藏所有曲线"), this, SLOT(hideAllGraph()));
        }
        //有曲线未被显示
        if(ishaveGraphHide())
        {
            menu->addAction(QStringLiteral("显示所有曲线"), this, SLOT(showAllGraph()));
        }
    }
    menu->addSeparator();
    menu->addAction(QStringLiteral("导入Csv"),
                    this, SLOT(vReadCsvData()));
    menu->addAction(QStringLiteral("导出Csv"),
                    this, SLOT(vSaveCsvData()));
                    */
   // menu->addAction(QStringLiteral("保存通道1"),
                   // this, SLOT(slotActionInsert1()));

    menu->popup(this->mapToGlobal(pos));

}
void vQCustomPlot::addSeaskyGraph(void)
{
    showEnable = false;
    this->clearGraphs();
    for(int i=0;i<lenSeasky;i++)
    {
        addRandomGraph();
    }
    showEnable = true;
}
/**********************添加默认的单个曲线**************************/
void vQCustomPlot::addSingleDefaultGraph(void)
{
    showEnable = false;
    this->clearGraphs();
#if 1
    if(this->graphCount()==0)
    {
        this->addGraph();
        int j = this->graphCount();
      #if 0
        if(this->vName[this->graphCount()-1]!=nullptr)
        {
            this->graph()->setName(this->vName[this->graphCount()-1]);
        }
        #endif
        this->graph()->setLineStyle((QCPGraph::LineStyle::lsLine));
        this->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc,1.325));//数据点
        QPen graphPen;
        //graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
        graphPen.setColor(QColor(255, 0, 0));//Color->red
        graphPen.setWidthF(1.3);
        this->graph()->setPen(graphPen);

        //设置轴的初始显示范围
        this->xAxis->setRange(0,4200);
        this->yAxis->setRangeUpper(3300);
        this->yAxis->setRange(0,3300);

        this->replot(QCustomPlot::rpQueuedReplot);
    }
#endif
    showEnable = true;
}

void vQCustomPlot::UltraSonicScope(const QVariant &rxData, QString GraphName, double xAxisFactor, vQCustomPlot::PLOT_TYPE_E ePlotType)
{
    QVector<double> Vu1UltraSonicScope = rxData.value< QVector<double> >();
    lenSeasky = 0;
    removeAllGraphs(); //每次显示都清空画布

    if(ePlotType == vQCustomPlot::ORIGINAL)
    {
        this->xAxis->setRange(-10,xAxisFactor*4200);
        this->yAxis->setRange(-50,3500);
    }
    else if(ePlotType == vQCustomPlot::FILTERED)
    {
        this->xAxis->setRange(-10,xAxisFactor*4200);
        this->yAxis->setRange(-50,3500);
    }
    else if(ePlotType == vQCustomPlot::FFT)
    {
       this->xAxis->setRange(-10,20000);
       this->yAxis->setRange(-50,1700);
    }
    this->graph()->setName(GraphName);
    lenSeasky = Vu1UltraSonicScope.size();
    if((showEnable)&&(lenSeasky>0))
    {
        int j = this->graphCount(); //曲線的個數
        for (int i =0;(i<lenSeasky);i++)
        {
            this->graph(0)->addData(this->graph(0)->dataCount()*xAxisFactor,(double)Vu1UltraSonicScope[i]);
        }
    }

}

void vQCustomPlot::GraphShowSlot(void)
{
    static int upper,lower,length,last_upper;
    upper = this->xAxis->range().upper;
    lower = this->xAxis->range().lower;
    length = upper - lower;
    if(last_upper>upper)
    {
        xAxisAuto=false;
    }
    else if(upper>=this->xAxis->range().upper)
    {
        xAxisAuto =true;
    }
    if(xAxisAuto == true)
    {
        this->yAxis->rescale(true);
        this->xAxis->rescale(true);
        upper = this->xAxis->range().upper;
        this->xAxis->setRange(upper,length,Qt::AlignRight);
    }
    last_upper = upper;
    this->replot(QCustomPlot::rpQueuedReplot);
    emit ledShowEnable();
}
void vQCustomPlot::showCsvData(double *data,int size)
{
    if(this->graphCount()==size)
    {
        for (int i =0;(i<size);i++)
        {
            this->graph(i)->addData(this->graph(0)->dataCount(),(double)data[i]);
        }
    }
    else
    {
        if(this->graphCount()==0)
        {
            for(int i=0;i<size;i++)
            {
                addRandomGraph();
            }
        }
    }
}
void vQCustomPlot::vSaveCsvData(void)
{
    QString dirpath = QFileDialog::getSaveFileName(this, QStringLiteral("保存为Csv"),
                                                   qApp->applicationDirPath(),
                                                   QString(tr("File (*.csv)")));
    if(dirpath!=NULL)
    {
        QFile file(dirpath);
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(NULL,QStringLiteral("提示"),
                                  QStringLiteral("无法创建文件"));
            return ;
        }
        else
        {
            /*以下仅适用于key为int的数据*/
            int myGraphCount,posCount,maxSize,maxSizePos;
            QString str;
            QVector<double> saveData[24];
            myGraphCount = this->graphCount();
            /*将数据加载到QVector<double>，进行排列*/
            /*寻找数据最多的曲线*/
            maxSize = 0;
            posCount = 0;
            for(posCount = 0;posCount<myGraphCount;posCount++)
            {
                if(maxSize<this->graph(posCount)->dataCount())
                {
                    maxSize = this->graph(posCount)->dataCount();
                    maxSizePos = posCount;
                }
            }
            for(int i=0;i<24;i++)
            {
                /*分配指定大小的内存*/
                saveData[i].resize(maxSize+1);
            }
            for(posCount = 0;posCount<myGraphCount;posCount++)
            {
                for(int i = 0; i <this->graph(posCount)->dataCount(); i++)
                {
                    saveData[posCount][(int)(
                            this->graph(posCount)->data()->at(i)->key)] =
                            this->graph(posCount)->data()->at(i)->value;
                }
            }
            str.clear();
            str.append("time,");
            for(posCount=0;posCount<myGraphCount-1;posCount++)
            {
                str.append(this->graph(posCount)->name()+",");
            }
            str.append(this->graph(posCount)->name()+"\n");
            file.write(str.toStdString().c_str());//每次写入一行文本
            for(int i = 0; i < maxSize; i++)
            {
                str.clear();
                str.append(QString::number(i,10)+",");
                for(posCount=0;posCount<myGraphCount-1;posCount++)
                {
                    str.append(QString::number(saveData[posCount][i], 'f', 6)+",");
                }
                str.append(QString::number(saveData[posCount][i], 'f', 6)+"\n");
                file.write(str.toStdString().c_str());//每次写入一行文本
            }
            file.close();
        }
    }
}
void vQCustomPlot::vReadCsvData(void)
{
    QString dirpath = QFileDialog::getOpenFileName(this,QStringLiteral("导入Csv"),
                                                   qApp->applicationDirPath(), QString(tr("File (*.csv)")));
    if(dirpath!=NULL)
    {
        QFile file(dirpath);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(NULL,
                                  QStringLiteral("提示"),
                                  QStringLiteral("无法打开该文件"));
            return ;
        }
        else
        {
            QStringList list;
            static double csvData[24];
            list.clear();
            QTextStream csvStream(&file);
            //遍历行
            this->clearGraphs();
            this->replot(QCustomPlot::rpQueuedReplot);

            QString fileLine = csvStream.readLine();
            list =  fileLine.split(",",
                                  QString::SkipEmptyParts);
            for(int j=1;j<list.size();j++)
            {
                //this->vName[j-1] = list[j];
                this->vName[j-1] = list[j];
                addRandomGraph();
            }
            for(int i = 0;!csvStream.atEnd();i++)
            {
                QString fileLine = csvStream.readLine();
                list =  fileLine.split(",",
                                       QString::KeepEmptyParts);
                //根据","开分隔开每行的列
                for(int j=1;j<list.size();j++)
                {
                    csvData[j-1] = list[j].toDouble();
                }
                this->showCsvData(csvData,
                                  list.size()-1);
            }
            file.close();
        }
    }
}




