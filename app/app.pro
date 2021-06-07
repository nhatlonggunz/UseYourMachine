QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    app.cpp

HEADERS += \
    app.h

FORMS += \
    app.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/AutomataLib/release/ -lAutomataLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/AutomataLib/debug/ -lAutomataLib
else:unix: LIBS += -L$$OUT_PWD/../lib/AutomataLib/ -lAutomataLib

INCLUDEPATH += $$PWD/../lib/AutomataLib
DEPENDPATH += $$PWD/../lib/AutomataLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/release/libAutomataLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/debug/libAutomataLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/release/AutomataLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/debug/AutomataLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/libAutomataLib.a
