include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
        TestAST.h \
        TestGeneral.h

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/AutomataLib/release/ -lAutomataLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/AutomataLib/debug/ -lAutomataLib
else:unix: LIBS += -L$$OUT_PWD/../lib/AutomataLib/ -lAutomataLib

INCLUDEPATH += $$PWD/../lib/AutomataLib
DEPENDPATH += $$PWD/../lib/AutomataLib

copydata.commands = $(COPY_DIR) $$shell_path($$PWD/InputFiles) $$shell_path($$OUT_PWD/InputFiles)
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/release/libAutomataLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/debug/libAutomataLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/release/AutomataLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/debug/AutomataLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/AutomataLib/libAutomataLib.a
