QT       += core gui serialport webenginecore webenginewidgets webengine location charts texttospeech multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
   MapZone.cpp \
   TimeZone.cpp \
   main.cpp \
   mainwindow.cpp \
   serialport.cpp \
   settingsdialog.cpp \
   telemetryframe.cpp \
   voicemanager.cpp

HEADERS += \
   MapZone.h \
   TimeZone.h \
   mainwindow.h \
   serialport.h \
   settingsdialog.h \
   telemetryframe.h \
   voicemanager.h


FORMS += \
   MapZone.ui \
   TimeZone.ui \
   mainwindow.ui \
   settingsdialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = antenne-satellite.ico

RESOURCES += \
   icone.qrc \
   map.qrc \
   song.qrc

DISTFILES += \
    ressource/bleep-126625.mp3





