QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AbstractSyntaxTree/TreeNode/ConcreteNode/concatenation.cpp \
    AbstractSyntaxTree/TreeNode/ConcreteNode/empty.cpp \
    AbstractSyntaxTree/TreeNode/ConcreteNode/kleenestar.cpp \
    AbstractSyntaxTree/TreeNode/ConcreteNode/literal.cpp \
    AbstractSyntaxTree/TreeNode/ConcreteNode/union.cpp \
    AbstractSyntaxTree/TreeNode/basenode.cpp \
    AbstractSyntaxTree/TreeNode/basenodefactory.cpp \
    AbstractSyntaxTree/abstractsyntaxtree.cpp \
    AbstractSyntaxTree/regexparser.cpp \
    Automata/automaton.cpp \
    Automata/parser.cpp \
    Automata/state.cpp \
    main.cpp \
    uicontroller.cpp

HEADERS += \
    AbstractSyntaxTree/TreeNode/ConcreteNode/concatenation.h \
    AbstractSyntaxTree/TreeNode/ConcreteNode/empty.h \
    AbstractSyntaxTree/TreeNode/ConcreteNode/kleenestar.h \
    AbstractSyntaxTree/TreeNode/ConcreteNode/literal.h \
    AbstractSyntaxTree/TreeNode/ConcreteNode/union.h \
    AbstractSyntaxTree/TreeNode/basenode.h \
    AbstractSyntaxTree/TreeNode/basenodefactory.h \
    AbstractSyntaxTree/abstractsyntaxtree.h \
    AbstractSyntaxTree/regexparser.h \
    Automata/automaton.h \
    Automata/parser.h \
    Automata/state.h \
    uicontroller.h

FORMS += \
    uicontroller.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
