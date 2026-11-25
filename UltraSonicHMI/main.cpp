/****************************************************************************
**
** 文件名: main.c
** 作者: Zhou LiGui
** 创建日期: 2025年4月11日
** 功能描述: 上位机的一些配置进行qsettings类的.ini文件进行保存和初始化调取
**
** 详细描述:使用的Qt版本为5.14.1
**
** 版权所有 (C): 2024 GZU
**
** 注意事项:
*  修订记录
*  2024-10-18 添加了高通滤波和低通滤波部分，对滤波配置存储级初始化进行了完善，FFT更新到原始波形为止不再单独开一个form进行显示，新增FFT三种状态转换，并添加了滤波设置错误提示框
*  2024-10-28 增加了波形保存文件为hex并通过加载恢复波形
*  2025-04-08 对整个工程进行整理，从widget.ccp剥离了多个类到单独的文件
*  2025-05-28 对整个工程进行整理，对毕设需要用的B-Scan成像的部分进行了删除
*
****************************************************************************/

#include "widget.h"
#include <QApplication>
#include "qthelper.h"
#include "appconfig.h"


int main(int argc, char *argv[])
{
    //::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashInfocallback);

    QApplication a(argc, argv);


    //调用AppConfig类的静态方法readConfig，该方法的作用是读取之前构建好路径的配置文件内容。一般而言，这个方法内部会包含文件读取、解析配置项等操作。
    AppConfig::ConfigFile = QString("%1/%2.ini").arg(QtHelper::appPath()).arg(QtHelper::appName());
    AppConfig::readConfig();


#if 1  //1：横屏    0：竖屏
    Widget w;
    w.show();
#else //竖屏状态下全聚焦成像的显示会有部分显示BUG
    Widget w;
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *w1 = scene->addWidget(&w);
    w1->setRotation(270);

    QGraphicsView *view = new QGraphicsView(scene);

    view->setWindowFlags(Qt::FramelessWindowHint);
    view->centerOn(0,0);
    view->resize(1920, 1200); //????????
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();
#endif

    return a.exec();
}
