CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

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
    Automata/finitestateautomaton.cpp \
    Automata/nfatodfaconverter.cpp \
    Automata/parser.cpp \
    Automata/pushdownautomaton.cpp \
    Automata/pushdownstatelink.cpp \
    Automata/state.cpp

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
    Automata/finitestateautomaton.h \
    Automata/nfatodfaconverter.h \
    Automata/parser.h \
    Automata/pushdownautomaton.h \
    Automata/pushdownstatelink.h \
    Automata/state.h

INCLUDEPATH += \
    Automata

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
