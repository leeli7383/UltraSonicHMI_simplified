TEMPLATE = app
TARGET = UltraSonicHMI
QT += core gui network printsupport serialport widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    SonicLaunchCfg.cpp \
    main.cpp \
    qcustomplot.cpp \
    seg_2.cpp \
    seg_y.cpp \
    serial_port.cpp \
    udp_data.cpp \
    widget.cpp \
    vqcustomplot.cpp \
    qthelper.cpp \
    appconfig.cpp \
    Datapacket.cpp \
    filter.cpp \
    vTracer.cpp

HEADERS += \
    SonicLaunchCfg.h \
    qcustomplot.h \
    seg_2.h \
    seg_y.h \
    serial_port.h \
    udp_data.h \
    widget.h \
    vqcustomplot.h \
    vtracer.h \
    appconfig.h \
    head.h \
    Datapacket.h \
    filter.h \
    qthelper.h

FORMS += SonicLaunchCfg.ui widget.ui
RESOURCES += image.qrc

UI_DIR = generated/ui
INCLUDEPATH += $$OUT_PWD/generated

isEmpty(FFTW_PREFIX): FFTW_PREFIX = $$(CONDA_PREFIX)
isEmpty(FFTW_PREFIX): error("Activate the ultrasonic-hmi Conda environment or set FFTW_PREFIX.")

win32 {
    FFTW_INCLUDE = $$FFTW_PREFIX/Library/include
    FFTW_LIBRARY = $$FFTW_PREFIX/Library/lib/fftw3.lib
    mingw: FFTW_LIBRARY = $$FFTW_PREFIX/Library/bin/fftw3.dll
    msvc: QMAKE_CXXFLAGS += /utf-8
} else {
    FFTW_INCLUDE = $$FFTW_PREFIX/include
    LIBS += -L$$shell_quote($$FFTW_PREFIX/lib) -lfftw3
}

!exists($$FFTW_INCLUDE/fftw3.h): error("fftw3.h is missing from $$FFTW_INCLUDE.")
INCLUDEPATH += $$FFTW_INCLUDE

win32 {
    !exists($$FFTW_LIBRARY): error("FFTW library is missing: $$FFTW_LIBRARY")
    LIBS += $$shell_quote($$FFTW_LIBRARY)
}
